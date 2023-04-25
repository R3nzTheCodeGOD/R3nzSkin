#pragma once

#include <cstdint>

template <class T>
class RiotArray {
public:
	T* list;
	std::int32_t size;
	std::int32_t cap;
};
