//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_COMPONENTS_HPP
#define KARTOFFEL_COMPONENTS_HPP

#include <functional>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Entity.hpp"

// Forward declaration — ScriptComponent callbacks take World& but we
// cannot include World.hpp here (circular dependency).
class World;

// ---------------------------------------------------------------------------
// TagComponent — human-readable name + optional string tags for an entity.
// ---------------------------------------------------------------------------
struct TagComponent {
    std::string              name;     // primary display name, e.g. "Sun"
    std::vector<std::string> tags;     // arbitrary labels, e.g. {"light","static"}

    // Convenience: check whether this entity carries a given tag label.
    [[nodiscard]] bool HasTag(const std::string &tag) const {
        for (const auto &t : tags)
            if (t == tag) return true;
        return false;
    }
};

// ---------------------------------------------------------------------------
// LightComponent — supports three common light types.
// ---------------------------------------------------------------------------
enum class LightType : int {
    Directional = 0,   // infinite, direction-only (sun, moon)
    Point       = 1,   // omni-directional from a position with falloff
    Spot        = 2,   // cone from a position+direction
};

struct LightComponent {
    LightType type = LightType::Point;

    glm::vec3 color     = {1.0f, 1.0f, 1.0f};
    float     intensity = 1.0f;

    // --- Directional & Spot ---
    glm::vec3 direction = {0.0f, -1.0f, 0.0f};  // world-space; normalised at render time

    // --- Point & Spot attenuation ---
    float constant  = 1.0f;
    float linear    = 0.09f;
    float quadratic = 0.032f;

    // --- Spot cone ---
    float inner_cutoff_deg = 12.5f;  // inner cone angle (degrees)
    float outer_cutoff_deg = 17.5f;  // outer cone angle (degrees)

    bool cast_shadows = false;       // reserved for future shadow-map support
};

struct TransformComponent {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
};

struct MeshComponent {
    unsigned int vao{0};
    unsigned int vbo{0};
    unsigned int ebo{0};
    int          vertex_count{0};
    bool         use_indices{false};
};

struct ModelComponent {
    std::string kasset_path;
    std::string vert_shader = "shaders/triangle.vert";
    std::string frag_shader = "shaders/triangle.frag";
};

struct ShaderComponent {
    unsigned int id{0};
};

struct TextureComponent {
    unsigned int id{0};
    unsigned int slot{0};
};

struct CameraComponent {
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 world_up{0.0f, 1.0f, 0.0f};
    float     yaw{-90.0f};
    float     pitch{0.0f};
    float     movement_speed{2.5f};
    float     mouse_sensitivity{0.1f};
    float     fov{45.0f};
};

// ---------------------------------------------------------------------------
// RigidBodyComponent — linear physics body (no rotation simulation).
// ---------------------------------------------------------------------------
struct RigidBodyComponent {
    glm::vec3 velocity{0.0f};          // metres per second
    glm::vec3 acceleration{0.0f};      // accumulated forces / mass this frame

    float mass        = 1.0f;          // kg  (0 = immovable / static)
    float restitution = 0.3f;          // bounciness [0,1]
    float linear_drag = 0.98f;         // velocity damping per second [0,1]

    bool use_gravity  = true;          // apply world gravity each frame
    bool is_kinematic = false;         // if true: moved by velocity but never pushed by solver

    // Apply an instantaneous force (accumulated until next PhysicsSystem tick)
    void ApplyForce(const glm::vec3 &force) {
        if (mass > 0.0f)
            acceleration += force / mass;
    }

    // Apply an instant velocity change (impulse)
    void ApplyImpulse(const glm::vec3 &impulse) {
        if (mass > 0.0f)
            velocity += impulse / mass;
    }
};

// ---------------------------------------------------------------------------
// BoxColliderComponent — axis-aligned bounding box collider.
// ---------------------------------------------------------------------------

// Collision layers — entities only collide when (a.layer & b.mask) != 0
// and (b.layer & a.mask) != 0.  Default: everything collides with everything.
using CollisionLayer = uint32_t;
static constexpr CollisionLayer LAYER_DEFAULT  = (1u << 0);
static constexpr CollisionLayer LAYER_PLAYER   = (1u << 1);
static constexpr CollisionLayer LAYER_ENEMY    = (1u << 2);
static constexpr CollisionLayer LAYER_STATIC   = (1u << 3);
static constexpr CollisionLayer LAYER_TRIGGER  = (1u << 4);
static constexpr CollisionLayer LAYER_ALL      = ~0u;

struct BoxColliderComponent {
    glm::vec3 half_extents{0.5f, 0.5f, 0.5f};  // half-size in each axis (local space)
    glm::vec3 offset{0.0f};                      // local offset from transform position

    bool           is_trigger     = false;        // trigger: fire callback, skip resolution
    CollisionLayer layer          = LAYER_DEFAULT;
    CollisionLayer mask           = LAYER_ALL;    // which layers this collider cares about

    // Computed world-space AABB (updated each frame by PhysicsSystem)
    glm::vec3 world_min{0.0f};
    glm::vec3 world_max{0.0f};
};

// ---------------------------------------------------------------------------
// CollisionInfo — passed to OnCollision callbacks.
// ---------------------------------------------------------------------------
struct CollisionInfo {
    Entity    other;            // the other entity involved
    glm::vec3 normal;           // collision normal pointing away from 'other'
    float     penetration;      // overlap depth along normal
    bool      is_trigger;       // true if either collider is a trigger
};

// ---------------------------------------------------------------------------
// ScriptComponent — per-entity gameplay logic via std::function callbacks.
//
// Usage (in MyGame::OnInit):
//   world.AddComponent<ScriptComponent>(e, ScriptComponent{
//       .on_init   = [](Entity self, World &w) { ... },
//       .on_update = [](Entity self, World &w, float dt) { ... },
//       .on_collision = [](Entity self, World &w, const CollisionInfo &c) { ... },
//   });
// ---------------------------------------------------------------------------
using ScriptInitFn      = std::function<void(Entity, World &)>;
using ScriptUpdateFn    = std::function<void(Entity, World &, float dt)>;
using ScriptCollisionFn = std::function<void(Entity, World &, const CollisionInfo &)>;

struct ScriptComponent {
    ScriptInitFn      on_init      = nullptr;
    ScriptUpdateFn    on_update    = nullptr;
    ScriptCollisionFn on_collision = nullptr;

    bool initialised = false;   // set to true by ScriptSystem after first on_init call
};

#endif // KARTOFFEL_COMPONENTS_HPP
