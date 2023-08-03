#pragma warning(disable : 6011)

#include <Windows.h>
#include <ShlObj.h>
#include <cinttypes>
#include <filesystem>
#include <string>

#include "fnv_hash.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "vmt_smart_hook.hpp"

#include "CheatManager.hpp"
#include "Hooks.hpp"
#include "Memory.hpp"
#include "SDK/AIBaseCommon.hpp"
#include "SDK/GameState.hpp"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static inline void testFunc() noexcept
{
	// The codes you write here are executed when you press the F7 key in the game.
	
	// Example Func
	const auto minions{ cheatManager.memory->minionList };
	for (auto i{ 0u }; i < minions->length; ++i) {
		const auto minion{ minions->list[i] };
		const auto owner{ minion->getGoldRedirectTarget() };
		cheatManager.logger->addLog("Minion: %s\n\tModelName: %s\n\t", minion->get_name()->c_str(), minion->get_character_data_stack()->base_skin.model.str);
		if (owner)
			cheatManager.logger->addLog("OwnerName: %s\n\t\tModelName: %s\n\t", owner->get_name()->c_str(), owner->get_character_data_stack()->base_skin.model.str);
		cheatManager.logger->addLog("IsLaneMinion: %d\n\t", minion->isLaneMinion());
		cheatManager.logger->addLog("IsEliteMinion: %d\n\t", minion->isEliteMinion());
		cheatManager.logger->addLog("IsEpicMinion: %d\n\t", minion->isEpicMinion());
		cheatManager.logger->addLog("IsMinion: %d\n\t", minion->isMinion());
		cheatManager.logger->addLog("IsJungle: %d\n\n", minion->isJungle());
	}
}

static LRESULT WINAPI wndProc(const HWND window, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	if (msg == WM_KEYDOWN) {
		if (wParam == cheatManager.config->menuKey.getKey()) {
			cheatManager.gui->is_open = !cheatManager.gui->is_open;
			if (!cheatManager.gui->is_open)
				cheatManager.config->save();
		} else if (wParam == 0x35) {
			const auto player{ cheatManager.memory->localPlayer };
			if (const auto player{ cheatManager.memory->localPlayer }; (::GetAsyncKeyState(VK_LCONTROL) & 0x8000) && player) {
				const auto playerHash{ fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str) };
				if (const auto it{ std::ranges::find_if(cheatManager.database->specialSkins,
				    [&skin = player->get_character_data_stack()->base_skin.skin, &ph = playerHash](const SkinDatabase::specialSkin& x) noexcept -> bool
				    {
						return x.champHash == ph && (x.skinIdStart <= skin && x.skinIdEnd >= skin);
				    })}; it != cheatManager.database->specialSkins.end())
				{
					const auto stack{ player->get_character_data_stack() };
					if (stack->base_skin.gear < static_cast<std::int8_t>(it->gears.size()) - 1)
						++stack->base_skin.gear;
					else
						stack->base_skin.gear = static_cast<std::int8_t>(0);

					stack->update(true);
				}
			}
		} else if (wParam == cheatManager.config->nextSkinKey.getKey() && cheatManager.config->quickSkinChange) {
			if (const auto player{ cheatManager.memory->localPlayer }; player) {
				const auto& values{ cheatManager.database->champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };
				if (++cheatManager.config->current_combo_skin_index > static_cast<std::int32_t>(values.size()))
					cheatManager.config->current_combo_skin_index = static_cast<std::int32_t>(values.size());
				if (cheatManager.config->current_combo_skin_index > 0)
					player->change_skin(values[cheatManager.config->current_combo_skin_index - 1].model_name, values[cheatManager.config->current_combo_skin_index - 1].skin_id);
				cheatManager.config->save();
			}
		} else if (wParam == cheatManager.config->previousSkinKey.getKey() && cheatManager.config->quickSkinChange) {
			if (const auto player{ cheatManager.memory->localPlayer }; player) {
				const auto& values{ cheatManager.database->champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };
				if (--cheatManager.config->current_combo_skin_index > 0)
					player->change_skin(values[cheatManager.config->current_combo_skin_index - 1].model_name, values[cheatManager.config->current_combo_skin_index - 1].skin_id);
				else
					cheatManager.config->current_combo_skin_index = 1;
				cheatManager.config->save();
			}
		} else if (wParam == VK_F7) {
			testFunc();
		}
	}

	return ::CallWindowProc(originalWndProc, window, msg, wParam, lParam);
}

