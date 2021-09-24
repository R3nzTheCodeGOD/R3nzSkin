#pragma once
#include <vector>
#include <string>

#include "../imgui/imgui.h"

namespace ImGui {
	bool Combo( const char* label, int* currIndex, std::vector<std::string>& values );
};