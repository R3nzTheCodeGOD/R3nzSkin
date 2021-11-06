#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "Offsets.hpp"

#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)
#define PAD(size) std::byte PAD_NAME(__LINE__) [size];

enum class GGameState_s : std::int32_t {
	LoadingScreen = 0,
	Connecting = 1,
	Running = 2,
	Poaused = 3,
	Finished = 4,
	Exiting = 5
};

class GameClient {
	PAD(0x8);
public:
	GGameState_s game_state;
};

class AString {
public:
	const char* str;
	std::size_t length;
	std::size_t capacity;
};

class Champion {
public:
	class Skin {
	public:
		std::int32_t skin_id;
		AString skin_name;
	};
	PAD(0x4);
	AString champion_name;
	PAD(0x48);
	std::vector<Skin> skins;
};

class ChampionManager {
	PAD(0x10);
public:
	std::vector<Champion*> champions;
};

class CharacterStackData {
public:
	AString model;
	std::int32_t skin;
	PAD(0x20);
	bool update_spells;
	bool dont_update_hud;
	bool change_particle;
	PAD(0x1);
	PAD(0xC);
};

class CharacterDataStack {
public:
	std::vector<CharacterStackData> stack;
	CharacterStackData base_skin;

	void update(const bool change) noexcept;
	void push(const char* model, std::int32_t skin) noexcept;
};

class GameObject {
public:
	std::string& get_name() noexcept { return *reinterpret_cast<std::string*>(std::uintptr_t(this) + offsets::GameObject::Name); }
	std::int32_t get_team() noexcept { return *reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + offsets::GameObject::Team); }
	std::int32_t get_id() noexcept { return *reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + offsets::GameObject::ID); }
};

class AIBaseCommon : public GameObject {
public:
	CharacterDataStack* get_character_data_stack() noexcept { return reinterpret_cast<CharacterDataStack*>(std::uintptr_t(this) + offsets::AIBaseCommon::CharacterDataStack); }

	bool skin_model_push(const char* model, std::int32_t skin) noexcept;
	void change_skin(const char* model, std::int32_t skin) noexcept;
};

class AIHero : public AIBaseCommon {
public:
};

class AIMinionClient : public AIBaseCommon {
public:
	AIBaseCommon* get_gold_redirect_target() noexcept;
};

template <class T>
class ManagerTemplate {
	PAD(0x4);
public:
	T** list;
	std::size_t length;
	std::size_t capacity;
};