std::once_flag init_device;
std::unique_ptr<::vmt_smart_hook> d3d_device_vmt{ nullptr };
std::unique_ptr<::vmt_smart_hook> swap_chain_vmt{ nullptr };

static const ImWchar tahomaRanges[] = {
	0x0020, 0x00FF, // Basic Latin + Latin Supplement
	0x0100, 0x024F, // Latin Extended-A + Latin Extended-B
	0x0250, 0x02FF, // IPA Extensions + Spacing Modifier Letters
	0x0300, 0x03FF, // Combining Diacritical Marks + Greek/Coptic
	0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
	0x0530, 0x06FF, // Armenian + Hebrew + Arabic
	0x0E00, 0x0E7F, // Thai
	0x1E00, 0x1FFF, // Latin Extended Additional + Greek Extended
	0x2000, 0x20CF, // General Punctuation + Superscripts and Subscripts + Currency Symbols
	0x2100, 0x218F, // Letterlike Symbols + Number Forms
	0,
};

namespace d3d_vtable {
	ID3D11Device* d3d11_device{ nullptr };
	ID3D11DeviceContext* d3d11_device_context{ nullptr };
	ID3D11RenderTargetView* main_render_target_view{ nullptr };
	IDXGISwapChain* p_swap_chain{ nullptr };

	static void WINAPI create_render_target() noexcept
	{
		ID3D11Texture2D* back_buffer{ nullptr };
		p_swap_chain->GetBuffer(0u, IID_PPV_ARGS(&back_buffer));

		if (back_buffer) {
			d3d11_device->CreateRenderTargetView(back_buffer, nullptr, &main_render_target_view);
			back_buffer->Release();
		}
	}

	static void init_imgui(void* device, bool is_d3d11 = false) noexcept
	{
		cheatManager.database->load();
		cheatManager.logger->addLog("All skins loaded from memory!\n");
		ImGui::CreateContext();
		auto& style{ ImGui::GetStyle() };

		style.WindowPadding = ImVec2(6.0f, 6.0f);
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(6.0f, 4.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style.ScrollbarSize = 12.0f;

		style.WindowBorderSize = 0.5f;
		style.ChildBorderSize = 0.5f;
		style.PopupBorderSize = 0.5f;
		style.FrameBorderSize = 0;

		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;
		style.PopupRounding = 0.0f;

		style.AntiAliasedFill = true;
		style.AntiAliasedLines = true;

		const auto colors{ style.Colors };

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		auto& io{ ImGui::GetIO() }; (void)io;
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		if (PWSTR pathToFonts; SUCCEEDED(::SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &pathToFonts))) {
			const std::filesystem::path path{ pathToFonts };
			::CoTaskMemFree(pathToFonts);
			ImFontConfig cfg;
			cfg.SizePixels = 15.0f;
			io.Fonts->AddFontFromFileTTF((path / "tahoma.ttf").string().c_str(), cfg.SizePixels, &cfg, tahomaRanges);
			cfg.MergeMode = true;
			io.Fonts->AddFontFromFileTTF((path / "malgun.ttf").string().c_str(), cfg.SizePixels, &cfg, io.Fonts->GetGlyphRangesKorean());
			io.Fonts->AddFontFromFileTTF((path / "msyh.ttc").string().c_str(), cfg.SizePixels, &cfg, io.Fonts->GetGlyphRangesChineseFull());
			cfg.MergeMode = false;
			cheatManager.logger->addLog("Fonts loaded!\n");
		}

		ImGui_ImplWin32_Init(cheatManager.memory->window);

		if (is_d3d11) {
			p_swap_chain = static_cast<IDXGISwapChain*>(device);
			p_swap_chain->GetDevice(__uuidof(d3d11_device), reinterpret_cast<void**>(&(d3d11_device)));
			d3d11_device->GetImmediateContext(&d3d11_device_context);
			create_render_target();
			::ImGui_ImplDX11_Init(d3d11_device, d3d11_device_context);
			::ImGui_ImplDX11_CreateDeviceObjects();
		} else
			::ImGui_ImplDX9_Init(static_cast<IDirect3DDevice9*>(device));

		originalWndProc = WNDPROC(::SetWindowLongPtr(cheatManager.memory->window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&wndProc)));
		cheatManager.logger->addLog("WndProc hooked!\n\tOriginal: 0x%X\n\tNew: 0x%X\n", &originalWndProc, &wndProc);
	}

	static void render(void* device, bool is_d3d11 = false) noexcept
	{
		const auto client{ cheatManager.memory->client };
		if (client && client->game_state == GGameState_s::Running) {
			cheatManager.hooks->init();
			if (cheatManager.gui->is_open) {
				if (is_d3d11)
					::ImGui_ImplDX11_NewFrame();
				else
					::ImGui_ImplDX9_NewFrame();
				::ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				cheatManager.gui->render();
				ImGui::EndFrame();
				ImGui::Render();

				if (is_d3d11) {
					d3d11_device_context->OMSetRenderTargets(1, &main_render_target_view, nullptr);
					::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				} else {
					unsigned long colorwrite, srgbwrite;
					const auto dvc{ static_cast<IDirect3DDevice9*>(device) };
					dvc->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
					dvc->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
					dvc->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
					dvc->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
					::ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
					dvc->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
					dvc->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
				}
			}
		}
	}

	struct dxgi_present {
		static long WINAPI hooked(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags) noexcept
		{
			std::call_once(init_device, [&]() { init_imgui(p_swap_chain, true); });
			render(p_swap_chain, true);
			return m_original(p_swap_chain, sync_interval, flags);
		}
		static decltype(&hooked) m_original;
	};
	decltype(dxgi_present::m_original) dxgi_present::m_original;

	struct dxgi_resize_buffers {
		static long WINAPI hooked(IDXGISwapChain* p_swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) noexcept
		{
			if (main_render_target_view) { main_render_target_view->Release(); main_render_target_view = nullptr; }
			const auto hr{ m_original(p_swap_chain, buffer_count, width, height, new_format, swap_chain_flags) };
			create_render_target();
			return hr;
		}
		static decltype(&hooked) m_original;
	};
	decltype(dxgi_resize_buffers::m_original) dxgi_resize_buffers::m_original;

	struct end_scene {
		static long WINAPI hooked(IDirect3DDevice9* p_device) noexcept
		{
			std::call_once(init_device, [&]() { init_imgui(p_device); });
			render(p_device);
			return m_original(p_device);
		}
		static decltype(&hooked) m_original;
	};
	decltype(end_scene::m_original) end_scene::m_original;

	struct reset {
		static long WINAPI hooked(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parametrs) noexcept
		{
			::ImGui_ImplDX9_InvalidateDeviceObjects();
			const auto hr{ m_original(device, parametrs) };
			if (hr >= 0)
				::ImGui_ImplDX9_CreateDeviceObjects();
			return hr;
		}
		static decltype(&hooked) m_original;
	};
	decltype(reset::m_original) reset::m_original;
};

