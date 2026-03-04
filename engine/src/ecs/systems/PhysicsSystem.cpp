//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/ecs/systems/PhysicsSystem.hpp"

#include <algorithm>
#include <cmath>

#include <glm/glm.hpp>

#include "kartoffel/ecs/Components.hpp"
#include "kartoffel/ecs/World.hpp"

PhysicsSystem::PhysicsSystem(glm::vec3 g) : gravity(g) {}

void PhysicsSystem::Update(World &world, const float dt) {
    UpdateAABBs(world);
    UpdateSphereBounds(world);
    Integrate(world, dt);
    UpdateAABBs(world);
    UpdateSphereBounds(world);
    DetectAndResolve(world);
}

void PhysicsSystem::UpdateAABBs(World &world) {
    for (const Entity e: world.Query<TransformComponent, BoxColliderComponent>()) {
        const auto &tf  = world.GetComponent<TransformComponent>(e);
        auto       &col = world.GetComponent<BoxColliderComponent>(e);

        const glm::vec3 world_center = tf.position + col.offset * tf.scale;
        const glm::vec3 world_half   = col.half_extents * glm::abs(tf.scale);

        col.world_min = world_center - world_half;
        col.world_max = world_center + world_half;
    }
}

void PhysicsSystem::UpdateSphereBounds(World &world) {
    for (const Entity e: world.Query<TransformComponent, SphereColliderComponent>()) {
        const auto &tf  = world.GetComponent<TransformComponent>(e);
        auto       &col = world.GetComponent<SphereColliderComponent>(e);

        const float max_scale = std::max({tf.scale.x, tf.scale.y, tf.scale.z});
        col.world_center      = tf.position + col.offset * max_scale;
    }
}

void PhysicsSystem::Integrate(World &world, const float dt) const {
    for (const Entity e: world.Query<TransformComponent, RigidBodyComponent>()) {
        auto &tf = world.GetComponent<TransformComponent>(e);
        auto &rb = world.GetComponent<RigidBodyComponent>(e);

        if (rb.mass <= 0.0f)
            continue;

        if (rb.use_gravity)
            rb.acceleration += gravity;

        rb.velocity += rb.acceleration * dt;
        rb.velocity *= std::pow(rb.linear_drag, dt);
        tf.position += rb.velocity * dt;
        rb.acceleration = glm::vec3{0.0f};
    }
}

std::vector<std::pair<Entity, Entity>> PhysicsSystem::BroadPhase(World &world) {
    struct Proxy {
        Entity         entity;
        float          min_x;
        float          max_x;
        CollisionLayer layer;
        CollisionLayer mask;
    };

    std::vector<Proxy> proxies;

    for (const Entity e: world.Query<BoxColliderComponent>()) {
        const auto &col = world.GetComponent<BoxColliderComponent>(e);
        proxies.push_back({e, col.world_min.x, col.world_max.x, col.layer, col.mask});
    }

    for (const Entity e: world.Query<SphereColliderComponent>()) {
        const auto &col   = world.GetComponent<SphereColliderComponent>(e);
        const auto &tf    = world.GetComponent<TransformComponent>(e);
        const float max_s = std::max({tf.scale.x, tf.scale.y, tf.scale.z});
        const float r     = col.radius * max_s;
        proxies.push_back({e, col.world_center.x - r, col.world_center.x + r, col.layer, col.mask});
    }

    std::ranges::sort(proxies, [](const Proxy &a, const Proxy &b) { return a.min_x < b.min_x; });

    std::vector<std::pair<Entity, Entity>> pairs;
    for (size_t i = 0; i < proxies.size(); ++i) {
        for (size_t j = i + 1; j < proxies.size(); ++j) {
            if (proxies[j].min_x > proxies[i].max_x)
                break;

            const Entity ea = proxies[i].entity;
            const Entity eb = proxies[j].entity;

            if (!(proxies[i].layer & proxies[j].mask) || !(proxies[j].layer & proxies[i].mask))
                continue;

            pairs.emplace_back(ea, eb);
        }
    }
    return pairs;
}

