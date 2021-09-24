#include <Windows.h>
#include <ShlObj.h>
#include <cinttypes>
#include <d3d11.h>
#include <d3d9.h>
#include <filesystem>
#include <memory>
#include <string>

#include "fnv_hash.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_extend.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "vmt_smart_hook.hpp"

#include "Config.hpp"
#include "GUI.hpp"
#include "GameClasses.hpp"
#include "Hooks.hpp"
#include "Memory.hpp"
#include "Offsets.hpp"
#include "SkinDatabase.hpp"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	if (msg == WM_KEYDOWN && wParam == VK_INSERT) {
		GUI::is_open = !GUI::is_open;
		if (!GUI::is_open)
			Config::save();
	}
	return ::CallWindowProcW(originalWndProc, window, msg, wParam, lParam);
}

std::once_flag init_device;
std::unique_ptr<::vmt_smart_hook> d3d_device_vmt{ nullptr };
std::unique_ptr<::vmt_smart_hook> swap_chain_vmt{ nullptr };

static constexpr ImWchar ranges[]{
	0x0020, 0x00FF, // Basic Latin + Latin Supplement
	0x0400, 0x044F, // Cyrillic
	0x0100, 0x017F, // Latin Extended-A
	0x0180, 0x024F, // Latin Extended-B
	0x2000, 0x206F, // General Punctuation
	0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
	0x31F0, 0x31FF, // Katakana Phonetic Extensions
	0xFF00, 0xFFEF, // Half-width characters
	0x4e00, 0x9FAF, // CJK Ideograms
	0,
};

namespace d3d_vtable {
	ID3D11Device* d3d11_device{ nullptr };
	ID3D11DeviceContext* d3d11_device_context{ nullptr };
	ID3D11RenderTargetView* main_render_target_view{ nullptr };
	IDXGISwapChain* p_swap_chain{ nullptr };

	void __stdcall create_render_target() noexcept
	{
		ID3D11Texture2D* back_buffer;
		p_swap_chain->GetBuffer(0u, IID_PPV_ARGS(&back_buffer));
		d3d11_device->CreateRenderTargetView(back_buffer, NULL, &main_render_target_view);
		back_buffer->Release();
	}

	void init_imgui(void* device, bool is_d3d11 = false) noexcept
	{
		SkinDatabase::load();
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

		auto* colors{ style.Colors };

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

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		if (PWSTR pathToFonts; SUCCEEDED(::SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &pathToFonts))) {
			std::filesystem::path path{ pathToFonts };
			::CoTaskMemFree(pathToFonts);
			ImGui::GetIO().Fonts->AddFontFromFileTTF((path / "Arial.ttf").string().c_str(), 14.0f, 0, ranges);
		}

		ImGui_ImplWin32_Init(*reinterpret_cast<HWND*>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::global::Riot__g_window));

		if (is_d3d11) {
			p_swap_chain = reinterpret_cast<IDXGISwapChain*>(device);
			p_swap_chain->GetDevice(__uuidof(d3d11_device), reinterpret_cast<void**>(&(d3d11_device)));
			d3d11_device->GetImmediateContext(&d3d11_device_context);
			create_render_target();
			::ImGui_ImplDX11_Init(d3d11_device, d3d11_device_context);
			::ImGui_ImplDX11_CreateDeviceObjects();

		} else
			::ImGui_ImplDX9_Init(reinterpret_cast<IDirect3DDevice9*>(device));

		originalWndProc = WNDPROC(SetWindowLongPtr(*reinterpret_cast<HWND*>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::global::Riot__g_window), GWLP_WNDPROC, LONG_PTR(&wndProc)));
	}

	void render(void* device, bool is_d3d11 = false) noexcept
	{
		auto* client{ *reinterpret_cast<GameClient**>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::global::GameClient) };
		if (client && client->game_state == GGameState_s::Running) {
			Hooks::init();
			if (GUI::is_open) {
				if (is_d3d11)
					::ImGui_ImplDX11_NewFrame();
				else
					::ImGui_ImplDX9_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				GUI::render();
				ImGui::EndFrame();
				ImGui::Render();

				if (is_d3d11) {
					d3d11_device_context->OMSetRenderTargets(1, &main_render_target_view, NULL);
					::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				} else {
					unsigned long colorwrite, srgbwrite;
					auto* dvc{ reinterpret_cast<IDirect3DDevice9*>(device) };
					dvc->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
					dvc->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
					dvc->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
					dvc->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
					ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
					dvc->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
					dvc->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
				}
			}
		}
	}

	struct dxgi_present {
		static long __stdcall hooked(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags) noexcept
		{
			std::call_once(init_device, [&]() {
				init_imgui(p_swap_chain, true);
			});
			render(p_swap_chain, true);
			return m_original(p_swap_chain, sync_interval, flags);
		}
		static decltype(&hooked) m_original;
	};
	decltype(dxgi_present::m_original) dxgi_present::m_original;

	struct dxgi_resize_buffers {
		static long __stdcall hooked(IDXGISwapChain* p_swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) noexcept
		{
			if (main_render_target_view) { main_render_target_view->Release(); main_render_target_view = nullptr; }
			auto hr = m_original(p_swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
			create_render_target();
			return hr;
		}
		static decltype(&hooked) m_original;
	};
	decltype(dxgi_resize_buffers::m_original) dxgi_resize_buffers::m_original;

	struct end_scene {
		static long __stdcall hooked(IDirect3DDevice9* p_device) noexcept
		{
			std::call_once(init_device, [&]() {
				init_imgui(p_device);
			});
			render(p_device);
			return m_original(p_device);
		}
		static decltype(&hooked) m_original;
	};
	decltype(end_scene::m_original) end_scene::m_original;

	struct reset {
		static long __stdcall hooked(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parametrs) noexcept
		{
			::ImGui_ImplDX9_InvalidateDeviceObjects();
			auto hr = m_original(device, parametrs);
			if (hr >= 0)
				::ImGui_ImplDX9_CreateDeviceObjects();
			return hr;
		}
		static decltype(&hooked) m_original;
	};
	decltype(reset::m_original) reset::m_original;
};

