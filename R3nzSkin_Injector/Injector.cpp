#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <psapi.h>
#include <string>
#include <thread>
#include <tlhelp32.h>

#include "Injector.hpp"
#include "R3nzUI.hpp"

proclist_t WINAPI Injector::findProcesses(const std::wstring name) noexcept
{
	auto process_snap{ ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
	proclist_t list;

	if (process_snap == INVALID_HANDLE_VALUE)
		return list;

	PROCESSENTRY32W pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (::Process32First(process_snap, &pe32)) {
		if (pe32.szExeFile == name)
			list.push_back(pe32.th32ProcessID);

		while (::Process32Next(process_snap, &pe32)) {
			if (pe32.szExeFile == name)
				list.push_back(pe32.th32ProcessID);
		}
	}

	::CloseHandle(process_snap);
	return list;
}

bool WINAPI Injector::isInjected(const std::uint32_t pid) noexcept
{
	auto hProcess{ ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid) };

	if (NULL == hProcess)
		return false;

	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (::K32EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (auto i{ 0u }; i < (cbNeeded / sizeof(HMODULE)); ++i) {
			TCHAR szModName[MAX_PATH];
			if (::K32GetModuleBaseNameW(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				if (::wcscmp(szModName, L"R3nzSkin.dll") == 0) {
					::CloseHandle(hProcess);
					return true;
				}
			}
		}
	}
	::CloseHandle(hProcess);
	return false;
}

bool WINAPI Injector::inject(const std::uint32_t pid) noexcept
{
	NtCreateThreadExBuffer ntbuffer;

	::memset(&ntbuffer, 0, sizeof(NtCreateThreadExBuffer));
	DWORD temp1{ 0 };
	DWORD temp2{ 0 };

	ntbuffer.Size = sizeof(NtCreateThreadExBuffer);
	ntbuffer.Unknown1 = 0x10003;
	ntbuffer.Unknown2 = 0x8;
	ntbuffer.Unknown3 = static_cast<PULONG>(&temp1);
	ntbuffer.Unknown4 = 0;
	ntbuffer.Unknown5 = 0x10004;
	ntbuffer.Unknown6 = 4;
	ntbuffer.Unknown7 = static_cast<PULONG>(&temp2);
	ntbuffer.Unknown8 = 0;

	TCHAR current_dir[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, current_dir);
	const auto handle{ ::OpenProcess(PROCESS_ALL_ACCESS, false, pid) };

	if (!handle || handle == INVALID_HANDLE_VALUE)
		return false;

	FILETIME ft;
	SYSTEMTIME st;
	::GetSystemTime(&st);
	::SystemTimeToFileTime(&st, &ft);
	FILETIME create, exit, kernel, user;
	::GetProcessTimes(handle, &create, &exit, &kernel, &user);

	const auto delta{ 10 - static_cast<std::int32_t>((*reinterpret_cast<std::uint64_t*>(&ft) - *reinterpret_cast<std::uint64_t*>(&create.dwLowDateTime)) / 10000000U) };

	if (delta > 0)
		std::this_thread::sleep_for(std::chrono::seconds(delta));

	const auto dll_path{ std::wstring(current_dir) + L"\\R3nzSkin.dll" };

	if (auto f{ std::ifstream(dll_path) }; !f.is_open()) {
		MessageBox(nullptr, L"R3nzSkin.dll file could not be found.\nTry reinstalling the cheat.", L"R3nzSkin", MB_ICONERROR | MB_OK);
		::CloseHandle(handle);
		return false;
	}

	const auto dll_path_remote{ ::VirtualAllocEx(handle, nullptr, (dll_path.size() + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) };

	if (!dll_path_remote) {
		::CloseHandle(handle);
		return false;
	}

	if (!::WriteProcessMemory(handle, dll_path_remote, dll_path.data(), (dll_path.size() + 1) * sizeof(wchar_t), nullptr)) {
		::VirtualFreeEx(handle, dll_path_remote, 0u, MEM_RELEASE);
		::CloseHandle(handle);
		return false;
	}

	HANDLE thread;
	NtCreateThreadEx(&thread, GENERIC_ALL, NULL, handle, reinterpret_cast<LPTHREAD_START_ROUTINE>(::GetProcAddress(::GetModuleHandle(L"kernel32.dll"), "LoadLibraryW")), dll_path_remote, FALSE, NULL, NULL, NULL, &ntbuffer);

	if (!thread || thread == INVALID_HANDLE_VALUE) {
		::VirtualFreeEx(handle, dll_path_remote, 0u, MEM_RELEASE);
		::CloseHandle(handle);
		return false;
	}

	::WaitForSingleObject(thread, INFINITE);
	::CloseHandle(thread);
	::VirtualFreeEx(handle, dll_path_remote, 0u, MEM_RELEASE);
	::CloseHandle(handle);
	return true;
}

void WINAPI Injector::enableDebugPrivilege() noexcept
{
	HANDLE token;

	if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
		LUID value;
		if (::LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &value)) {
			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = value;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (::AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), NULL, NULL))
				::CloseHandle(token);
		}
	}
}

std::string Injector::randomString(std::uint32_t size) noexcept
{
	static const char alphanum[]{ "_0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };
	std::string tmp_s;
	tmp_s.reserve(size);

	for (auto i{ 0u }; i < size; ++i)
		tmp_s += alphanum[std::rand() % (sizeof(alphanum) - 1)];

	return tmp_s;
}

void Injector::renameExe() noexcept
{
	char szExeFileName[MAX_PATH];
	GetModuleFileNameA(nullptr, szExeFileName, MAX_PATH);

	const auto path{ std::string(szExeFileName) };
	const auto exe{ path.substr(path.find_last_of("\\") + 1, path.size()) };
	const auto newName{ randomString(std::rand() % (10 - 7 + 1) + 7) + ".exe" };

	rename(exe.c_str(), newName.c_str());
}

void Injector::run() noexcept
{
	enableDebugPrivilege();

	while (true) {
		const auto& league_client_processes{ Injector::findProcesses(L"LeagueClient.exe") };
		const auto& league_processes{ Injector::findProcesses(L"League of Legends.exe") };

		R3nzSkinInjector::gameState = (league_processes.size() > 0) ? true : false;
		R3nzSkinInjector::clientState = (league_client_processes.size() > 0) ? true : false;

		for (auto& pid : league_processes) {
			if (!Injector::isInjected(pid)) {
				R3nzSkinInjector::cheatState = false;
				if (R3nzSkinInjector::btnState) {
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
