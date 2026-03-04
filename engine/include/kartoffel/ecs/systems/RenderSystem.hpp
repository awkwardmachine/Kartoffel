//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_RENDERSYSTEM_HPP
#define KARTOFFEL_RENDERSYSTEM_HPP

#include "ecs/System.hpp"

class Window;
class World;

class RenderSystem : public System {
public:
    explicit RenderSystem(Window *window);

    void Update(World &world, float dt) override;

private:
    Window *window_;
};

#endif // KARTOFFEL_RENDERSYSTEM_HPP
