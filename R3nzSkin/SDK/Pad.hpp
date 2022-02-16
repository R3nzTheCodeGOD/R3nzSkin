#pragma once

#include <cstddef>

#define __CONCAT__(a, b) a##b
#define __PAD_NAME__(n) __CONCAT__(pad, n)
#define PAD(size) std::byte __PAD_NAME__(__LINE__) [size];