bool PhysicsSystem::NarrowPhase(const BoxColliderComponent &a, const BoxColliderComponent &b, CollisionInfo &out_a,
                                CollisionInfo &out_b) {
    if (a.world_max.x < b.world_min.x || a.world_min.x > b.world_max.x)
        return false;
    if (a.world_max.y < b.world_min.y || a.world_min.y > b.world_max.y)
        return false;
    if (a.world_max.z < b.world_min.z || a.world_min.z > b.world_max.z)
        return false;

    const float ox = std::min(a.world_max.x, b.world_max.x) - std::max(a.world_min.x, b.world_min.x);
    const float oy = std::min(a.world_max.y, b.world_max.y) - std::max(a.world_min.y, b.world_min.y);
    const float oz = std::min(a.world_max.z, b.world_max.z) - std::max(a.world_min.z, b.world_min.z);

    const glm::vec3 ca = (a.world_min + a.world_max) * 0.5f;
    const glm::vec3 cb = (b.world_min + b.world_max) * 0.5f;
    const glm::vec3 d  = ca - cb;

    glm::vec3 normal;
    float     penetration;

    if (ox <= oy && ox <= oz) {
        penetration = ox;
        normal      = {d.x < 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f};
    } else if (oy <= ox && oy <= oz) {
        penetration = oy;
        normal      = {0.0f, d.y < 0.0f ? -1.0f : 1.0f, 0.0f};
    } else {
        penetration = oz;
        normal      = {0.0f, 0.0f, d.z < 0.0f ? -1.0f : 1.0f};
    }

    const bool trigger = a.is_trigger || b.is_trigger;
    out_a              = {NULL_ENTITY, normal, penetration, trigger};
    out_b              = {NULL_ENTITY, -normal, penetration, trigger};
    return true;
}

bool PhysicsSystem::NarrowPhase(const SphereColliderComponent &a, const SphereColliderComponent &b,
                                CollisionInfo &out_a, CollisionInfo &out_b) {
    const float dist_sq   = glm::dot(b.world_center - a.world_center, b.world_center - a.world_center);
    const float radii_sum = a.radius + b.radius;

    if (dist_sq >= radii_sum * radii_sum)
        return false;

    const float dist = std::sqrt(dist_sq);
    glm::vec3   normal;
    if (dist < 1e-6f)
        normal = {0.0f, 1.0f, 0.0f};
    else
        normal = (a.world_center - b.world_center) / dist;

    const float penetration = radii_sum - dist;
    const bool  trigger     = a.is_trigger || b.is_trigger;

    out_a = {NULL_ENTITY, normal, penetration, trigger};
    out_b = {NULL_ENTITY, -normal, penetration, trigger};
    return true;
}

bool PhysicsSystem::NarrowPhase(const SphereColliderComponent &a, const BoxColliderComponent &b, CollisionInfo &out_a,
                                CollisionInfo &out_b) {
    const glm::vec3 closest = glm::clamp(a.world_center, b.world_min, b.world_max);
    const glm::vec3 delta   = a.world_center - closest;
    const float     dist_sq = glm::dot(delta, delta);

    if (dist_sq >= a.radius * a.radius)
        return false;

    const float dist = std::sqrt(dist_sq);
    glm::vec3   normal;
    if (dist < 1e-6f) {
        const glm::vec3 box_center = (b.world_min + b.world_max) * 0.5f;
        const glm::vec3 d          = a.world_center - box_center;
        const glm::vec3 half       = (b.world_max - b.world_min) * 0.5f;
        if (const glm::vec3 overlap = half - glm::abs(d); overlap.x <= overlap.y && overlap.x <= overlap.z)
            normal = {d.x < 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f};
        else if (overlap.y <= overlap.x && overlap.y <= overlap.z)
            normal = {0.0f, d.y < 0.0f ? -1.0f : 1.0f, 0.0f};
        else
            normal = {0.0f, 0.0f, d.z < 0.0f ? -1.0f : 1.0f};
    } else {
        normal = delta / dist;
    }

    const float penetration = a.radius - dist;
    const bool  trigger     = a.is_trigger || b.is_trigger;

    out_a = {NULL_ENTITY, normal, penetration, trigger};
    out_b = {NULL_ENTITY, -normal, penetration, trigger};
    return true;
}

