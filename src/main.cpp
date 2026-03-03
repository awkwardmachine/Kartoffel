//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include <glad/glad.h>
#include <iostream>

#include "InputManager.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Window.hpp"

#include "ecs/Components.hpp"
#include "ecs/World.hpp"
#include "ecs/systems/InputSystem.hpp"
#include "ecs/systems/RenderSystem.hpp"

int main() {
    // Initial setup
    Window       window;
    InputManager input;

    if (!window.Initialize(800, 600, "Kartoffel")) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    if (Renderer renderer; !renderer.Initialize(window.GetNativeWindow())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    Shader  shader;
    Mesh    cube;
    Texture texture;

    if (!shader.LoadFromFile("shaders/triangle.vert", "shaders/triangle.frag")) {
        std::cerr << "Failed to load shaders" << std::endl;
        return -1;
    }

    if (!cube.CreateCube()) {
        std::cerr << "Failed to create cube" << std::endl;
        return -1;
    }

    if (!texture.Load("textures/stone.png", TextureFilter::Nearest)) {
        std::cerr << "Failed to load texture" << std::endl;
        return -1;
    }

    input.Initialize(window.GetNativeWindow());
    window.SetMouseCursorEnabled(false);

    // ECS
    World world;

    world.RegisterComponent<TransformComponent>();
    world.RegisterComponent<MeshComponent>();
    world.RegisterComponent<ShaderComponent>();
    world.RegisterComponent<TextureComponent>();
    world.RegisterComponent<CameraComponent>();

    world.RegisterSystem<InputSystem>(&input, &window);
    world.RegisterSystem<RenderSystem>(&window);

    // Camera entity
    const Entity camera_entity = world.CreateEntity();
    world.AddComponent<CameraComponent>(camera_entity, CameraComponent{});

    // Cube entity
    const Entity cube_entity = world.CreateEntity();
    world.AddComponent<TransformComponent>(cube_entity, TransformComponent{});
    world.AddComponent<MeshComponent>(cube_entity, MeshComponent{
                                                           .vao          = cube.GetVAO(),
                                                           .vbo          = cube.GetVBO(),
                                                           .ebo          = cube.GetEBO(),
                                                           .vertex_count = cube.GetVertexCount(),
                                                           .use_indices  = true,
                                                   });
    world.AddComponent<ShaderComponent>(cube_entity, ShaderComponent{.id = shader.GetId()});
    world.AddComponent<TextureComponent>(cube_entity, TextureComponent{.id = texture.GetId(), .slot = 0});

    while (!window.ShouldClose()) {
        Window::PollEvents();
        world.UpdateSystems(0.0f);
        window.SwapBuffers();
    }

    world.ShutdownSystems();

    return 0;
}
