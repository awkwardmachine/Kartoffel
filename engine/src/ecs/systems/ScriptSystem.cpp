//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/ecs/systems/ScriptSystem.hpp"

#include "kartoffel/ecs/Components.hpp"
#include "kartoffel/ecs/World.hpp"

void ScriptSystem::Update(World &world, const float dt) {
    for (const Entity e : world.Query<ScriptComponent>()) {
        auto &sc = world.GetComponent<ScriptComponent>(e);

        if (!sc.initialised) {
            if (sc.on_init) sc.on_init(e, world);
            sc.initialised = true;
        }

        if (sc.on_update) sc.on_update(e, world, dt);
    }
}
