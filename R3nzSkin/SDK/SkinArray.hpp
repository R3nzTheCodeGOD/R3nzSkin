#pragma once

#include <cstdlib>

template <class T>
class SkinArray {
public:
	T* list;
	std::size_t size;
	std::size_t cap;
};
