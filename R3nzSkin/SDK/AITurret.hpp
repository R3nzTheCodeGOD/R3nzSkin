#pragma once

#include "AIBaseCommon.hpp"
#include "Vector.hpp"

class AITurret : public AIBaseCommon {
public:
	[[nodiscard]] inline Vector getTurretPosition() const noexcept { return this->getPos() + 7.0f; }
	[[nodiscard]] inline float getTurretRange() const noexcept { return 810.0f; }
};
