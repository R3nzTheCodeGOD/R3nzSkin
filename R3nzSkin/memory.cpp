#include <Windows.h>
#include <string>
#include <vector>

#include "GameClasses.hpp"
#include "Memory.hpp"
#include "Offsets.hpp"

#define SEE_ERROR 1

std::uint8_t* find_signature(const wchar_t* szModule, const char* szSignature) noexcept
{
	try {
		const auto module{ ::GetModuleHandleW(szModule) };
		static auto pattern_to_byte = [](const char* pattern) {
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

					if (mbi.Protect == PAGE_NOACCESS) {
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
	std::string pattern;
	bool sub_base;
	bool read;
	std::int32_t additional;
	std::uintptr_t* offset;
};

std::vector<offset_signature> gameClientSig{
	{
		"A1 ? ? ? ? 68 ? ? ? ? 8B 70 08", // mov eax, dword_
		true, true, 0,
		&offsets::global::GameClient
	}
};

std::vector<offset_signature> sigs{ 
	{
		"A1 ? ? ? ? 8B 54 24 28", // mov eax, dword_
		true, true, 0,
		&offsets::global::Player
	},
	{
		"8B 0D ? ? ? ? 50 8D 44 24 18 C7 44 24 ? ? ? ? ?", // mov ecx, dword_
		true, true, 0,
		&offsets::global::ManagerTemplate_AIHero_
	},
	{
		"89 1D ? ? ? ? 57 8D 4B 04", // mov dword_, ebx
		true, true, 0,
		&offsets::global::ChampionManager
	},
	{
		"A1 ? ? ? ? 53 55 8B 6C 24 1C", // mov eax, dword_
		true, true, 0,
		&offsets::global::ManagerTemplate_AIMinionClient_ 
	},
	{
		"3B 05 ? ? ? ? 75 72", // cmp eax, dword_
		true, true, 0,
		&offsets::global::Riot__g_window
	},
	{
		"8D 8E ? ? ? ? FF 74 24 4C", // lea ecx, [esi+2BD0h]
		false, true, 0,
		&offsets::AIBaseCommon::CharacterDataStack
	},
	{
		"80 BE ? ? ? ? ? 75 4D 0F 31", // cmp byte ptr [esi+0EFCh], 0
		false, true, 0,
		&offsets::AIBaseCommon::SkinId
	},
	{
		"8B 86 ? ? ? ? 89 4C 24 08", // mov eax, [esi+208h]
		false, true, 0,
		&offsets::MaterialRegistry::D3DDevice
	},
	{
		"8B 8E ? ? ? ? 52 57", // mov ecx, [esi+10Ch]
		false, true, 0,
		&offsets::MaterialRegistry::SwapChain
	},
	{
		"83 EC 50 53 55 56 57 8B F9 8B 47 04", // sub esp, 50h
		true, false, 0,
		&offsets::functions::CharacterDataStack__Push },
	{
		"E8 ? ? ? ? 8B 4E 7C 5E", // sub esp, 1Ch
		true, false, 0,
		&offsets::functions::CharacterDataStack__Update
	},
	{
		"E8 ? ? ? ? FF 73 58", // mov eax, dword_
		true, false, 0,
		&offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr
	},
	{
		"E8 ? ? ? ? FF 75 FC 50", // mov ecx, [esp+arg_0]
		true, false, 0,
		&offsets::functions::translateString_UNSAFE_DONOTUSE
	},
	{
		"E8 ? ? ? ? 85 C0 74 40", // add ecx, 4F8h
		true, false, 0,
		&offsets::functions::GetGoldRedirectTarget
	}
};

void Memory::Search(bool gameClient) noexcept
{
	try {
		static const auto base{ Memory::getLeagueModule() };
		const auto& signatureToSearch{ (gameClient ? gameClientSig : sigs) };

		for (const auto& sig : signatureToSearch)
			*sig.offset = 0;

		for (const auto& sig : signatureToSearch) {
			if (*sig.offset != 0)
				continue;

			auto address{ find_signature(nullptr, sig.pattern.c_str()) };

			if (address == nullptr) {
#if SEE_ERROR
				::MessageBoxA(nullptr, sig.pattern.c_str(), "R3nzSkin", MB_OK | MB_ICONWARNING);
#endif
			} else {
				if (sig.read)
					address = *reinterpret_cast<std::uint8_t**>(address + (sig.pattern.find_first_of("?") / 3));
				else if (address[0] == 0xE8)
					address = address + *reinterpret_cast<std::uint32_t*>(address + 1) + 5;

				if (sig.sub_base)
					address -= base;
					
				address += sig.additional;

				*sig.offset = reinterpret_cast<std::uint32_t>(address);
				continue;
			}
		}

#if SEE_ERROR
	} catch (const std::exception& e) {
		::MessageBoxA(nullptr, e.what(), "R3nzSkin", MB_OK | MB_ICONWARNING);
#else
	} catch (const std::exception&) {
#endif
	}
}
