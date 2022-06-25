#pragma once

#include <cstdint>
#include <string>

#include "../offsets.hpp"
#include "../imgui/imgui.h"
#include "CallVirtual.hpp"
#include "Vector.hpp"


class GameObject {
public:
	[[nodiscard]] std::string& get_name() const noexcept { return *reinterpret_cast<std::string*>(std::uintptr_t(this) + offsets::GameObject::Name); }
	[[nodiscard]] std::int32_t get_team() const noexcept { return *reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + offsets::GameObject::Team); }
	[[nodiscard]] Vector get_position() const noexcept { return *reinterpret_cast<Vector*>(std::uintptr_t(this) + offsets::GameObject::Position); }
	[[nodiscard]] bool get_visiblity() const noexcept { return *reinterpret_cast<bool*>(std::uintptr_t(this) + offsets::GameObject::Visible); }
	[[nodiscard]] float getBoundingRadius() noexcept { return callVirtual<34, float>(this); }

	[[nodiscard]] bool isOnScreen(const Vector& vec) const noexcept { return (((vec.x > 0) && vec.x < ImGui::GetIO().DisplaySize.x) && ((vec.y > 0) && vec.y < ImGui::GetIO().DisplaySize.y)); }
};