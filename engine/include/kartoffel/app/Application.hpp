//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#pragma once

#include <string>
#include <memory>

#include <kartoffel/ecs/World.hpp>
#include "Window.hpp"
#include <kartoffel/input/InputManager.hpp>

namespace Kartoffel {

    struct AppConfig {
        int         width         = 1280;
        int         height        = 720;
        std::string title         = "Kartoffel Game";
        bool        capture_mouse = true;
    };

    class Application {
    public:
        explicit Application(AppConfig config = {});
        virtual ~Application() = default;

        // Entry point — call this from main().
        int Run();

        // -----------------------------------------------------------------------
        // Override these in your game
        // -----------------------------------------------------------------------
        virtual void OnInit() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnShutdown() {}

        // -----------------------------------------------------------------------
        // Engine services available to your game
        // -----------------------------------------------------------------------
        World        &GetWorld() { return world_; }
        Window       &GetWindow() { return window_; }
        InputManager &GetInput() { return input_; }

    protected:
        AppConfig    config_;
        World        world_;
        Window       window_;
        InputManager input_;

    private:
        void RegisterCoreComponents();
    };

} // namespace Kartoffel
