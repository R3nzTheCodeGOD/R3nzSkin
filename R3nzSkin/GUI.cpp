#include <Windows.h>
#include <fstream>
#include <string>

#include "Json/json.hpp"
#include "fnv_hash.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_extend.h"

#include "Config.hpp"
#include "GUI.hpp"
#include "GameClasses.hpp"
#include "Hooks.hpp"
#include "Offsets.hpp"
#include "SkinDatabase.hpp"

void TextCenter(std::string text) noexcept
{
	auto font_size{ (ImGui::GetFontSize() * (text.size() / 2)) };
	ImGui::SameLine(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2));
	ImGui::Text(text.c_str());
}

char str_buffer[256];
void GUI::render() noexcept
{
	ImGui::Begin("R3nzSkin", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize);
	{
		static float r{ 1.0f };
		static float g{ 0.f };
		static float b{ 0.f };
		auto* player{ *reinterpret_cast<AIBaseCommon**>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::global::Player) };
		auto* heroes{ *reinterpret_cast<ManagerTemplate<AIHero>**>(std::uintptr_t(::GetModuleHandleA(nullptr)) + offsets::global::ManagerTemplate_AIHero_) };
		auto my_team{ player ? player->get_team() : 100 };

		if (Config::config.rainbowText) {
			if (r == 1.f && g >= 0.f && b <= 0.f) {
				g += 0.005f;
				b = 0.f;
			}
			if (r <= 1.f && g >= 1.f && b == 0.f) {
				g = 1.f;
				r -= 0.005f;
			}
			if (r <= 0.f && g == 1.f && b >= 0.f) {
				r = 0.f;
				b += 0.005f;
			}
			if (r == 0.f && g <= 1.f && b >= 1.f) {
				b = 1.f;
				g -= 0.005f;
			}
			if (r >= 0.f && g <= 0.f && b == 1.f) {
				g = 0.f;
				r += 0.005f;
			}
			if (r >= 1.f && g >= 0.f && b <= 1.f) {
				r = 1.f;
				b -= 0.005f;
			}
			ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(r, g, b, 1.00f);
		} else
			if (auto& clr{ ImGui::GetStyle().Colors[ImGuiCol_Text] }; clr.x != 0.92f && clr.y != 0.92f && clr.z != 0.92f)
				clr = ImVec4(0.92f, 0.92f, 0.92f, 0.92f);

		static const auto vector_getter_skin = [](void* vec, int32_t idx, const char** out_text) {
			const auto& vector{ *static_cast<std::vector<SkinDatabase::skin_info>*>(vec) };
			if (idx < 0 || idx > static_cast<int32_t>(vector.size())) { return false; }
			*out_text = idx == 0 ? "Default" : vector.at(idx - 1).skin_name.c_str();
			return true;
		};

		static const auto vector_getter_ward_skin = [](void* vec, int32_t idx, const char** out_text) {
			const auto& vector{ *static_cast<std::vector<std::pair<int32_t, std::string>>*>(vec) };
			if (idx < 0 || idx > static_cast<int32_t>(vector.size())) { return false; }
			*out_text = idx == 0 ? "Default" : vector.at(idx - 1).second.c_str();
			return true;
		};

		if (player) {
			auto& values{ SkinDatabase::champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };
			ImGui::Text("Player Skins Settings:");

			if (ImGui::Combo("Current Skin", &Config::config.current_combo_skin_index, vector_getter_skin, static_cast<void*>(&values), values.size() + 1))
				if (Config::config.current_combo_skin_index > 0)
					player->change_skin(values[Config::config.current_combo_skin_index - 1].model_name.c_str(), values[Config::config.current_combo_skin_index - 1].skin_id);

			if (ImGui::Combo("Current Ward Skin", &Config::config.current_combo_ward_index, vector_getter_ward_skin, static_cast<void*>(&SkinDatabase::wards_skins), SkinDatabase::wards_skins.size() + 1))
				Config::config.current_ward_skin_index = Config::config.current_combo_ward_index == 0 ? -1 : SkinDatabase::wards_skins.at(Config::config.current_combo_ward_index - 1).first;

			ImGui::Separator();
		}

		ImGui::Text("Other Champs Skins Settings:");
		int32_t last_team{ 0 };
		for (auto i{ 0u }; i < heroes->length; ++i) {
			auto* hero{ heroes->list[i] };
			if (hero == player)
				continue;

			auto hero_team{ hero->get_team() };
			auto is_enemy{ hero_team != my_team };

			if (last_team == 0 || hero_team != last_team) {
				if (last_team != 0)
					ImGui::Separator();
				if (is_enemy)
					ImGui::Text(" Enemy champions");
				else
					ImGui::Text(" Ally champions");
				last_team = hero_team;
			}

			auto& config_array{ is_enemy ? Config::config.current_combo_enemy_skin_index : Config::config.current_combo_ally_skin_index };
			auto champion_name_hash{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) };
			auto config_entry{ config_array.insert({ champion_name_hash, 0 }) };

			snprintf(str_buffer, 256, Config::config.heroName ? "HeroName: [ %s ]##%X" : "PlayerName: [ %s ]##%X", Config::config.heroName ? hero->get_character_data_stack()->base_skin.model.str : hero->get_name().c_str(), reinterpret_cast<std::uintptr_t>(hero));

			auto& values{ SkinDatabase::champions_skins[champion_name_hash] };
			if (ImGui::Combo(str_buffer, &config_entry.first->second, vector_getter_skin, static_cast<void*>(&values), values.size() + 1))
				if (config_entry.first->second > 0)
					hero->change_skin(values[config_entry.first->second - 1].model_name.c_str(), values[config_entry.first->second - 1].skin_id);
		}

		ImGui::Separator();
		ImGui::Checkbox("RainbowText", &Config::config.rainbowText);
		ImGui::Checkbox(Config::config.heroName ? "HeroName based" : "PlayerName based", &Config::config.heroName);
		ImGui::Text("FPS: %.0f FPS", ImGui::GetIO().Framerate);
		
		if (ImGui::Button("UnHook"))
			Hooks::uninstall();

		ImGui::Separator();
		TextCenter("Copyright (C) 2021 R3nzTheCodeGOD");

	}
	ImGui::End();
}
