//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include <glad/glad.h>
#include <iostream>

#include "Camera.hpp"
#include "InputManager.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Window.hpp"

int main() {
    Window       window;
    Shader       shader;
    Mesh         quad;
    Camera       camera;
    InputManager input;
    Texture      texture;

    if (!window.Initialize(800, 600, "Kartoffel")) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    if (Renderer renderer; !renderer.Initialize(window.GetNativeWindow())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    if (!shader.LoadFromFile("shaders/triangle.vert", "shaders/triangle.frag")) {
        std::cerr << "Failed to load shaders" << std::endl;
        return -1;
    }

    if (!quad.CreateCube()) {
        std::cerr << "Failed to create cube" << std::endl;
        return -1;
    }

    if (!texture.Load("textures/stone.png", TextureFilter::Nearest)) {
        std::cerr << "Failed to load texture" << std::endl;
        return -1;
    }

    shader.Use();
    glUniform1i(glGetUniformLocation(shader.GetId(), "uTexture"), 0);

    input.Initialize(window.GetNativeWindow());
    window.SetMouseCursorEnabled(false);
    camera.SetPosition(0.0f, 0.0f, 3.0f);
    auto model = glm::mat4(1.0f);

    while (!window.ShouldClose()) {
        Window::PollEvents();
        input.Update();

        if (input.IsKeyPressed(GLFW_KEY_ESCAPE))
            break;

        if (input.IsKeyPressed(GLFW_KEY_W))
            camera.Move(0.015f, 0.0f, 0.0f);
        if (input.IsKeyPressed(GLFW_KEY_S))
            camera.Move(-0.015f, 0.0f, 0.0f);
        if (input.IsKeyPressed(GLFW_KEY_A))
            camera.Move(0.0f, -0.015f, 0.0f);
        if (input.IsKeyPressed(GLFW_KEY_D))
            camera.Move(0.0f, 0.015f, 0.0f);
        if (input.IsKeyPressed(GLFW_KEY_SPACE))
            camera.Move(0.0f, 0.0f, 0.015f);
        if (input.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
            camera.Move(0.0f, 0.0f, -0.015f);

        if (!window.GetMouseCursorEnabled())
            camera.Rotate(static_cast<float>(input.GetMouseDeltaX()), static_cast<float>(input.GetMouseDeltaY()));

        Renderer::Clear(0.6196f, 0.8784f, 1.0f, 1.0f);

        const float aspect_ratio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
        glm::mat4   view         = camera.GetViewMatrix();
        glm::mat4   projection   = Camera::GetProjectionMatrix(aspect_ratio);

        texture.Bind(0);
        shader.Use();

        glUniformMatrix4fv(glGetUniformLocation(shader.GetId(), "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.GetId(), "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.GetId(), "projection"), 1, GL_FALSE, &projection[0][0]);

        quad.Render();
        Texture::Unbind();

        window.SwapBuffers();
    }

    return 0;
}
