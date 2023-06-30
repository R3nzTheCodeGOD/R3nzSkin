#pragma once

#include <cstdint>

#include "Pad.hpp"

template <class T>
class ManagerTemplate {
	PAD(0x8)
	T** list;
	std::int32_t length;
	std::int32_t capacity;
};
