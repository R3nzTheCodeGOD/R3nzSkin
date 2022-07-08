#pragma once

#include <cstddef>

#include "Pad.hpp"

template <class T>
class ManagerTemplate {
	PAD(0x4)
	T** list;
	std::size_t length;
	std::size_t capacity;
};
