#include <Windows.h>
#include <chrono>
#include <cstdint>
#include <thread>

#include "Config.hpp"
#include "GUI.hpp"
#include "Hooks.hpp"
#include "Memory.hpp"

#include "SDK/GameState.hpp"

void WINAPI DllAttach(HMODULE hModule) noexcept
{
	using namespace std::chrono_literals;

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

	::FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	if (hModule) {
		::DisableThreadLibraryCalls(hModule);
	} else {
		::MessageBoxA(nullptr, "Not found hModule", "R3nzSkin", MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	if (reason == DLL_PROCESS_ATTACH) {
		if (const auto hThread{ ::CreateThread(nullptr, 0u, reinterpret_cast<LPTHREAD_START_ROUTINE>(DllAttach), hModule, 0ul, nullptr) }; hThread)
			::CloseHandle(hThread);

		return TRUE;
	}

	return FALSE;
}
