#include <Windows.h>
#include <algorithm>
#include <array>
#include <string_view>

#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "Config.hpp"
#include "Utils.hpp"

struct Key {
    constexpr Key(std::string_view name, int code) : name{ name }, code{ code } {  }

    std::string_view name;
    int code;
};

// indices must match KeyBind::KeyCode enum
static constexpr auto keyMap = std::to_array<Key>({
    { "'", VK_OEM_7 },
    { ",", VK_OEM_COMMA },
    { "-", VK_OEM_MINUS },
    { ".", VK_OEM_PERIOD },
    { "/", VK_OEM_2 },
    { "0", '0' },
    { "1", '1' },
    { "2", '2' },
    { "3", '3' },
    { "4", '4' },
    { "5", '5' },
    { "6", '6' },
    { "7", '7' },
    { "8", '8' },
    { "9", '9' },
    { ";", VK_OEM_1 },
    { "=", VK_OEM_PLUS },
    { "A", 'A' },
    { "ADD", VK_ADD },
    { "B", 'B' },
    { "BACKSPACE", VK_BACK },
    { "C", 'C' },
    { "CAPSLOCK", VK_CAPITAL },
    { "D", 'D' },
    { "DECIMAL", VK_DECIMAL },
    { "DELETE", VK_DELETE },
    { "DIVIDE", VK_DIVIDE },
    { "DOWN", VK_DOWN },
    { "E", 'E' },
    { "END", VK_END },
    { "ENTER", VK_RETURN },
    { "F", 'F' },
    { "F1", VK_F1 },
    { "F10", VK_F10 },
    { "F11", VK_F11 },
    { "F12", VK_F12 },
    { "F2", VK_F2 },
    { "F3", VK_F3 },
    { "F4", VK_F4 },
    { "F5", VK_F5 },
    { "F6", VK_F6 },
    { "F7", VK_F7 },
    { "F8", VK_F8 },
    { "F9", VK_F9 },
    { "G", 'G' },
    { "H", 'H' },
    { "HOME", VK_HOME },
    { "I", 'I' },
    { "INSERT", VK_INSERT },
    { "J", 'J' },
    { "K", 'K' },
    { "L", 'L' },
    { "LALT", VK_LMENU },
    { "LCTRL", VK_LCONTROL },
    { "LEFT", VK_LEFT },
    { "LSHIFT", VK_LSHIFT },
    { "M", 'M' },
    { "MOUSE1", 0x0 },
    { "MOUSE2", 0x1 },
    { "MOUSE3", 0x2 },
    { "MOUSE4", 0x3 },
    { "MOUSE5", 0x4 },
    { "MULTIPLY", VK_MULTIPLY },
    { "MWHEEL_DOWN", 0x0 },
    { "MWHEEL_UP", 0x0 },
    { "N", 'N' },
    { "NONE", 0x0 },
    { "NUMPAD_0", VK_NUMPAD0 },
    { "NUMPAD_1", VK_NUMPAD1 },
    { "NUMPAD_2", VK_NUMPAD2 },
    { "NUMPAD_3", VK_NUMPAD3 },
    { "NUMPAD_4", VK_NUMPAD4 },
    { "NUMPAD_5", VK_NUMPAD5 },
    { "NUMPAD_6", VK_NUMPAD6 },
    { "NUMPAD_7", VK_NUMPAD7 },
    { "NUMPAD_8", VK_NUMPAD8 },
    { "NUMPAD_9", VK_NUMPAD9 },
    { "O", 'O' },
    { "P", 'P' },
    { "PAGE_DOWN", VK_NEXT },
    { "PAGE_UP", VK_PRIOR },
    { "Q", 'Q' },
    { "R", 'R' },
    { "RALT", VK_RMENU },
    { "RCTRL", VK_RCONTROL },
    { "RIGHT", VK_RIGHT },
    { "RSHIFT", VK_RSHIFT },
    { "S", 'S' },
    { "SPACE", VK_SPACE },
    { "SUBTRACT", VK_SUBTRACT },
    { "T", 'T' },
    { "TAB", VK_TAB },
    { "U", 'U' },
    { "UP", VK_UP },
    { "V", 'V' },
    { "W", 'W' },
    { "X", 'X' },
    { "Y", 'Y' },
    { "Z", 'Z' },
    { "[", VK_OEM_4 },
    { "\\", VK_OEM_5 },
    { "]", VK_OEM_6 },
    { "`", VK_OEM_3 }
});

static_assert(keyMap.size() == KeyBind::MAX);
static_assert(std::ranges::is_sorted(keyMap, {}, &Key::name));

void ImGui::textUnformattedCentered(const char* text) noexcept
{
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2.0f);
    ImGui::TextUnformatted(text);
}

void ImGui::rainbowText() noexcept
{
	static float r{ 1.0f };
	static float g{ 0.f };
	static float b{ 0.f };

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
	} else {
		if (auto& clr{ ImGui::GetStyle().Colors[ImGuiCol_Text] }; clr.x != 0.92f && clr.y != 0.92f && clr.z != 0.92f)
			clr = ImVec4(0.92f, 0.92f, 0.92f, 0.92f);
	}
}
