#pragma once

#include <Windows.h>
#include <mutex>

inline std::once_flag change_skins;
inline WNDPROC originalWndProc;
inline bool run;

namespace Hooks {
	void init() noexcept;
	void WINAPI install() noexcept;
	void WINAPI uninstall() noexcept;
};
