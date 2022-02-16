#pragma once

#include <cstdint>
#include <string>

#include "../offsets.hpp"

class GameObject {
public:
	std::string& get_name() noexcept { return *reinterpret_cast<std::string*>(std::uintptr_t(this) + offsets::GameObject::Name); }
	std::int32_t get_team() noexcept { return *reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + offsets::GameObject::Team); }
	std::int32_t get_id() noexcept { return *reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + offsets::GameObject::ID); }
};