void Hooks::init() noexcept
{
	auto league_module{ std::uintptr_t(::GetModuleHandleA(nullptr)) };
	auto* player{ *reinterpret_cast<AIBaseCommon**>(league_module + offsets::global::Player) };
	auto* heroes{ *reinterpret_cast<ManagerTemplate<AIHero>**>(league_module + offsets::global::ManagerTemplate_AIHero_) };
	auto* minions{ *reinterpret_cast<ManagerTemplate<AIMinionClient>**>(league_module + offsets::global::ManagerTemplate_AIMinionClient_) };

	std::call_once(change_skins, [&]() {
		if (player) {
			if (Config::config.current_combo_skin_index > 0) {
				const auto& values{ SkinDatabase::champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };
				player->change_skin(values[Config::config.current_combo_skin_index - 1].model_name.c_str(), values[Config::config.current_combo_skin_index - 1].skin_id);
			}
		}

		auto my_team{ player ? player->get_team() : 100 };
		for (auto i{ 0u }; i < heroes->length; i++) {
			auto* hero{ heroes->list[i] };
			if (hero == player)
				continue;

			auto is_enemy{ my_team != hero->get_team() };
			auto& config_array{ is_enemy ? Config::config.current_combo_enemy_skin_index : Config::config.current_combo_ally_skin_index };
			auto champion_name_hash{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) };
			auto config_entry{ config_array.find(champion_name_hash) };
			if (config_entry == config_array.end())
				continue;

			if (config_entry->second > 0) {
				auto& values = SkinDatabase::champions_skins[champion_name_hash];
				hero->change_skin(values[config_entry->second - 1].model_name.c_str(), values[config_entry->second - 1].skin_id);
			}
		}
	});

	static const auto change_skin_for_object = [](AIBaseCommon* obj, const int32_t skin) -> void {
		if (skin == -1)
			return;

		if (obj->get_character_data_stack()->base_skin.skin != skin) {
			obj->get_character_data_stack()->base_skin.skin = skin;
			obj->get_character_data_stack()->update(true);
		}
	};

	for (auto i{ 0u }; i < minions->length; ++i) {
		auto* minion{ minions->list[i] };
		auto* owner{ minion->get_gold_redirect_target() };

		if (!owner)
			continue;

		auto hash{ fnv::hash_runtime(minion->get_character_data_stack()->base_skin.model.str) };
		if (hash == FNV("JammerDevice") || hash == FNV("SightWard") || hash == FNV("YellowTrinket") || hash == FNV("VisionWard") || hash == FNV("TestCubeRender10Vision")) {
			if (!player || owner == player) {
				if (hash == FNV("TestCubeRender10Vision"))
					change_skin_for_object(minion, 0);
				else
					change_skin_for_object(minion, Config::config.current_ward_skin_index);
			}
			continue;
		}
		change_skin_for_object(minion, owner->get_character_data_stack()->base_skin.skin);
	}
}

void Hooks::install() noexcept
{
	auto material_registry{ reinterpret_cast<std::uintptr_t(__stdcall*)()>(reinterpret_cast<std::uintptr_t>(::GetModuleHandleA(nullptr)) + offsets::functions::Riot__Renderer__MaterialRegistry__GetSingletonPtr)() };
	auto* d3d_device{ *reinterpret_cast<IDirect3DDevice9**>(material_registry + offsets::MaterialRegistry::D3DDevice) };
	auto* swap_chain{ *reinterpret_cast<IDXGISwapChain**>(material_registry + offsets::MaterialRegistry::SwapChain) };

	if (d3d_device) {
		d3d_device_vmt = std::make_unique<::vmt_smart_hook>(d3d_device);
		d3d_device_vmt->apply_hook<d3d_vtable::end_scene>(42);
		d3d_device_vmt->apply_hook<d3d_vtable::reset>(16);
	} else if (swap_chain) {
		swap_chain_vmt = std::make_unique<::vmt_smart_hook>(swap_chain);
		swap_chain_vmt->apply_hook<d3d_vtable::dxgi_present>(8);
		swap_chain_vmt->apply_hook<d3d_vtable::dxgi_resize_buffers>(13);
	}
}

void Hooks::uninstall() noexcept
{
	::SetWindowLongW(*reinterpret_cast<HWND*>(reinterpret_cast<std::uintptr_t>(::GetModuleHandleA(nullptr)) + offsets::global::Riot__g_window), GWLP_WNDPROC, LONG_PTR(originalWndProc));

	if (d3d_device_vmt)
		d3d_device_vmt->unhook();
	if (swap_chain_vmt)
		swap_chain_vmt->unhook();

	run = false;
}
