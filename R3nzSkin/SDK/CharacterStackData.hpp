#pragma once

#include <cstdint>

#include "AString.hpp"
#include "Pad.hpp"

class CharacterStackData {
public:
    AString model;
    PAD(0xC)
    std::int32_t skin;
    PAD(0x4A)
};
