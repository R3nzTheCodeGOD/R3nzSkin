#include <Windows.h>
#include <algorithm>
#include <array>
#include <string_view>

#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "CheatManager.hpp"
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

KeyBind::KeyBind(const KeyCode keyCode) noexcept : keyCode{ static_cast<std::size_t>(keyCode) < keyMap.size() ? keyCode : KeyCode::NONE } {  }

KeyBind::KeyBind(const char* keyName) noexcept
{
    if (const auto it{ std::ranges::lower_bound(keyMap, keyName, {}, &Key::name) }; it != keyMap.end() && it->name == keyName)
        keyCode = static_cast<KeyCode>(std::distance(keyMap.begin(), it));
    else
        keyCode = KeyCode::NONE;
}

const char* KeyBind::toString() const noexcept
{
    return keyMap[static_cast<std::size_t>(keyCode) < keyMap.size() ? keyCode : KeyCode::NONE].name.data();
}

int KeyBind::getKey() const noexcept
{
    return keyMap[static_cast<std::size_t>(keyCode) < keyMap.size() ? keyCode : KeyCode::NONE].code;
}

bool KeyBind::isPressed() const noexcept
{
    if (keyCode == KeyCode::NONE)
        return false;

    if (keyCode == KeyCode::MOUSEWHEEL_DOWN)
        return ImGui::GetIO().MouseWheel < 0.0f;

    if (keyCode == KeyCode::MOUSEWHEEL_UP)
        return ImGui::GetIO().MouseWheel > 0.0f;

    if (keyCode >= KeyCode::MOUSE1 && keyCode <= KeyCode::MOUSE5)
        return ImGui::IsMouseClicked(keyMap[keyCode].code);

    return static_cast<std::size_t>(keyCode) < keyMap.size() && ImGui::IsKeyPressed(keyMap[keyCode].code, false);
}

bool KeyBind::isDown() const noexcept
{
    if (keyCode == KeyCode::NONE)
        return false;

    if (keyCode == KeyCode::MOUSEWHEEL_DOWN)
        return ImGui::GetIO().MouseWheel < 0.0f;

    if (keyCode == KeyCode::MOUSEWHEEL_UP)
        return ImGui::GetIO().MouseWheel > 0.0f;

    if (keyCode >= KeyCode::MOUSE1 && keyCode <= KeyCode::MOUSE5)
        return ImGui::IsMouseDown(keyMap[keyCode].code);

    return static_cast<std::size_t>(keyCode) < keyMap.size() && ImGui::IsKeyDown(keyMap[keyCode].code);
}

bool KeyBind::setToPressedKey() noexcept
{
    if (ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
        keyCode = KeyCode::NONE;
        return true;
    } else if (ImGui::GetIO().MouseWheel < 0.0f) {
        keyCode = KeyCode::MOUSEWHEEL_DOWN;
        return true;
    } else if (ImGui::GetIO().MouseWheel > 0.0f) {
        keyCode = KeyCode::MOUSEWHEEL_UP;
        return true;
    }

    for (auto i{ 0u }; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); ++i) {
        if (ImGui::IsMouseClicked(i)) {
            keyCode = static_cast<KeyCode>(KeyCode::MOUSE1 + i);
            return true;
        }
    }

    for (auto i{ 0u }; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); ++i) {
        if (!ImGui::IsKeyPressed(i))
            continue;

        if (const auto it{ std::ranges::find(keyMap, i, &Key::code) }; it != keyMap.end()) {
            keyCode = static_cast<KeyCode>(std::distance(keyMap.begin(), it));
            if (keyCode == KeyCode::LCTRL && ImGui::IsKeyPressed(keyMap[KeyCode::RALT].code))
                keyCode = KeyCode::RALT;
            return true;
        }
    }
    return false;
}

void KeyBindToggle::handleToggle() noexcept
{
    if (isPressed())
        toggledOn = !toggledOn;
}

void ImGui::textUnformattedCentered(const char* text) noexcept
{
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) / 2.0f);
    ImGui::TextUnformatted(text);
}

void ImGui::hoverInfo(const char* desc) noexcept
{
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ImGui::rainbowText() noexcept
{
	static float r{ 1.0f };
	static float g{ 0.f };
	static float b{ 0.f };

	if (cheatManager.config->rainbowText) {
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

struct InputTextCallback_UserData {
    std::string* Str;
    ImGuiInputTextCallback ChainCallback;
    void* ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data) noexcept
{
    const auto user_data{ static_cast<InputTextCallback_UserData*>(data->UserData) };
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        const auto str{ user_data->Str };
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = const_cast<char*>(str->c_str());
    } else if (user_data->ChainCallback) {
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool ImGui::InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* userData) noexcept
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;
    auto cbUserData{ InputTextCallback_UserData(str, callback, userData) };
    return InputText(label, const_cast<char*>(str->c_str()), str->capacity() + 1, flags, InputTextCallback, &cbUserData);
}

void ImGui::hotkey(const char* label, KeyBind& key, float samelineOffset, const ImVec2& size) noexcept
{
    const auto id{ GetID(label) };
    PushID(label);

    TextUnformatted(label);
    SameLine(samelineOffset);

    if (GetActiveID() == id) {
        PushStyleColor(ImGuiCol_Button, GetColorU32(ImGuiCol_ButtonActive));
        Button("...", size);
        PopStyleColor();

        GetCurrentContext()->ActiveIdAllowOverlap = true;
        if ((!IsItemHovered() && GetIO().MouseClicked[0]) || key.setToPressedKey())
            ClearActiveID();
    } else if (Button(key.toString(), size)) {
        SetActiveID(id, GetCurrentWindow());
    }

    PopID();
}
