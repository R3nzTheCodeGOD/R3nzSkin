#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <psapi.h>
#include <string>
#include <thread>
#include <tlhelp32.h>
#include <regex>
#include <msclr/marshal_cppstd.h>

#include "Injector.hpp"
#include "R3nzUI.hpp"
#include "xorstr.hpp"
#include "lazy_importer.hpp"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Globalization;
using namespace System::Net;

#define xor_clrstr_w(x) msclr::interop::marshal_as<String^>(static_cast<std::wstring>(_XorStrW(x)))

proclist_t WINAPI Injector::findProcesses(const std::wstring& name) noexcept
{
	auto process_snap{ LI_FN(CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, 0) };
	proclist_t list;

	if (process_snap == INVALID_HANDLE_VALUE)
		return list;

	PROCESSENTRY32W pe32{};
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (LI_FN(Process32FirstW).get()(process_snap, &pe32)) {
		if (pe32.szExeFile == name)
			list.push_back(pe32.th32ProcessID);

		while (LI_FN(Process32NextW).get()(process_snap, &pe32)) {
			if (pe32.szExeFile == name)
				list.push_back(pe32.th32ProcessID);
		}
	}

	LI_FN(CloseHandle)(process_snap);
	return list;
}

bool WINAPI Injector::isInjected(const std::uint32_t pid) noexcept
{
	auto hProcess{ LI_FN(OpenProcess)(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid) };

	if (nullptr == hProcess)
		return false;

	HMODULE hMods[1024];
	DWORD cbNeeded{};

	if (LI_FN(K32EnumProcessModules)(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (auto i{ 0u }; i < (cbNeeded / sizeof(HMODULE)); ++i) {
			TCHAR szModName[MAX_PATH];
			if (LI_FN(K32GetModuleBaseNameW)(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				if (std::wcscmp(szModName, _XorStrW(L"R3nzSkin.dll")) == 0) {
					LI_FN(CloseHandle)(hProcess);
					return true;
				}
			}
		}
	}
	LI_FN(CloseHandle)(hProcess);
	return false;
}

bool WINAPI Injector::inject(const std::uint32_t pid) noexcept
{
	TCHAR current_dir[MAX_PATH];
	LI_FN(GetCurrentDirectoryW)(MAX_PATH, current_dir);
	const auto handle{ LI_FN(OpenProcess)(PROCESS_ALL_ACCESS, false, pid) };

	if (!handle || handle == INVALID_HANDLE_VALUE)
		return false;

	FILETIME ft{};
	SYSTEMTIME st{};
	LI_FN(GetSystemTime)(&st);
	LI_FN(SystemTimeToFileTime)(&st, &ft);
	FILETIME create{}, exit{}, kernel{}, user{};
	LI_FN(GetProcessTimes)(handle, &create, &exit, &kernel, &user);

	const auto delta{ 10 - static_cast<std::int32_t>((*reinterpret_cast<std::uint64_t*>(&ft) - *reinterpret_cast<std::uint64_t*>(&create.dwLowDateTime)) / 10000000U) };

	if (delta > 0)
		std::this_thread::sleep_for(std::chrono::seconds(delta));

	const auto dll_path{ std::wstring(current_dir) + _XorStrW(L"\\R3nzSkin.dll") };

	if (const auto f{ std::ifstream(dll_path) }; !f.is_open()) {
		LI_FN(MessageBoxW)(nullptr, _XorStrW(L"R3nzSkin.dll file could not be found.\nTry reinstalling the cheat."), _XorStrW(L"R3nzSkin"), MB_ICONERROR | MB_OK);
		LI_FN(CloseHandle)(handle);
		return false;
	}

	const auto dll_path_remote{ LI_FN(VirtualAllocEx).get()(handle, nullptr, (dll_path.size() + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) };

	if (!dll_path_remote) {
		LI_FN(CloseHandle)(handle);
		return false;
	}

	if (!LI_FN(WriteProcessMemory).get()(handle, dll_path_remote, dll_path.data(), (dll_path.size() + 1) * sizeof(wchar_t), nullptr)) {
		LI_FN(VirtualFreeEx).get()(handle, dll_path_remote, 0u, MEM_RELEASE);
		LI_FN(CloseHandle)(handle);
		return false;
	}

	HANDLE thread{};
	LI_FN(NtCreateThreadEx).nt_cached()(&thread, GENERIC_ALL, nullptr, handle, reinterpret_cast<LPTHREAD_START_ROUTINE>(LI_FN(GetProcAddress).get()(LI_FN(GetModuleHandleW).get()(_XorStrW(L"kernel32.dll")), _XorStr("LoadLibraryW"))), dll_path_remote, FALSE, NULL, NULL, NULL, nullptr);

	if (!thread || thread == INVALID_HANDLE_VALUE) {
		LI_FN(VirtualFreeEx).get()(handle, dll_path_remote, 0u, MEM_RELEASE);
		LI_FN(CloseHandle)(handle);
		return false;
	}

	LI_FN(WaitForSingleObject)(thread, INFINITE);
	LI_FN(CloseHandle)(thread);
	LI_FN(VirtualFreeEx).get()(handle, dll_path_remote, 0u, MEM_RELEASE);
	LI_FN(CloseHandle)(handle);
	return true;
}

void WINAPI Injector::enableDebugPrivilege() noexcept
{
	HANDLE token{};
	if (OpenProcessToken(LI_FN(GetCurrentProcess).get()(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
		LUID value;
		if (LookupPrivilegeValueW(nullptr, _XorStrW(SE_DEBUG_NAME), &value)) {
			TOKEN_PRIVILEGES tp{};
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = value;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr))
				LI_FN(CloseHandle)(token);
		}
	}
}

void Injector::autoUpdate()
{
	/* do not even bother if we are from Chinese regions and Brazil */
	auto whitelisted_regions = gcnew array<String^> {
		xor_clrstr_w(L"zh-CN"),
		xor_clrstr_w(L"zh-TW"),
		xor_clrstr_w(L"zh-HK"),
		xor_clrstr_w(L"zh-MO"),
		xor_clrstr_w(L"zh-SG"),
		xor_clrstr_w(L"pt-BR")
	};
	auto current_culture = CultureInfo::CurrentCulture;
	auto region = current_culture->Name;
	for each (auto whitelisted_region in whitelisted_regions)
	{
		if (region->Equals(whitelisted_region, StringComparison::OrdinalIgnoreCase))
		{
			return;
		}
	}
	
	auto client = gcnew WebClient();
	ServicePointManager::Expect100Continue = true;
	ServicePointManager::SecurityProtocol = SecurityProtocolType::Tls12;
	client->Headers->Add(xor_clrstr_w(L"User-Agent"), xor_clrstr_w(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/115.0"));

	try
	{
		auto json = msclr::interop::marshal_as<std::string>(client->DownloadString(xor_clrstr_w(L"https://api.github.com/repos/R3nzTheCodeGOD/R3nzSkin/releases/latest")));
		std::regex tagnameRegex(_XorStr("\"tag_name\"\\s*:\\s*\"([^\"]+)"));
		std::regex urlRegex(_XorStr("\"browser_download_url\"\\s*:\\s*\"([^\"]+)"));
		std::regex dateRegex(_XorStr("\"created_at\"\\s*:\\s*\"([^\"]+)"));

		if (std::smatch tagnameMatch; std::regex_search(json, tagnameMatch, tagnameRegex))
		{
			auto version = gcnew String(tagnameMatch[1].str().c_str());
			if (std::smatch dateMatch; std::regex_search(json, dateMatch, dateRegex))
			{
				if (!System::IO::File::Exists(xor_clrstr_w(L"R3nzSkin.dll")))
				{
					throw gcnew Exception(xor_clrstr_w(L"Failed to find R3nzSkin.dll in the current directory"));
				}
				const auto date_of_github_release = DateTime::ParseExact(gcnew String(dateMatch[1].str().c_str()), xor_clrstr_w(L"yyyy-MM-ddTHH:mm:ssZ"), CultureInfo::InvariantCulture).ToString(xor_clrstr_w(L"dd.MM.yyyy HH:00"));
				const auto date_of_current_release = System::IO::File::GetLastWriteTime(xor_clrstr_w(L"R3nzSkin.dll")).ToString(xor_clrstr_w(L"dd.MM.yyyy HH:00"));
				if (date_of_current_release != date_of_github_release)
				{
					const auto date_of_github_release_class = DateTime::ParseExact(date_of_github_release, xor_clrstr_w(L"dd.MM.yyyy HH:00"), CultureInfo::InvariantCulture);
					const auto date_of_current_release_class = DateTime::ParseExact(date_of_current_release, xor_clrstr_w(L"dd.MM.yyyy HH:00"), CultureInfo::InvariantCulture);
					if (date_of_current_release_class > date_of_github_release_class)
					{
						return;
					}

					const auto result = MessageBox::Show(xor_clrstr_w(L"New version is available on GitHub\nWould you like to download it now?"), xor_clrstr_w(L"R3nzSkin"), MessageBoxButtons::YesNo, MessageBoxIcon::Information);
					if (result == DialogResult::Yes)
					{
						if (std::smatch urlMatch; std::regex_search(json, urlMatch, urlRegex))
						{
							auto url = gcnew String(urlMatch[1].str().c_str());
							auto file = String::Format(xor_clrstr_w(L"R3nzSkin_{0}.zip"), version);
							client->DownloadFile(url, file);

							System::IO::Compression::ZipFile::ExtractToDirectory(file, xor_clrstr_w(L"R3nzSkin"));
							System::IO::File::Delete(file);
							System::IO::File::Delete(xor_clrstr_w(L"R3nzSkin.dll"));
							System::IO::File::Move(xor_clrstr_w(L"R3nzSkin\\R3nzSkin_Injector.exe"), String::Format(xor_clrstr_w(L"R3nzSkin_Injector_{0}.exe"), version));
							System::IO::File::Move(xor_clrstr_w(L"R3nzSkin\\R3nzSkin.dll"), xor_clrstr_w(L"R3nzSkin.dll"));
							System::IO::Directory::Delete(xor_clrstr_w(L"R3nzSkin"));
							
							auto process_info = gcnew System::Diagnostics::ProcessStartInfo();
							process_info->Arguments = xor_clrstr_w(L"/C choice /C Y /N /D Y /T 1 & del \"") + System::Diagnostics::Process::GetCurrentProcess()->MainModule->FileName + xor_clrstr_w(L"\"");
							process_info->CreateNoWindow = true;
							process_info->FileName = xor_clrstr_w(L"cmd.exe");
							process_info->WindowStyle = System::Diagnostics::ProcessWindowStyle::Hidden;
							System::Diagnostics::Process::Start(process_info);
							System::Diagnostics::Process::Start(String::Format(xor_clrstr_w(L"R3nzSkin_Injector_{0}.exe"), version));
			
							Environment::Exit(0);
						}
					}
				}
			}
		}
	}
	catch (Exception^ e)
	{
		MessageBox::Show(String::Format(xor_clrstr_w(L"{0} - {1}"), e->Message, e->StackTrace->Substring(5)), xor_clrstr_w(L"R3nzSkin"), MessageBoxButtons::OK, MessageBoxIcon::Error);
		Environment::Exit(0);
	}
}

void Injector::run() noexcept
{
	enableDebugPrivilege();
	while (true) {
		const auto& league_client_processes{ Injector::findProcesses(_XorStrW(L"LeagueClient.exe")) };
		const auto& league_processes{ Injector::findProcesses(_XorStrW(L"League of Legends.exe")) };

		R3nzSkinInjector::gameState = (league_processes.size() > 0) ? true : false;
		R3nzSkinInjector::clientState = (league_client_processes.size() > 0) ? true : false;
		
		// antiviruses don't like endless loops, show them that this loop is a breaking point. (technically still an infinite loop :D)
		if (league_processes.size() > 0xff)
			break;

		for (auto& pid : league_processes) {
			if (!Injector::isInjected(pid)) {
				R3nzSkinInjector::cheatState = false;
				if (R3nzSkinInjector::btnState) {
					std::this_thread::sleep_for(1.5s);
					if (Injector::inject(pid))
						R3nzSkinInjector::cheatState = true;
					else
						R3nzSkinInjector::cheatState = false;
				}
				std::this_thread::sleep_for(1s);
			} else {
				R3nzSkinInjector::cheatState = true;
			}
		}
		std::this_thread::sleep_for(1s);
	}
}
