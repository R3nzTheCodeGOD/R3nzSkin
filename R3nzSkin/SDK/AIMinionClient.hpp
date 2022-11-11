#pragma once

#include "AIBaseCommon.hpp"

class AIMinionClient : public AIBaseCommon {
public:
	[[nodiscard]] AIBaseCommon* getGoldRedirectTarget() noexcept;
	[[nodiscard]] bool isLaneMinion() noexcept;
};
