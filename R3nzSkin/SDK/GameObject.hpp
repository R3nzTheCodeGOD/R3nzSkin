#pragma once

#include <cstdint>
#include <string>

#include "../offsets.hpp"
#include "Pad.hpp"

class GameObject {
public:
	CLASS_GETTER_P(std::string, get_name, offsets::GameObject::Name)
	CLASS_GETTER(std::int32_t, get_team, offsets::GameObject::Team)
};