#pragma once

#include <cstdint>

#include "AString.hpp"
#include "Pad.hpp"

class CharacterStackData {
public:
    AString model;
    PAD(0x10)
    std::int32_t skin;
    PAD(0x60)
    std::int8_t gear;
    PAD(0x7)
};
