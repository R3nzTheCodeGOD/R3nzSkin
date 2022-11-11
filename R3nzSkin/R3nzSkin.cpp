#pragma warning(disable : 6387)
#pragma warning(disable : 4715)

#include <Windows.h>
#include <clocale>
#include <chrono>
#include <cstdint>
#include <thread>

#include "CheatManager.hpp"

#include "Config.hpp"
#include "GUI.hpp"
#include "Hooks.hpp"
#include "Memory.hpp"

#include "SDK/GameState.hpp"

bool WINAPI HideThread(const HANDLE hThread) noexcept
{
	__try {
		using FnSetInformationThread = NTSTATUS(NTAPI*)(HANDLE ThreadHandle, UINT ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength);
		const auto NtSetInformationThread{ reinterpret_cast<FnSetInformationThread>(::GetProcAddress(::GetModuleHandle(L"ntdll.dll"), "NtSetInformationThread")) };

		if (!NtSetInformationThread)
			return false;

		if (const auto status{ NtSetInformationThread(hThread, 0x11u, nullptr, 0ul) }; status == 0x00000000)
			return true;
	} __except (TRUE) {
		return false;
	}
}

static void WINAPI StartThread(LPVOID thread, DWORD address) noexcept
{
	VirtualProtect(reinterpret_cast<LPVOID>(address), 0x1000, PAGE_EXECUTE_READWRITE, nullptr);

	CONTEXT ctx{};
	if (auto handle = _beginthreadex(nullptr, 0u, reinterpret_cast<_beginthreadex_proc_type>(address), nullptr, CREATE_SUSPENDED, nullptr); handle)
	{
		ctx.ContextFlags = CONTEXT_ALL;
		GetThreadContext(reinterpret_cast<HANDLE>(handle), &ctx);
		ctx.Eax = reinterpret_cast<DWORD>(thread);
		SetThreadContext(reinterpret_cast<HANDLE>(handle), &ctx);

		ResumeThread(reinterpret_cast<HANDLE>(handle));
		CloseHandle(reinterpret_cast<HANDLE>(handle));
	}
	return;
}

static void WINAPI DllAttach([[maybe_unused]] LPVOID lp) noexcept
{
	using namespace std::chrono_literals;

	HideThread(::GetCurrentThread());
	cheatManager.start();
	cheatManager.memory->Search(true);
	while (true) {
		std::this_thread::sleep_for(1s);
		
		if (!cheatManager.memory->client)
			cheatManager.memory->Search(true);
		else
			if (cheatManager.memory->client->game_state == GGameState_s::Running)
				break;
	}

	std::this_thread::sleep_for(500ms);
	cheatManager.memory->Search(false);
	std::this_thread::sleep_for(500ms);
	cheatManager.config->init();
	cheatManager.config->load();
	cheatManager.hooks->install();
		
	while (cheatManager.cheatState)
		std::this_thread::sleep_for(250ms);

	::ExitProcess(0u);
}

__declspec(safebuffers) BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD reason, [[maybe_unused]] LPVOID reserved)
{
	DisableThreadLibraryCalls(hModule);

	if (reason != DLL_PROCESS_ATTACH)
		return FALSE;

	HideThread(hModule);
	std::setlocale(LC_ALL, ".utf8");

	::StartThread(DllAttach, 0xFFFF920C205F6000);
	::CloseHandle(hModule);
	return TRUE;
}
