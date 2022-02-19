#pragma warning(disable: 28182)

#include <Windows.h>
#include <chrono>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include "Memory.hpp"
#include "Offsets.hpp"


[[nodiscard]] static std::uint8_t* find_signature(const wchar_t* szModule, const char* szSignature) noexcept
{
	try {
		const auto module{ ::GetModuleHandle(szModule) };
		static const auto pattern_to_byte = [](const char* pattern) noexcept {
			auto bytes{ std::vector<std::int32_t>{} };
			const auto start{ const_cast<char*>(pattern) };
			const auto end{ const_cast<char*>(pattern) + ::strlen(pattern) };

			for (auto* current{ start }; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				} else
					bytes.push_back(::strtoul(current, &current, 16));
			}
			return bytes;
		};

		const auto dosHeader{ reinterpret_cast<PIMAGE_DOS_HEADER>(module) };
		if (dosHeader == NULL)
			return nullptr;

		const auto ntHeaders{ reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module + dosHeader->e_lfanew)) };
		const auto textSection{ IMAGE_FIRST_SECTION(ntHeaders) };
		const auto sizeOfImage{ textSection->SizeOfRawData };
		const auto patternBytes{ pattern_to_byte(szSignature) };
		const auto scanBytes{ reinterpret_cast<std::uint8_t*>(module) + textSection->VirtualAddress };
		const auto s{ patternBytes.size() };
		const auto d{ patternBytes.data() };
		auto mbi{ MEMORY_BASIC_INFORMATION{ 0 } };
		std::uint8_t* next_check_address{ 0 };

		for (auto i{ 0ul }; i < sizeOfImage - s; ++i) {
			bool found{ true };
			for (auto j{ 0ul }; j < s; ++j) {
				const auto current_address{ scanBytes + i + j };
				if (current_address >= next_check_address) {
					if (!::VirtualQuery(reinterpret_cast<void*>(current_address), &mbi, sizeof(mbi)))
						break;

					if (mbi.Protect == PAGE_NOACCESS || mbi.State != MEM_COMMIT) {
						i += ((reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize) - (reinterpret_cast<std::uintptr_t>(scanBytes) + i));
						i--;
						found = false;
						break;
					} else
						next_check_address = reinterpret_cast<std::uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
				}

				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found)
				return &scanBytes[i];
		}
		return nullptr;
	} catch (const std::exception&) {
		return nullptr;
	}
}

void Memory::update(bool gameClient) noexcept
{
	if (gameClient)
		this->client = *reinterpret_cast<GameClient**>(this->getLeagueModule() + offsets::global::GameClient);
	else {
		this->localPlayer = *reinterpret_cast<AIBaseCommon**>(this->getLeagueModule() + offsets::global::Player);
		this->heroList = *reinterpret_cast<ManagerTemplate<AIHero>**>(this->getLeagueModule() + offsets::global::ManagerTemplate_AIHero_);
		this->minionList = *reinterpret_cast<ManagerTemplate<AIMinionClient>**>(this->getLeagueModule() + offsets::global::ManagerTemplate_AIMinionClient_);
		this->championManager = *reinterpret_cast<ChampionManager**>(this->getLeagueModule() + offsets::global::ChampionManager);
		this->translateString = reinterpret_cast<const char* (__cdecl*)(const char*)>(this->getLeagueModule() + offsets::functions::translateString_UNSAFE_DONOTUSE);
		this->materialRegistry = reinterpret_cast<std::uintptr_t(__stdcall*)()>(this->getLeagueModule() + offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr)();
		this->d3dDevice = *reinterpret_cast<IDirect3DDevice9**>(this->materialRegistry + offsets::MaterialRegistry::D3DDevice);
#ifdef _RIOT
		this->swapChain = *reinterpret_cast<IDXGISwapChain**>(this->materialRegistry + offsets::MaterialRegistry::SwapChain);
#endif
	}
}

void Memory::Search(bool gameClient) noexcept
{
	using namespace std::chrono_literals;

	try {
		const auto base{ this->getLeagueModule() };
		const auto& signatureToSearch{ (gameClient ? this->gameClientSig : this->sigs) };

		for (const auto& sig : signatureToSearch)
			*sig.offset = 0;

		while (true) {
			bool missing_offset{ false };
			for (auto& sig : signatureToSearch) {

				if (*sig.offset != 0)
					continue;

				for (auto& pattern : sig.pattern) {
					auto address{ find_signature(nullptr, pattern.c_str()) };

					if (!address) {
						::MessageBoxA(nullptr, ("Failed to find pattern: " + pattern).c_str(), "R3nzSkin", MB_OK | MB_ICONWARNING);
						continue;
					}

					if (sig.read)
						address = *reinterpret_cast<uint8_t**>(address + (pattern.find_first_of("?") / 3));
					else if (address[0] == 0xE8)
						address = address + *reinterpret_cast<uint32_t*>(address + 1) + 5;

					if (sig.sub_base)
						address -= base;

					address += sig.additional;

					*sig.offset = reinterpret_cast<uint32_t>(address);
					break;
				}

				if (!*sig.offset) {
					missing_offset = true;
					break;
				}
			}

			if (!missing_offset)
				break;

			std::this_thread::sleep_for(2s);
		}
		this->update(gameClient);
	} catch (const std::exception& e) {
		::MessageBoxA(nullptr, e.what(), "R3nzSkin", MB_OK | MB_ICONWARNING);
	}
}
