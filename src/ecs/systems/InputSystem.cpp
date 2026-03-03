//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "InputSystem.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "World.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

InputSystem::InputSystem(InputManager *input, Window *window) : input_(input), window_(window) {}

void InputSystem::Update(World &world, float dt) {
    input_->Update();

    if (input_->IsKeyPressed(GLFW_KEY_ESCAPE))
        window_->SetShouldClose(true);

    const auto cameras = world.Query<CameraComponent>();
    if (cameras.empty())
        return;

    auto &cam = world.GetComponent<CameraComponent>(cameras[0]);

    if (input_->IsKeyPressed(GLFW_KEY_W))
        cam.position += cam.front * 0.015f * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_S))
        cam.position -= cam.front * 0.015f * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_A))
        cam.position -= cam.right * 0.015f * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_D))
        cam.position += cam.right * 0.015f * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_SPACE))
        cam.position += cam.world_up * 0.015f * cam.movement_speed;
    if (input_->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
        cam.position -= cam.world_up * 0.015f * cam.movement_speed;

    if (!window_->GetMouseCursorEnabled()) {
        const float yaw_offset   = static_cast<float>(input_->GetMouseDeltaX()) * cam.mouse_sensitivity;
        const float pitch_offset = static_cast<float>(input_->GetMouseDeltaY()) * cam.mouse_sensitivity;

        cam.yaw += yaw_offset;
        cam.pitch += pitch_offset;
        if (cam.pitch > 89.0f)
            cam.pitch = 89.0f;
        if (cam.pitch < -89.0f)
            cam.pitch = -89.0f;

        UpdateCameraVectors(cam);
    }
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
