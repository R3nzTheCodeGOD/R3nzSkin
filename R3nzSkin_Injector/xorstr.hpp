#pragma once
#include <array>

namespace XorCompileTime
{
    constexpr auto time = __TIME__;
    constexpr auto seed = static_cast<int>(time[7]) + static_cast<int>(time[6]) * 10 + static_cast<int>(time[4]) * 60 + static_cast<int>(time[3]) * 600 + static_cast<int>(time[1]) * 3600 + static_cast<int>(time[0]) * 36000;

    template<int N>
    struct RandomGenerator
    {
    private:
        static constexpr unsigned a = 0x41a7; // 7^5
        static constexpr unsigned m = 0x7fffffff; // 2^31 - 1

        static constexpr unsigned s = RandomGenerator< N - 1 >::value;
        static constexpr unsigned lo = a * (s & 0xFFFF); // Multiply lower 16 bits by 16807
        static constexpr unsigned hi = a * (s >> 16); // Multiply higher 16 bits by 16807
        static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16); // Combine lower 15 bits of hi with lo's upper bits
        static constexpr unsigned hi2 = hi >> 15; // Discard lower 15 bits of hi
        static constexpr unsigned lo3 = lo2 + hi;

    public:
        static constexpr unsigned max = m;
        static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
    };

    template<>
    struct RandomGenerator<0>
    {
        static constexpr unsigned value = seed;
    };

    template<int N, int M>
    struct RandomInt
    {
        static constexpr auto value = RandomGenerator<N + 1>::value % M;
    };

    template<int N>
    struct RandomChar
    {
        static const char value = static_cast<char>(1 + RandomInt<N, 0x7F - 1>::value);
    };

    template<size_t N, int K, typename Char>
    struct XorString
    {
    private:
        const char _key;
        std::array<Char, N + 1> _encrypted;

        constexpr Char enc(Char c) const
        {
            return c ^ _key;
        }

        Char dec(Char c) const
        {
            return c ^ _key;
        }

    public:
        template<size_t... Is>
        constexpr __forceinline XorString(const Char* str, std::index_sequence< Is... >) : _key(RandomChar<K>::value), _encrypted{ enc(str[Is])... } {}

        __forceinline decltype(auto) decrypt()
        {
            for (size_t i = 0; i < N; ++i) {
                _encrypted[i] = dec(_encrypted[i]);
            }
            _encrypted[N] = '\0';
            return _encrypted.data();
        }
    };
	#define _XorStr(s) []{ constexpr XorCompileTime::XorString<sizeof(s)/sizeof(char) - 1, __COUNTER__, char> expr(s, std::make_index_sequence<sizeof(s)/sizeof(char) - 1>()); return expr; }().decrypt()
	#define _XorStrW(s) []{ constexpr XorCompileTime::XorString<sizeof(s)/sizeof(wchar_t) - 1, __COUNTER__, wchar_t> expr(s, std::make_index_sequence<sizeof(s)/sizeof(wchar_t) - 1>()); return expr; }().decrypt()
}