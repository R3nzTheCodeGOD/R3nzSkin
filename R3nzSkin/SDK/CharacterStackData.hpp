#pragma once

#include <cstdint>

#include "AString.hpp"
#include "Pad.hpp"

class CharacterStackData {
public:
    AString model;
    PAD(0xC)
    std::int32_t skin;
    PAD(0x44)
    std::int8_t gear;
    PAD(0x7)
};
