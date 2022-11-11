#pragma once
#pragma once

#include <cstdint>

#include "x86RetSpoof.hpp"

class RetSpoofInvoker {
private:
    std::uintptr_t gadgetAddress{ 0 };
public:
    void init(std::uintptr_t gadgetAddress) noexcept
    {
        this->gadgetAddress = gadgetAddress;
    }

    template <typename ReturnType, typename... Args>
    ReturnType invokeFastcall(std::uintptr_t ecx, std::uintptr_t edx, std::uintptr_t functionAddress, Args... args) const noexcept
    {
        return x86RetSpoof::invokeFastcall<ReturnType, Args...>(ecx, edx, functionAddress, this->gadgetAddress, args...);
    }

    template <typename ReturnType, typename... Args>
    ReturnType invokeThiscall(std::uintptr_t ecx, std::uintptr_t functionAddress, Args... args) const noexcept
    {
        return x86RetSpoof::invokeThiscall<ReturnType, Args...>(ecx, functionAddress, this->gadgetAddress, args...);
    }

    template <typename ReturnType, std::size_t index, typename... Args>
    ReturnType invokeThiscall(std::uintptr_t ecx, Args... args) const noexcept
    {
        return x86RetSpoof::invokeThiscall<ReturnType, Args...>(ecx, (*reinterpret_cast<std::uintptr_t**>(ecx))[index], this->gadgetAddress, args...);
    }

    template <typename ReturnType, typename... Args>
    ReturnType invokeStdcall(std::uintptr_t functionAddress, Args... args) const noexcept
    {
        return x86RetSpoof::invokeStdcall<ReturnType, Args...>(functionAddress, this->gadgetAddress, args...);
    }

    template <typename ReturnType, typename... Args>
    ReturnType invokeCdecl(std::uintptr_t functionAddress, Args... args) const noexcept
    {
        return x86RetSpoof::invokeCdecl<ReturnType, Args...>(functionAddress, this->gadgetAddress, args...);
    }
};

inline RetSpoofInvoker invoker;
