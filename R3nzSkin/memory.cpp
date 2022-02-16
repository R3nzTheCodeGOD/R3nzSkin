#include <Windows.h>
#include <chrono>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include "Memory.hpp"
#include "Offsets.hpp"


std::uint8_t* find_signature(const wchar_t* szModule, const char* szSignature) noexcept
{
	try {
		const auto module{ ::GetModuleHandleW(szModule) };
		static auto pattern_to_byte = [](const char* pattern) noexcept {
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

class offset_signature {
public:
	std::vector<std::string> pattern;
	bool sub_base;
	bool read;
	std::int32_t additional;
	std::uint32_t* offset;
};

std::vector<offset_signature> gameClientSig{
	{
		{
			"A1 ? ? ? ? 56 83 78 08 00",
			"A1 ? ? ? ? 68 ? ? ? ? 8B 70 08",
			"A1 ? ? ? ? 83 78 08 02 0F 85 ? ? ? ?"
		},
		true, true, 0,
		&offsets::global::GameClient
	}
};

std::vector<offset_signature> sigs{ 
	{
		{
			"8B 3D ? ? ? ? 3B F7 75 09",
			"A1 ? ? ? ? 8B 54 24 28 85 C0",
			"8B 0D ? ? ? ? 85 C9 0F 84 ? ? ? ? 83 7E 10 00"
		},
		true, true, 0,
		&offsets::global::Player
	},
	{
		{
			"A1 ? ? ? ? 8B 54 24 14 53",
			"8B 0D ? ? ? ? 50 8D 44 24 18 C7 44 24 ? ? ? ? ?",
			"8B 0D ? ? ? ? 6A 00 E8 ? ? ? ? 8B CE"
		},
		true, true, 0,
		&offsets::global::ManagerTemplate_AIHero_
	},
	{
		{
			"89 1D ? ? ? ? 57 8D 4B 08",
			"8B 3D ? ? ? ? 85 FF 74 2B 8B 4F 1C"
		},
		true, true, 0,
		&offsets::global::ChampionManager
	},
	{
		{
			"A1 ? ? ? ? 53 55 8B 6C 24 1C",
			"8B 35 ? ? ? ? 8B 56 04",
			"8B 35 ? ? ? ? 8B 49 08 E8 ? ? ? ? F3 0F 10 0D ? ? ? ?"
		},
		true, true, 0,
		&offsets::global::ManagerTemplate_AIMinionClient_ 
	},
	{
		{
			"3B 05 ? ? ? ? 75 72",
			"A3 ? ? ? ? 6A 64 6A 00",
			"FF 35 ? ? ? ? E8 ? ? ? ? 83 C4 0C 80 7E 2C 00"
		},
		true, true, 0,
		&offsets::global::Riot__g_window
	},
	{
		{
			"8D 8E ? ? ? ? FF 74 24 4C"
		},
		false, true, 0,
		&offsets::AIBaseCommon::CharacterDataStack
	},
	{
		{
			"80 BE ? ? ? ? ? 75 4D 0F 31"
		},
		false, true, 0,
		&offsets::AIBaseCommon::SkinId
	},
	{
		{
			"8B B7 ? ? ? ? FF 70 28",
			"8B 8F ? ? ? ? 88 A7 ? ? ? ?",
			"8B 8E ? ? ? ? D1 E8",
			"8B 86 ? ? ? ? 89 4C 24 08",
			"8B 8E ? ? ? ? C1 E8 02",
		},
		false, true, 0,
		&offsets::MaterialRegistry::D3DDevice
	},
#ifdef _RIOT
	{
		{
			"8B 95 ? ? ? ? 0F 10 44 24 ?",
			"8D 9F ? ? ? ? 0F 84 ? ? ? ?",
			"8B 8E ? ? ? ? 52 57"
		},
		false, true, 0,
		&offsets::MaterialRegistry::SwapChain
	},
#endif
	{
		{
			"83 EC 50 53 55 56 57 8B F9 8B 47 04"
		},
		true, false, 0,
		&offsets::functions::CharacterDataStack__Push },
	{
		{
			"E8 ? ? ? ? 5F 5E 5B 83 C4 44",
			"E8 ? ? ? ? 8B 4E 7C 5E"
		},
		true, false, 0,
		&offsets::functions::CharacterDataStack__Update
	},
	{
		{
			"E8 ? ? ? ? FF 75 38",
			"E8 ? ? ? ? FF 73 58",
		},
		true, false, 0,
		&offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr
	},
	{
		{
			"E8 ? ? ? ? FF 75 FC 50",
			"E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 04 8B F0 6A 0B",
			"E8 ? ? ? ? 83 C4 04 8D 4E 0C 8B D0"
		},
		true, false, 0,
		&offsets::functions::translateString_UNSAFE_DONOTUSE
	},
	{
		{
			"E8 ? ? ? ? 39 44 24 1C 5F",
			"E8 ? ? ? ? 85 C0 74 3A"
		},
		true, false, 0,
		&offsets::functions::GetGoldRedirectTarget
	}
};

void Memory::Search(bool gameClient) noexcept
{
	using namespace std::chrono_literals;

	try {
		const auto base{ Memory::getLeagueModule() };
		const auto& signatureToSearch{ (gameClient ? gameClientSig : sigs) };

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
	} catch (const std::exception& e) {
		::MessageBoxA(nullptr, e.what(), "R3nzSkin", MB_OK | MB_ICONWARNING);
	}
}
