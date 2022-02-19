#include <fstream>
#include <string>

#include "Json/json.hpp"
#include "fnv_hash.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_extend.h"

#include "CheatManager.hpp"
#include "GUI.hpp"
#include "Memory.hpp"
#include "Utils.hpp"

void GUI::render() noexcept
{
	ImGui::Begin("R3nzSkin", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize);
	{
		static const auto player{ cheatManager.memory->localPlayer };
		static const auto heroes{ cheatManager.memory->heroList };
		static const auto my_team{ player ? player->get_team() : 100 };

		static const auto vector_getter_skin = [](void* vec, std::int32_t idx, const char** out_text) noexcept {
			const auto& vector{ *static_cast<std::vector<SkinDatabase::skin_info>*>(vec) };
			if (idx < 0 || idx > static_cast<std::int32_t>(vector.size())) return false;
			*out_text = idx == 0 ? "Default" : vector.at(idx - 1).skin_name.c_str();
			return true;
		};

		static const auto vector_getter_ward_skin = [](void* vec, std::int32_t idx, const char** out_text) noexcept {
			const auto& vector{ *static_cast<std::vector<std::pair<std::int32_t, std::string>>*>(vec) };
			if (idx < 0 || idx > static_cast<std::int32_t>(vector.size())) return false;
			*out_text = idx == 0 ? "Default" : vector.at(idx - 1).second.c_str();
			return true;
		};

		ImGui::rainbowText();

		if (player) {
			auto& values{ cheatManager.database->champions_skins[fnv::hash_runtime(player->get_character_data_stack()->base_skin.model.str)] };
			ImGui::Text("Player Skins Settings:");

			if (ImGui::Combo("Current Skin", &cheatManager.config->config.current_combo_skin_index, vector_getter_skin, static_cast<void*>(&values), values.size() + 1))
				if (cheatManager.config->config.current_combo_skin_index > 0)
					player->change_skin(values[cheatManager.config->config.current_combo_skin_index - 1].model_name.c_str(), values[cheatManager.config->config.current_combo_skin_index - 1].skin_id);

			if (ImGui::Combo("Current Ward Skin", &cheatManager.config->config.current_combo_ward_index, vector_getter_ward_skin, static_cast<void*>(&cheatManager.database->wards_skins), cheatManager.database->wards_skins.size() + 1))
				cheatManager.config->config.current_ward_skin_index = cheatManager.config->config.current_combo_ward_index == 0 ? -1 : cheatManager.database->wards_skins.at(cheatManager.config->config.current_combo_ward_index - 1).first;

			ImGui::Separator();
		}

		ImGui::Text("Other Champs Skins Settings:");
		std::int32_t last_team{ 0 };
		for (auto i{ 0u }; i < heroes->length; ++i) {
			const auto hero{ heroes->list[i] };
			
			if (hero == player)
				continue;

			const auto champion_name_hash{ fnv::hash_runtime(hero->get_character_data_stack()->base_skin.model.str) };
			if (champion_name_hash == FNV("PracticeTool_TargetDummy"))
				continue;

			const auto hero_team{ hero->get_team() };
			const auto is_enemy{ hero_team != my_team };

			if (last_team == 0 || hero_team != last_team) {
				if (last_team != 0)
					ImGui::Separator();
				if (is_enemy)
					ImGui::Text(" Enemy champions");
				else
					ImGui::Text(" Ally champions");
				last_team = hero_team;
			}

			auto& config_array{ is_enemy ? cheatManager.config->config.current_combo_enemy_skin_index : cheatManager.config->config.current_combo_ally_skin_index };
			const auto config_entry{ config_array.insert({ champion_name_hash, 0 }) };

			snprintf(this->str_buffer, sizeof(this->str_buffer), cheatManager.config->config.heroName ? "HeroName: [ %s ]##%X" : "PlayerName: [ %s ]##%X", cheatManager.config->config.heroName ? hero->get_character_data_stack()->base_skin.model.str : hero->get_name().c_str(), reinterpret_cast<std::uintptr_t>(hero));

			auto& values{ cheatManager.database->champions_skins[champion_name_hash] };
			if (ImGui::Combo(str_buffer, &config_entry.first->second, vector_getter_skin, static_cast<void*>(&values), values.size() + 1))
				if (config_entry.first->second > 0)
					hero->change_skin(values[config_entry.first->second - 1].model_name.c_str(), values[config_entry.first->second - 1].skin_id);
		}

		ImGui::Separator();
		ImGui::hotkey("Menu Key", cheatManager.config->config.menuKey);
		ImGui::Checkbox(cheatManager.config->config.heroName ? "HeroName based" : "PlayerName based", &cheatManager.config->config.heroName);
		ImGui::Checkbox("Rainbow Text", &cheatManager.config->config.rainbowText);
		ImGui::Checkbox("Quick Skin Change", &cheatManager.config->config.quickSkinChange);

		if (cheatManager.config->config.quickSkinChange) {
			ImGui::Separator();
			ImGui::hotkey("Previous Skin Key", cheatManager.config->config.previousSkinKey);
			ImGui::hotkey("Next Skin Key", cheatManager.config->config.nextSkinKey);
			ImGui::Separator();
		}

		ImGui::Text("FPS: %.0f FPS", ImGui::GetIO().Framerate);
		
		if (ImGui::Button("Force Close"))
			cheatManager.hooks->uninstall();

		ImGui::Separator();
		ImGui::textUnformattedCentered((std::string("Last Build: ") + __DATE__ + " - " + __TIME__).c_str());
		ImGui::textUnformattedCentered("Copyright (C) 2021-2022 R3nzTheCodeGOD");
	}
	ImGui::End();
}
