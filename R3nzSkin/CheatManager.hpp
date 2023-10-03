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
    CheatManager() noexcept :
        hooks(std::make_unique<Hooks>()),
        config(std::make_unique<Config>()),
        gui(std::make_unique<GUI>()),
        memory(std::make_unique<Memory>()),
        database(std::make_unique<SkinDatabase>()),
        logger(std::make_unique<R3nzSkinLogger>()),
        cheatState(true)
    {}

    

private:
    bool cheatState;
    std::unique_ptr<Hooks> hooks;
    std::unique_ptr<Config> config;
    std::unique_ptr<GUI> gui;
    std::unique_ptr<Memory> memory;
    std::unique_ptr<SkinDatabase> database;
    std::unique_ptr<R3nzSkinLogger> logger;
};

inline CheatManager cheatManager;
