//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/ecs/systems/InputSystem.hpp"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "kartoffel/ecs/World.hpp"
#include "kartoffel/input/InputManager.hpp"
#include "kartoffel/app/Window.hpp"
#include "kartoffel/assets/SceneLoader.hpp"
#include "kartoffel/assets/AssetManager.hpp"

InputSystem::InputSystem(InputManager *input, Window *window, std::string scene_path) :
    input_(input), window_(window), scene_path_(std::move(scene_path)) {}

void InputSystem::Update(World &world, const float dt) {
    input_->Update();

    if (input_->IsKeyPressed(GLFW_KEY_ESCAPE))
        window_->SetShouldClose(true);

    // Save current scene to disk
    const bool f5_now = input_->IsKeyPressed(GLFW_KEY_F5);
    if (f5_now && !f5_was_pressed_) {
        std::cout << "[InputSystem] Saving scene...\n";
        SceneLoader::Save(scene_path_, world);
    }
    f5_was_pressed_ = f5_now;

    // Hot-reload scene from disk
    const bool r_now = input_->IsKeyPressed(GLFW_KEY_R);
    if (r_now && !r_was_pressed_) {
        std::cout << "[InputSystem] Hot-reloading scene: " << scene_path_ << "\n";
        HotReloadScene(world);
    }
    r_was_pressed_ = r_now;

    // Camera movement
    const auto cameras = world.Query<CameraComponent>();
    if (cameras.empty())
        return;

    auto &cam = world.GetComponent<CameraComponent>(cameras[0]);

    if (input_->IsKeyPressed(GLFW_KEY_W))
        cam.position += cam.front * dt * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_S))
        cam.position -= cam.front * dt * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_A))
        cam.position -= cam.right * dt * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_D))
        cam.position += cam.right * dt * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_SPACE))
        cam.position += cam.world_up * dt * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
        cam.position -= cam.world_up * dt * cam.movement_speed;

    if (!window_->GetMouseCursorEnabled()) {
        const float yaw_offset   = static_cast<float>(input_->GetMouseDeltaX()) * cam.mouse_sensitivity;
        const float pitch_offset = static_cast<float>(input_->GetMouseDeltaY()) * cam.mouse_sensitivity;

        cam.yaw += yaw_offset;
        cam.pitch = glm::clamp(cam.pitch + pitch_offset, -89.0f, 89.0f);

        UpdateCameraVectors(cam);
    }
}

void InputSystem::HotReloadScene(World &world) const {
    std::vector<Entity> to_destroy;
    for (const Entity e: world.Entities()) {
        if (!world.HasComponent<CameraComponent>(e))
            to_destroy.push_back(e);
    }
    for (const Entity e: to_destroy)
        world.DestroyEntity(e);

    AssetManager::EvictAll();

    if (SceneLoader::Load(scene_path_, world))
        std::cout << "[InputSystem] Hot-reload complete." << std::endl;
    else
        std::cerr << "[InputSystem] Hot-reload FAILED, scene file may be missing or malformed.\n" << std::endl;
}

void InputSystem::UpdateCameraVectors(CameraComponent &cam) {
    glm::vec3 front;
    front.x   = glm::cos(glm::radians(cam.yaw)) * glm::cos(glm::radians(cam.pitch));
    front.y   = glm::sin(glm::radians(cam.pitch));
    front.z   = glm::sin(glm::radians(cam.yaw)) * glm::cos(glm::radians(cam.pitch));
    cam.front = glm::normalize(front);
    cam.right = glm::normalize(glm::cross(cam.front, cam.world_up));
    cam.up    = glm::normalize(glm::cross(cam.right, cam.front));
}
