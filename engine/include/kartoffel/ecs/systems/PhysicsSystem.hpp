//
// PhysicsSystem.hpp
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//
// Responsibilities (executed in order each tick):
//   1. Update world-space AABBs from TransformComponent + BoxColliderComponent
//   2. Integrate RigidBodyComponent velocity (gravity, drag, user forces)
//   3. Broad-phase: collect candidate pairs via sweep-and-prune on X axis
//   4. Narrow-phase: AABB vs AABB overlap test + penetration vector
//   5. Resolution: positional correction + velocity reflection for non-triggers
//   6. Dispatch CollisionInfo to ScriptComponent::on_collision callbacks
//

#ifndef KARTOFFEL_PHYSICSSYSTEM_HPP
#define KARTOFFEL_PHYSICSSYSTEM_HPP

#include <vector>
#include <utility>

#include <glm/glm.hpp>

#include "kartoffel/ecs/System.hpp"
#include "kartoffel/ecs/Entity.hpp"
#include "kartoffel/ecs/Components.hpp"

class PhysicsSystem : public System {
public:
    // gravity — world-space acceleration applied to every RigidBody with use_gravity=true
    explicit PhysicsSystem(glm::vec3 gravity = {0.0f, -9.81f, 0.0f});

    void Update(World &world, float dt) override;

    // Expose gravity so game code can tweak it at runtime (e.g. moon level)
    glm::vec3 gravity{0.0f, -9.81f, 0.0f};

private:
    // Step 1 — recompute world-space AABBs
    static void UpdateAABBs(World &world);

    // Step 2 — integrate velocities & positions
    void Integrate(World &world, float dt) const;

    // Steps 3-6 — detect + resolve collisions
    static void DetectAndResolve(World &world);

    // Broad-phase: returns candidate pairs (sorted by X min already)
    static std::vector<std::pair<Entity, Entity>> BroadPhase(World &world);

    // Narrow-phase: returns true + fills out CollisionInfo if overlapping
    static bool NarrowPhase(const BoxColliderComponent &a,
                             const BoxColliderComponent &b,
                             CollisionInfo              &out_a,
                             CollisionInfo              &out_b);

    // Resolution: push apart + reflect velocity
    static void Resolve(World        &world,
                        Entity        ea,
                        Entity        eb,
                        const CollisionInfo &info_a);
};

#endif // KARTOFFEL_PHYSICSSYSTEM_HPP
