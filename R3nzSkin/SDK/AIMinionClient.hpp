#pragma once

#include "AIBaseCommon.hpp"

class AIMinionClient : public AIBaseCommon {
public:
	[[nodiscard]] AIBaseCommon* get_gold_redirect_target() noexcept;
	[[nodiscard]] bool is_lane_minion() noexcept;
};
