#pragma once

#include <cstdlib>
#include <cstdint>

namespace detail {
	template <typename Type, Type OffsetBasis, Type Prime>
	struct size_dependant_data {
		using type = Type;
		constexpr static auto k_offset_basis{ OffsetBasis };
		constexpr static auto k_prime{ Prime };
	};

	template <std::size_t Bits>
	struct size_selector;

	template <>
	struct size_selector<32> {
		using type = size_dependant_data<std::uint32_t, 0x811c9dc5ul, 16777619ul>;
	};

	template <>
	struct size_selector<64> {
		using type = size_dependant_data<std::uint64_t, 0xcbf29ce484222325ull, 1099511628211ull>;
	};

	template <std::size_t Size>
	class fnv_hash {
	private:
		using data_t = typename size_selector<Size>::type;

	public:
		using hash = typename data_t::type;

	private:
		constexpr static auto k_offset_basis{ data_t::k_offset_basis };
		constexpr static auto k_prime{ data_t::k_prime };

	public:
		template <std::size_t N>
		static __forceinline constexpr auto hash_constexpr(const char(&str)[N], const std::size_t size = N) -> hash {
			return static_cast<hash>(1ull * (size == 1 ? (k_offset_basis ^ str[0]) : (hash_constexpr(str, size - 1) ^ str[size - 1])) * k_prime);
		}

		static auto __forceinline hash_runtime(const char* str) -> hash {
			auto result{ k_offset_basis };
			do {
				result ^= *str++;
				result *= k_prime;
			} while (*(str - 1) != '\0');

			return result;
		}
	};
}

using fnv = ::detail::fnv_hash<sizeof(void*) * 8>;

#define FNV(str) (std::integral_constant<fnv::hash, fnv::hash_constexpr(str)>::value)
