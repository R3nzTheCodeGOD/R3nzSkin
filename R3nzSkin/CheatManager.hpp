#pragma once

#include <memory>

#include "Config.hpp"
#include "GUI.hpp"
#include "Hooks.hpp"
#include "Logger.hpp"
#include "Memory.hpp"
#include "SkinDatabase.hpp"

class CheatManager {
public:
	void start() noexcept
	{
		this->hooks = std::make_unique<Hooks>();
		this->config = std::make_unique<Config>();
		this->gui = std::make_unique<GUI>();
		this->memory = std::make_unique<Memory>();
		this->database = std::make_unique<SkinDatabase>();
		this->logger = std::make_unique<R3nzSkinLogger>();
	}

	bool cheatState{ true };
	std::unique_ptr<Hooks> hooks;
	std::unique_ptr<Config> config;
	std::unique_ptr<GUI> gui;
	std::unique_ptr<Memory> memory;
	std::unique_ptr<SkinDatabase> database;
	std::unique_ptr<R3nzSkinLogger> logger;
};

inline CheatManager cheatManager;
