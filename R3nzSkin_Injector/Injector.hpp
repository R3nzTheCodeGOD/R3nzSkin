#pragma once

#include <Windows.h>
#include <chrono>
#include <cinttypes>
#include <string>
#include <vector>

using namespace std::chrono_literals;
using proclist_t = std::vector<std::uint32_t>;

NTSTATUS NTAPI NtCreateThreadEx(PHANDLE, ACCESS_MASK, LPVOID, HANDLE, LPTHREAD_START_ROUTINE, LPVOID, BOOL, SIZE_T, SIZE_T, SIZE_T, LPVOID);

class Injector {
public:
	static proclist_t WINAPI findProcesses(const std::wstring name) noexcept;
	static bool WINAPI isInjected(const std::uint32_t pid) noexcept;
	static bool WINAPI inject(const std::uint32_t pid) noexcept;
	static void WINAPI enableDebugPrivilege() noexcept;
    static std::string randomString(std::uint32_t size) noexcept;
    static void renameExe() noexcept;
	static void run() noexcept;
};
