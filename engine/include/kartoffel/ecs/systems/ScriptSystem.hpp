//
// ScriptSystem.hpp
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//
// Drives ScriptComponent lifecycle:
//   • Calls on_init once per entity on the first frame it is seen.
//   • Calls on_update every frame for all entities with a ScriptComponent.
//
// Collision callbacks (on_collision) are dispatched by PhysicsSystem
// directly at resolution time, so they are not called here.
//

#ifndef KARTOFFEL_SCRIPTSYSTEM_HPP
#define KARTOFFEL_SCRIPTSYSTEM_HPP

#include "kartoffel/ecs/System.hpp"

class ScriptSystem : public System {
public:
    void Update(World &world, float dt) override;
};

#endif // KARTOFFEL_SCRIPTSYSTEM_HPP
