#pragma once

#include <Windows.h>
#include <mutex>

inline std::once_flag change_skins;
inline WNDPROC originalWndProc;

class Hooks {
public:
	void init() const noexcept;
	void install() const noexcept;
	void uninstall() const noexcept;
};
