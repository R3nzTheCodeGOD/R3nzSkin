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

bool NTAPI HideThread(HANDLE hThread) noexcept
{
	__try {
		using FnSetInformationThread = NTSTATUS(NTAPI*)(HANDLE, UINT, PVOID, ULONG);
		const auto NtSetInformationThread{ reinterpret_cast<FnSetInformationThread>(::GetProcAddress(::GetModuleHandle(L"ntdll.dll"), "NtSetInformationThread")) };

		if (NtSetInformationThread == NULL)
			return false;

		if (const auto status{ NtSetInformationThread(hThread, 0x11u, NULL, 0ul) }; status == 0x00000000)
			return true;
	} __except (TRUE) {
		return false;
	}
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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	HideThread(hModule);

	if (reason == DLL_PROCESS_ATTACH) {
		std::setlocale(LC_ALL, ".utf8");
		::_beginthreadex(nullptr, 0u, reinterpret_cast<_beginthreadex_proc_type>(DllAttach), nullptr, 0u, nullptr);
		::CloseHandle(hModule);
		return TRUE;
	}

	return FALSE;
}