static void changeModelForObject(const AIBaseCommon* obj, const char* model, const std::int32_t skin) noexcept
{
	if (skin == -1)
		return;

	if (const auto stack{ obj->get_character_data_stack() }; stack->base_skin.skin != skin) {
		stack->base_skin.skin = skin;
		stack->stack.clear();
		stack->push(model, skin);
	}
}

static void changeSkinForObject(const AIBaseCommon* obj, const std::int32_t skin) noexcept
{
	if (skin == -1)
		return;

	if (const auto stack{ obj->get_character_data_stack() }; stack->base_skin.skin != skin) {
		stack->base_skin.skin = skin;
		stack->update(true);
	}
}

void Hooks::init() noexcept
{
	const auto player{ cheatManager.memory->localPlayer };
	const auto heroes{ cheatManager.memory->heroList };
	const auto minions{ cheatManager.memory->minionList };
	static const auto playerHash{ player ? fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str) : 0u };

	std::call_once(change_skins, [&]() noexcept -> void {
		if (player) {
			if (cheatManager.config->current_combo_skin_index > 0) {
				const auto& values{ cheatManager.database->champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };
				player->change_skin(values[cheatManager.config->current_combo_skin_index - 1].model_name, values[cheatManager.config->current_combo_skin_index - 1].skin_id);
			}
		}

		const auto my_team{ player ? player->get_team() : 100 };
		for (auto i{ 0u }; i < heroes->length; ++i) {
			const auto hero{ heroes->list[i] };
			if (hero == player)
				continue;

			const auto champion_name_hash{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) };
			if (champion_name_hash == FNV("PracticeTool_TargetDummy"))
				continue;

			const auto is_enemy{ my_team != hero->get_team() };
			const auto& config_array{ is_enemy ? cheatManager.config->current_combo_enemy_skin_index : cheatManager.config->current_combo_ally_skin_index };
			const auto config_entry{ config_array.find(champion_name_hash) };
			if (config_entry == config_array.end())
				continue;

			if (config_entry->second > 0) {
				const auto& values = cheatManager.database->champions_skins[champion_name_hash];
				hero->change_skin(values[config_entry->second - 1].model_name, values[config_entry->second - 1].skin_id);
			}
		}
	});

	for (auto i{ 0u }; i < heroes->length; ++i) {
		if (const auto hero{ heroes->list[i] }; !hero->get_character_data_stack()->stack.empty()) {
			// Viego transforms into another champion as 2nd form, our own skin's id may not match for every champion. (same problem exists in sylas) 
			if (const auto championName{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) }; championName == FNV("Viego") || championName == FNV("Sylas"))
				continue;

			if (auto& stack{ hero->get_character_data_stack()->stack.front() }; stack.skin != hero->get_character_data_stack()->base_skin.skin) {
				stack.skin = hero->get_character_data_stack()->base_skin.skin;
				hero->get_character_data_stack()->update(true);
			}
		}
	}

	for (auto i{ 0u }; i < minions->length; ++i) {
		const auto minion{ minions->list[i] };

		if (minion->isLaneMinion()) {
			if (player && player->get_team() == 200)
				changeSkinForObject(minion, cheatManager.config->current_minion_skin_index * 2 + 1);
			else
				changeSkinForObject(minion, cheatManager.config->current_minion_skin_index * 2);
			continue;
		}

		const auto hash{ fnv::hash_runtime(minion->get_character_data_stack()->base_skin.model.str) };

		if (const auto owner{ minion->getGoldRedirectTarget() }; owner) {
			if (hash == FNV("JammerDevice") || hash == FNV("SightWard") || hash == FNV("YellowTrinket") || hash == FNV("VisionWard") || hash == FNV("BlueTrinket") || hash == FNV("TestCubeRender10Vision")) {
				if (!player || owner == player) {
					if (hash == FNV("TestCubeRender10Vision") && playerHash == FNV("Yone"))
						changeModelForObject(minion, "Yone", owner->get_character_data_stack()->base_skin.skin);
					else if (hash == FNV("TestCubeRender10Vision"))
						changeSkinForObject(minion, 0);
					else
						changeSkinForObject(minion, cheatManager.config->current_ward_skin_index);
				}
			} else if (hash != FNV("SRU_Jungle_Companions") && hash != FNV("DominationScout"))
				changeSkinForObject(minion, owner->get_character_data_stack()->base_skin.skin);
			continue;
		}

		if (const auto config_entry{ cheatManager.config->current_combo_jungle_mob_skin_index.find(hash) }; config_entry != cheatManager.config->current_combo_jungle_mob_skin_index.end() && config_entry->second != 0) {
			changeSkinForObject(minion, config_entry->second - 1);
			continue;
		}

		// Just LocalPlayer
		if ((hash == FNV("NunuSnowball") && playerHash == FNV("Nunu")) || (hash == FNV("KindredWolf") && playerHash == FNV("Kindred")) || (hash == FNV("QuinnValor") && playerHash == FNV("Quinn")))
			changeSkinForObject(minion, player->get_character_data_stack()->base_skin.skin);
	}
}

void Hooks::install() noexcept
{
	if (cheatManager.memory->d3dDevice) {
		d3d_device_vmt = std::make_unique<::vmt_smart_hook>(cheatManager.memory->d3dDevice);
		d3d_device_vmt->apply_hook<d3d_vtable::end_scene>(42);
		d3d_device_vmt->apply_hook<d3d_vtable::reset>(16);
		cheatManager.logger->addLog("DX9 Hooked!\n");
	} else if (cheatManager.memory->swapChain) {
		swap_chain_vmt = std::make_unique<::vmt_smart_hook>(cheatManager.memory->swapChain);
		swap_chain_vmt->apply_hook<d3d_vtable::dxgi_present>(8);
		swap_chain_vmt->apply_hook<d3d_vtable::dxgi_resize_buffers>(13);
		cheatManager.logger->addLog("DX11 Hooked!\n");
	}
}

void Hooks::uninstall() noexcept
{
	::SetWindowLongW(cheatManager.memory->window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWndProc));

	if (d3d_device_vmt)
		d3d_device_vmt->unhook();
	if (swap_chain_vmt)
		swap_chain_vmt->unhook();

	cheatManager.cheatState = false;
}
