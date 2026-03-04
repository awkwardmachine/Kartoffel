//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
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
    explicit PhysicsSystem(glm::vec3 gravity = {0.0f, -9.81f, 0.0f});

    void Update(World &world, float dt) override;

    glm::vec3 gravity{0.0f, -9.81f, 0.0f};

private:
    static void UpdateAABBs(World &world);
    static void UpdateSphereBounds(World &world);
    void        Integrate(World &world, float dt) const;

    static void DetectAndResolve(World &world);

    static std::vector<std::pair<Entity, Entity>> BroadPhase(World &world);

    // Box vs Box
    static bool NarrowPhase(const BoxColliderComponent    &a,
                             const BoxColliderComponent    &b,
                             CollisionInfo &out_a, CollisionInfo &out_b);

    // Sphere vs Sphere
    static bool NarrowPhase(const SphereColliderComponent &a,
                             const SphereColliderComponent &b,
                             CollisionInfo &out_a, CollisionInfo &out_b);

    // Sphere vs Box  (a = sphere, b = box)
    static bool NarrowPhase(const SphereColliderComponent &a,
                             const BoxColliderComponent    &b,
                             CollisionInfo &out_a, CollisionInfo &out_b);

    static void Resolve(World &world, Entity ea, Entity eb, const CollisionInfo &info_a);
};

#endif // KARTOFFEL_PHYSICSSYSTEM_HPP
