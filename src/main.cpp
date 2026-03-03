//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "InputManager.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

#include "assets/AssetManager.hpp"
#include "assets/SceneLoader.hpp"
#include "ecs/Components.hpp"
#include "ecs/MeshFactory.hpp"
#include "ecs/World.hpp"
#include "ecs/systems/InputSystem.hpp"
#include "ecs/systems/MeshCleanupSystem.hpp"
#include "ecs/systems/RenderSystem.hpp"

static constexpr auto SCENE_PATH = "scenes/main.kscene";

int main() {
    // Initial setup
    Window       window;
    InputManager input;

    if (!window.Initialize(800, 600, "Kartoffel")) {
        std::cerr << "Failed to initialize window\n";
        return -1;
    }

    if (Renderer renderer; !renderer.Initialize(window.GetNativeWindow())) {
        std::cerr << "Failed to initialize renderer\n";
        return -1;
    }

    AssetManager::Init();

    input.Initialize(window.GetNativeWindow());
    window.SetMouseCursorEnabled(false);

    // ECS
    World world;

    world.RegisterComponent<TransformComponent>();
    world.RegisterComponent<MeshComponent>();
    world.RegisterComponent<ModelComponent>();
    world.RegisterComponent<ShaderComponent>();
    world.RegisterComponent<TextureComponent>();
    world.RegisterComponent<CameraComponent>();

    world.RegisterSystem<InputSystem>(&input, &window, SCENE_PATH);
    world.RegisterSystem<RenderSystem>(&window);
    world.RegisterSystem<MeshCleanupSystem>();

    // Camera
    const Entity camera_entity = world.CreateEntity();
    world.AddComponent<CameraComponent>(camera_entity, CameraComponent{});

    // const unsigned int shader_id = AssetManager::GetShader("shaders/triangle.vert", "shaders/triangle.frag");
    //
    // const auto       &monkey_meshes = MeshFactory::LoadFromFile("models/monkey.glb");
    // for (const MeshResult &r: monkey_meshes) {
    //     const Entity e = world.CreateEntity();
    //     world.AddComponent<TransformComponent>(e, TransformComponent{});
    //     world.AddComponent<MeshComponent>(e, r.mesh);
    //     world.AddComponent<ShaderComponent>(e, ShaderComponent{.id = shader_id});
    //     world.AddComponent<ModelComponent>(e, ModelComponent{
    //                                                   .kasset_path = "models/monkey.kasset",
    //                                                   .vert_shader = "shaders/triangle.vert",
    //                                                   .frag_shader = "shaders/triangle.frag",
    //                                           });
    //     if (r.texture.has_value())
    //         world.AddComponent<TextureComponent>(e, r.texture.value());
    // }

    SceneLoader::Load(SCENE_PATH, world);

    // Delta time
    double last_time = glfwGetTime();

    while (!window.ShouldClose()) {
        const double now = glfwGetTime();
        const auto   dt  = static_cast<float>(now - last_time);
        last_time        = now;

        Window::PollEvents();
        world.UpdateSystems(dt);
        window.SwapBuffers();
    }

    world.ShutdownSystems();
    AssetManager::Shutdown();

    return 0;
}
