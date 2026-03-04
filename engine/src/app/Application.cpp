//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/app/Application.hpp"

#include <iostream>

#include "kartoffel/rendering/Renderer.hpp"
#include <GLFW/glfw3.h>
#include "kartoffel/assets/AssetManager.hpp"
#include "kartoffel/ecs/systems/InputSystem.hpp"
#include "kartoffel/ecs/systems/RenderSystem.hpp"
#include "kartoffel/ecs/systems/MeshCleanupSystem.hpp"
#include "kartoffel/ecs/systems/PhysicsSystem.hpp"
#include "kartoffel/ecs/systems/ScriptSystem.hpp"
#include "kartoffel/ecs/Components.hpp"

namespace Kartoffel {

    Application::Application(AppConfig config) : config_(std::move(config)) {}

    void Application::RegisterCoreComponents() {
        world_.RegisterComponent<TagComponent>();
        world_.RegisterComponent<TransformComponent>();
        world_.RegisterComponent<MeshComponent>();
        world_.RegisterComponent<ModelComponent>();
        world_.RegisterComponent<ShaderComponent>();
        world_.RegisterComponent<TextureComponent>();
        world_.RegisterComponent<CameraComponent>();
        world_.RegisterComponent<LightComponent>();
        world_.RegisterComponent<RigidBodyComponent>();
        world_.RegisterComponent<BoxColliderComponent>();
        world_.RegisterComponent<ScriptComponent>();
    }

    int Application::Run() {
        // window + gl
        if (!window_.Initialize(config_.width, config_.height, config_.title.c_str())) {
            std::cerr << "[Application] Window init failed\n";
            return -1;
        }

        {
            if (Renderer renderer; !renderer.Initialize(window_.GetNativeWindow())) {
                std::cerr << "[Application] Renderer init failed\n";
                return -1;
            }
        }

        glEnable(GL_MULTISAMPLE);

        AssetManager::Init();
        input_.Initialize(window_.GetNativeWindow());

        if (config_.capture_mouse)
            window_.SetMouseCursorEnabled(false);

        // ECS
        RegisterCoreComponents();

        // In-built systems
        world_.RegisterSystem<ScriptSystem>();
        world_.RegisterSystem<PhysicsSystem>();
        world_.RegisterSystem<InputSystem>(&input_, &window_, "assets/scenes/main.kscene");
        world_.RegisterSystem<RenderSystem>(&window_);
        world_.RegisterSystem<MeshCleanupSystem>();

        OnInit();

        // main loop
        double last_time = glfwGetTime();
        while (!window_.ShouldClose()) {
            const double now = glfwGetTime();
            const auto  dt  = static_cast<float>(now - last_time);
            last_time        = now;

            Window::PollEvents();
            world_.UpdateSystems(dt);
            OnUpdate(dt);
            window_.SwapBuffers();
        }

        // shutdown
        OnShutdown();
        world_.ShutdownSystems();
        AssetManager::Shutdown();

        return 0;
    }

} // namespace Kartoffel
