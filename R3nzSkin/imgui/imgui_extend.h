#pragma once
#include <string>
#include <vector>

#include "../depend/imgui/imgui.h"

namespace ImGui {
bool Combo(const char *label, int *currIndex, std::vector<std::string> &values);
};