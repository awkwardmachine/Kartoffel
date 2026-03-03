//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_COMPONENTS_HPP
#define KARTOFFEL_COMPONENTS_HPP

#include <string>
#include <glm/glm.hpp>

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

#endif // KARTOFFEL_COMPONENTS_HPP
