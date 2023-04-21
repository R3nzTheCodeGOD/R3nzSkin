#pragma once
#pragma once

#include <cstdint>

#include "x64RetSpoof.hpp"

class RetSpoofInvoker {
private:
	void* gadgetAddress{ 0 };
public:
	void init(std::uintptr_t gadgetAddress) noexcept
	{
		this->gadgetAddress = reinterpret_cast<void*>(gadgetAddress);
	}

	template <typename ReturnType, typename... Args>
	ReturnType invokeFastcall(std::uintptr_t functionAddress, Args&&... args) const noexcept
	{
		return x64RetSpoof::FunctionTraits<ReturnType(Args...)>::spoof_call(this->gadgetAddress, reinterpret_cast<void*>(functionAddress), std::forward<Args>(args)...);
	}
};

inline RetSpoofInvoker invoker;
