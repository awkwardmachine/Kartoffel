//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_INPUTSYSTEM_HPP
#define KARTOFFEL_INPUTSYSTEM_HPP

#include <string>

#include "ecs/Components.hpp"
#include "ecs/System.hpp"

class InputManager;
class Window;
class World;

class InputSystem : public System {
public:
    InputSystem(InputManager *input, Window *window, std::string scene_path);

    void Update(World &world, float dt) override;

private:
    InputManager *input_;
    Window       *window_;
    std::string   scene_path_;

    bool f5_was_pressed_ = false;
    bool r_was_pressed_  = false;

    static void UpdateCameraVectors(CameraComponent &cam);

    void HotReloadScene(World &world) const;
};

#endif // KARTOFFEL_INPUTSYSTEM_HPP
