#pragma once

#include <cstdint>
#include <string>

#include "../offsets.hpp"

class GameObject {
public:
	[[nodiscard]] std::string& get_name() const noexcept { return *reinterpret_cast<std::string*>(std::uintptr_t(this) + offsets::GameObject::Name); }
	[[nodiscard]] std::int32_t get_team() const noexcept { return *reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + offsets::GameObject::Team); }
	[[nodiscard]] std::int32_t get_id() const noexcept { return *reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + offsets::GameObject::ID); }
};