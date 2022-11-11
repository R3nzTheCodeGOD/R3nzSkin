#pragma once

#include <cstdlib>

template <class T>
class RiotArray {
public:
	T* list;
	std::size_t size;
	std::size_t cap;
};
