#pragma once

#include <cstdint>
#include <intrin.h>

#pragma pack(push, 4)
template <typename T>
class xor_value {
	bool xor_key_was_init{ 0 };
	std::uint8_t bytes_xor_count;
	std::uint8_t bytes_xor_count_8;
	T xor_key;
	std::uint8_t value_index{ 0 };
	T values_table[4];
public:
	T decrypt() noexcept
	{
		if (xor_key_was_init != 1)
			return 0;

		auto xored_value{ values_table[value_index] };
		auto xor_key_value{ xor_key };

		{
			auto* xor_value_ptr{ reinterpret_cast<std::uint32_t*>(&xor_key_value) };
			for (auto i{ 0u }; i < bytes_xor_count; i++)
				*(reinterpret_cast<std::uint32_t*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		{
			auto* xor_value_ptr{ reinterpret_cast<std::uint8_t*>(&xor_key_value) };

			for (auto i{ sizeof(T) - bytes_xor_count_8 }; i < sizeof(T); ++i)
				*(reinterpret_cast<std::uint8_t*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		return xored_value;
	}

	void encrypt(T value) noexcept
	{
		if (!xor_key_was_init) {
			if (sizeof(T) <= 2) {
				bytes_xor_count_8 = sizeof(T);
				bytes_xor_count = 0;
			} else {
				bytes_xor_count_8 = sizeof(T) % 4;
				bytes_xor_count = (sizeof(T) - bytes_xor_count_8) / 4;
			}

			auto key{ ::__rdtsc() };
			auto key_index{ 0 };

			for (auto i{ 0u }; i < sizeof(T); i++) {
				*(reinterpret_cast<std::uint8_t*>(&xor_key) + i) = *(reinterpret_cast<std::uint8_t*>(&key) + key_index++);

				if (key_index == 8) {
					key = ::__rdtsc();
					key_index = 0;
				}
			}

			value_index = 0;
			xor_key_was_init = 1;
		}

		auto xored_value{ value };
		auto xor_key_value{ xor_key };

		{
			auto* xor_value_ptr{ reinterpret_cast<std::uint32_t*>(&xor_key_value) };
			for (auto i{ 0u }; i < bytes_xor_count; i++)
				*(reinterpret_cast<std::uint32_t*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		{
			auto* xor_value_ptr{ reinterpret_cast<std::uint8_t*>(&xor_key_value) };

			for (auto i{ sizeof(T) - bytes_xor_count_8 }; i < sizeof(T); ++i)
				*(reinterpret_cast<std::uint8_t*>(&xored_value) + i) ^= ~xor_value_ptr[i];
		}

		auto new_value_index{ std::uint8_t(value_index + 1) & 3 };
		values_table[new_value_index] = xored_value;
		value_index = new_value_index;
	}
};
#pragma pack(pop)
