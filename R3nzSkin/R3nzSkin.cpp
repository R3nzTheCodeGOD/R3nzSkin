#include <Windows.h>
#include <clocale>
#include <cstdint>

#include "Config.hpp"
#include "GUI.hpp"
#include "GameClasses.hpp"
#include "Hooks.hpp"
#include "Memory.hpp"

void WINAPI DllAttach(HMODULE hModule) noexcept
{
	Memory::Search(true);
	auto client{ Memory::getClient() };

	while (true) {
		if (!client)
			client = Memory::getClient();
		else if (client->game_state == GGameState_s::Running)
			break;

		std::this_thread::sleep_for(500ms);
	}

	GUI::is_open = true;
	std::this_thread::sleep_for(100ms);
	Memory::Search(false);
	std::this_thread::sleep_for(100ms);
	Config::load();
	Hooks::install();
		
	run = true;
	while (run)
		std::this_thread::sleep_for(200ms);

	::FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	if (hModule) {
		std::setlocale(LC_CTYPE, ".utf8");
		::DisableThreadLibraryCalls(hModule);
	} else {
		::MessageBoxA(nullptr, "Not found hModule", "R3nzSkin", MB_OK | MB_ICONWARNING);
		return 0;
	}

	if (reason == DLL_PROCESS_ATTACH) {
		if (const auto hThread{ ::CreateThread(nullptr, 0u, reinterpret_cast<LPTHREAD_START_ROUTINE>(DllAttach), hModule, 0ul, nullptr) }; hThread)
			::CloseHandle(hThread);

		return 1;
	}

	return 0;
}