void PhysicsSystem::Resolve(World &world, const Entity ea, const Entity eb, const CollisionInfo &info_a) {
    if (!info_a.is_trigger) {
        const bool a_movable = world.HasComponent<RigidBodyComponent>(ea) &&
                               world.GetComponent<RigidBodyComponent>(ea).mass > 0.0f &&
                               !world.GetComponent<RigidBodyComponent>(ea).is_kinematic;
        const bool b_movable = world.HasComponent<RigidBodyComponent>(eb) &&
                               world.GetComponent<RigidBodyComponent>(eb).mass > 0.0f &&
                               !world.GetComponent<RigidBodyComponent>(eb).is_kinematic;

        const float total_inv_mass = (a_movable ? 1.0f / world.GetComponent<RigidBodyComponent>(ea).mass : 0.0f) +
                                     (b_movable ? 1.0f / world.GetComponent<RigidBodyComponent>(eb).mass : 0.0f);

        if (total_inv_mass > 0.0f) {
            constexpr float slop       = 0.005f;
            constexpr float correction = 0.8f;
            const float     depth      = std::max(info_a.penetration - slop, 0.0f);
            const glm::vec3 push       = depth / total_inv_mass * correction * info_a.normal;

            if (a_movable) {
                auto       &tf  = world.GetComponent<TransformComponent>(ea);
                const float inv = 1.0f / world.GetComponent<RigidBodyComponent>(ea).mass;
                tf.position += push * inv;
            }
            if (b_movable) {
                auto       &tf  = world.GetComponent<TransformComponent>(eb);
                const float inv = 1.0f / world.GetComponent<RigidBodyComponent>(eb).mass;
                tf.position -= push * inv;
            }

            const glm::vec3 &n  = info_a.normal;
            const glm::vec3  va = a_movable ? world.GetComponent<RigidBodyComponent>(ea).velocity : glm::vec3{0.0f};
            const glm::vec3  vb = b_movable ? world.GetComponent<RigidBodyComponent>(eb).velocity : glm::vec3{0.0f};
            const glm::vec3  rv = va - vb;

            const float vel_along_normal = glm::dot(rv, n);
            if (vel_along_normal > 0.0f)
                return;

            const float e = (a_movable ? world.GetComponent<RigidBodyComponent>(ea).restitution : 0.0f) *
                            (b_movable ? world.GetComponent<RigidBodyComponent>(eb).restitution : 1.0f);

            const float j = -(1.0f + e) * vel_along_normal / total_inv_mass;

            if (a_movable) {
                auto &rb = world.GetComponent<RigidBodyComponent>(ea);
                rb.velocity += (j / rb.mass) * n;
            }
            if (b_movable) {
                auto &rb = world.GetComponent<RigidBodyComponent>(eb);
                rb.velocity -= (j / rb.mass) * n;
            }
        }
    }

    CollisionInfo for_a = info_a;
    for_a.other         = eb;
    CollisionInfo for_b = info_a;
    for_b.normal        = -info_a.normal;
    for_b.other         = ea;

    if (world.HasComponent<ScriptComponent>(ea))
        if (const auto &sc = world.GetComponent<ScriptComponent>(ea); sc.on_collision)
            sc.on_collision(ea, world, for_a);

    if (world.HasComponent<ScriptComponent>(eb))
        if (const auto &sc = world.GetComponent<ScriptComponent>(eb); sc.on_collision)
            sc.on_collision(eb, world, for_b);
}

void PhysicsSystem::DetectAndResolve(World &world) {

    for (const auto pairs = BroadPhase(world); const auto &[ea, eb]: pairs) {
        const bool ea_box    = world.HasComponent<BoxColliderComponent>(ea);
        const bool eb_box    = world.HasComponent<BoxColliderComponent>(eb);
        const bool ea_sphere = world.HasComponent<SphereColliderComponent>(ea);
        const bool eb_sphere = world.HasComponent<SphereColliderComponent>(eb);

        CollisionInfo info_a{}, info_b{};
        bool          hit = false;

        if (ea_box && eb_box) {
            const auto &ca = world.GetComponent<BoxColliderComponent>(ea);
            const auto &cb = world.GetComponent<BoxColliderComponent>(eb);
            hit            = NarrowPhase(ca, cb, info_a, info_b);

        } else if (ea_sphere && eb_sphere) {
            auto        ca  = world.GetComponent<SphereColliderComponent>(ea);
            auto        cb  = world.GetComponent<SphereColliderComponent>(eb);
            const auto &tfa = world.GetComponent<TransformComponent>(ea);
            const auto &tfb = world.GetComponent<TransformComponent>(eb);
            ca.radius *= std::max({tfa.scale.x, tfa.scale.y, tfa.scale.z});
            cb.radius *= std::max({tfb.scale.x, tfb.scale.y, tfb.scale.z});
            hit = NarrowPhase(ca, cb, info_a, info_b);

        } else if (ea_sphere && eb_box) {
            auto        ca  = world.GetComponent<SphereColliderComponent>(ea);
            const auto &tfa = world.GetComponent<TransformComponent>(ea);
            ca.radius *= std::max({tfa.scale.x, tfa.scale.y, tfa.scale.z});
            hit = NarrowPhase(ca, world.GetComponent<BoxColliderComponent>(eb), info_a, info_b);

        } else if (ea_box && eb_sphere) {
            auto        cb  = world.GetComponent<SphereColliderComponent>(eb);
            const auto &tfb = world.GetComponent<TransformComponent>(eb);
            cb.radius *= std::max({tfb.scale.x, tfb.scale.y, tfb.scale.z});
            hit = NarrowPhase(cb, world.GetComponent<BoxColliderComponent>(ea), info_b, info_a);
        }

        if (hit)
            Resolve(world, ea, eb, info_a);
    }
}
