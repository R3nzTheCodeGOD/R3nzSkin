#include <type_traits>


void* GetGadget(const wchar_t* moduleName);
extern "C" void* NoStackShellcode();

namespace x64RetSpoof
{
	struct shell_params
	{
		const void* gadget;
		void* realfunction_addr;
		void* Nonvolatile_register;
		void* shellcode_retaddr;
		void* shellcode_fixstack;
	};

	template <typename RET>
	static inline RET shellcode_stub_arg6(const void* shell, void* shell_param, void* first = 0, void* second = 0, void* third = 0, void* forth = 0)
	{
		auto fn = (RET(*)(void*, void*, void*, void*, void*))(shell);
		return fn(shell_param, first, second, third, forth);
	}

	template <typename RET, typename... ARGS>
	static inline RET shellcode_stub(const void* shell, void* shell_param, ARGS&&... args)
	{
		auto fn = (RET(*)(void*, ARGS...))(shell);
		return fn(shell_param, std::forward<ARGS>(args)...);
	}

	template<typename RET, typename... ARGS>
	struct argument_remapper
	{
		static RET do_call(void* shell_param, ARGS&&... args)
		{
			if constexpr (sizeof...(args) >= 4)
			{
				return shellcode_stub<RET, ARGS...>(&NoStackShellcode, shell_param, std::forward<ARGS>(args)...);
			}
			else
			{
				return shellcode_stub_arg6<RET>(&NoStackShellcode, shell_param, (void*)std::forward<ARGS>(args)...);
			}
		}
	};

	template<typename T>	concept NOT_USE_XMM = !std::is_floating_point_v<T>;

	template <typename RET, typename... ARGS>
	struct FunctionTraits {
	};

	template <typename RET, typename... ARGS>
	struct FunctionTraits<RET(ARGS...)> {
	};

	template <typename RET, NOT_USE_XMM... ARGS>
	struct FunctionTraits<RET(ARGS...)> {
		static inline RET spoof_call(const void* gadget, void* fn, ARGS&&... args)
		{
			shell_params param{ .gadget = gadget, .realfunction_addr = fn };
			return argument_remapper<RET, ARGS...>::do_call(&param, std::forward<ARGS>(args)...);
		}
	};
}
