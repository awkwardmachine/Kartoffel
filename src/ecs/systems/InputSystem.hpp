//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_INPUTSYSTEM_HPP
#define KARTOFFEL_INPUTSYSTEM_HPP

#include "Components.hpp"
#include "System.hpp"

class InputManager;
class Window;
class World;

class InputSystem : public System {
public:
    InputSystem(InputManager *input, Window *window);

    void Update(World &world, float dt) override;

private:
    InputManager *input_;
    Window       *window_;

    static void UpdateCameraVectors(CameraComponent &cam);
};

#endif // KARTOFFEL_INPUTSYSTEM_HPP
