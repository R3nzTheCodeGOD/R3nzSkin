#pragma once

#include <Windows.h>
#include <chrono>
#include <cinttypes>
#include <string>
#include <vector>

using namespace std::chrono_literals;
using proclist_t = std::vector<std::uint32_t>;

struct NtCreateThreadExBuffer {
    SIZE_T Size;
    SIZE_T Unknown1;
    SIZE_T Unknown2;
    PULONG Unknown3;
    SIZE_T Unknown4;
    SIZE_T Unknown5;
    SIZE_T Unknown6;
    PULONG Unknown7;
    SIZE_T Unknown8;
};

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
