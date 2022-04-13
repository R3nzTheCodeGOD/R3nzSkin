#pragma once

#include <cstdint>

#include "AString.hpp"
#include "Pad.hpp"

class CharacterStackData {
public:
    AString model;
private:
    PAD(0xC);
public:
    std::int32_t skin;
private:
    PAD(0x40);
};
