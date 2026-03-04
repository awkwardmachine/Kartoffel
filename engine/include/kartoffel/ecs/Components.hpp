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

class World;

struct TagComponent {
    std::string              name;
    std::vector<std::string> tags;

    [[nodiscard]] bool HasTag(const std::string &tag) const {
        for (const auto &t: tags)
            if (t == tag)
                return true;
        return false;
    }
};

enum class LightType : int {
    Directional = 0,
    Point       = 1,
    Spot        = 2,
};

struct LightComponent {
    LightType type = LightType::Point;

    glm::vec3 color     = {1.0f, 1.0f, 1.0f};
    float     intensity = 1.0f;

    glm::vec3 direction = {0.0f, -1.0f, 0.0f};

    float constant  = 1.0f;
    float linear    = 0.09f;
    float quadratic = 0.032f;

    float inner_cutoff_deg = 12.5f;
    float outer_cutoff_deg = 17.5f;

    bool cast_shadows = false;
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

struct RigidBodyComponent {
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};

    float mass        = 1.0f;
    float restitution = 0.3f;
    float linear_drag = 0.98f;

    bool use_gravity  = true;
    bool is_kinematic = false;

    void ApplyForce(const glm::vec3 &force) {
        if (mass > 0.0f)
            acceleration += force / mass;
    }

    void ApplyImpulse(const glm::vec3 &impulse) {
        if (mass > 0.0f)
            velocity += impulse / mass;
    }
};

using CollisionLayer                          = uint32_t;
static constexpr CollisionLayer LAYER_DEFAULT = (1u << 0);
static constexpr CollisionLayer LAYER_PLAYER  = (1u << 1);
static constexpr CollisionLayer LAYER_ENEMY   = (1u << 2);
static constexpr CollisionLayer LAYER_STATIC  = (1u << 3);
static constexpr CollisionLayer LAYER_TRIGGER = (1u << 4);
static constexpr CollisionLayer LAYER_ALL     = ~0u;

struct BoxColliderComponent {
    glm::vec3 half_extents{0.5f, 0.5f, 0.5f};
    glm::vec3 offset{0.0f};

    bool           is_trigger = false;
    CollisionLayer layer      = LAYER_DEFAULT;
    CollisionLayer mask       = LAYER_ALL;

    glm::vec3 world_min{0.0f};
    glm::vec3 world_max{0.0f};
};

struct CollisionInfo {
    Entity    other;
    glm::vec3 normal;
    float     penetration;
    bool      is_trigger;
};

using ScriptInitFn      = std::function<void(Entity, World &)>;
using ScriptUpdateFn    = std::function<void(Entity, World &, float dt)>;
using ScriptCollisionFn = std::function<void(Entity, World &, const CollisionInfo &)>;

struct ScriptComponent {
    ScriptInitFn      on_init      = nullptr;
    ScriptUpdateFn    on_update    = nullptr;
    ScriptCollisionFn on_collision = nullptr;

    bool initialised = false;
};

#endif // KARTOFFEL_COMPONENTS_HPP
