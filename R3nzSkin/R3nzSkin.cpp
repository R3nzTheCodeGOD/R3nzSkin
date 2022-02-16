#pragma warning(disable : 6387)
#pragma warning(disable : 4715)

#include <Windows.h>
#include <chrono>
#include <cstdint>
#include <thread>

#include "Config.hpp"
#include "GUI.hpp"
#include "Hooks.hpp"
#include "Memory.hpp"

#include "SDK/GameState.hpp"

bool NTAPI HideThread(HANDLE hThread) noexcept
{
	__try {
		static const auto NtSetInformationThread{ reinterpret_cast<NTSTATUS(NTAPI*)(HANDLE, UINT, PVOID, ULONG)>(GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtSetInformationThread")) };

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

	Memory::Search(true);
	auto client{ Memory::getClient() };

	while (true) {
		std::this_thread::sleep_for(1s);

		if (!client) {
			Memory::Search(true);
			client = Memory::getClient();
		} else {
			if (client->game_state == GGameState_s::Running)
				break;
		}
	}

	GUI::is_open = true;
	std::this_thread::sleep_for(500ms);
	Memory::Search(false);
	std::this_thread::sleep_for(500ms);
	Config::load();
	Hooks::install();
		
	run = true;
	while (run)
		std::this_thread::sleep_for(250ms);

	::ExitProcess(0u);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	HideThread(hModule);

	if (reason == DLL_PROCESS_ATTACH) {
		::_beginthreadex(nullptr, 0u, reinterpret_cast<_beginthreadex_proc_type>(DllAttach), nullptr, 0u, nullptr);
		::CloseHandle(hModule);
		return TRUE;
	}

	return FALSE;
}
