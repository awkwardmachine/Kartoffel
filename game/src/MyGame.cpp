//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "MyGame.hpp"

#include <iostream>
#include <kartoffel/EngineAPI.hpp>

static constexpr auto SCENE_PATH = "assets/scenes/main.kscene";

MyGame::MyGame() :
    Application(Kartoffel::AppConfig{
            .width         = 1280,
            .height        = 720,
            .title         = "My Kartoffel Game",
            .capture_mouse = true,
    }) {}

void MyGame::OnInit() {
    // Camera
    const Entity cam = GetWorld().CreateEntity();
    GetWorld().AddComponent<TagComponent>(cam, TagComponent{.name = "MainCamera", .tags = {"camera"}});
    GetWorld().AddComponent<CameraComponent>(cam, CameraComponent{
        .position = {0.0f, 4.0f, 12.0f},
    });

    // Lights
    SceneLoader::Load(SCENE_PATH, GetWorld());

    const unsigned int shared_shader = AssetManager::GetShader(
        "assets/shaders/triangle.vert",
        "assets/shaders/triangle.frag"
    );

    // Floor
    for (const auto &[mesh, texture] : MeshFactory::LoadFromFile("assets/models/floor.kasset")) {
        const Entity e = GetWorld().CreateEntity();
        GetWorld().AddComponent<TagComponent>(e, TagComponent{.name = "Floor", .tags = {"static"}});
        GetWorld().AddComponent<TransformComponent>(e, TransformComponent{
            .position = {0.0f, 0.0f, 0.0f},
            .scale    = {1.0f, 1.0f, 1.0f},
        });
        GetWorld().AddComponent<MeshComponent>(e, mesh);
        GetWorld().AddComponent<ShaderComponent>(e, ShaderComponent{.id = shared_shader});
        GetWorld().AddComponent<ModelComponent>(e, ModelComponent{
            .kasset_path = "assets/models/floor.kasset",
            .vert_shader = "assets/shaders/triangle.vert",
            .frag_shader = "assets/shaders/triangle.frag",
        });
        if (texture.has_value())
            GetWorld().AddComponent<TextureComponent>(e, texture.value());

        auto floor_col = MeshFactory::ComputeAABB("assets/models/floor.kasset");
        floor_col.layer = LAYER_STATIC;
        floor_col.mask  = LAYER_ALL;
        GetWorld().AddComponent<BoxColliderComponent>(e, floor_col);
    }

    // Cube
    const MeshComponent cube_mesh = MeshFactory::Create(MeshType::Cube);
    const Entity e = GetWorld().CreateEntity();
    GetWorld().AddComponent<TagComponent>(e, TagComponent{.name = "Cube", .tags = {"dynamic"}});
    GetWorld().AddComponent<TransformComponent>(e, TransformComponent{
        .position = {0.0f, 5.0f, 0.0f},
        .scale    = {1.0f, 1.0f, 1.0f},
    });
    GetWorld().AddComponent<MeshComponent>(e, cube_mesh);
    GetWorld().AddComponent<ShaderComponent>(e, ShaderComponent{.id = shared_shader});
    GetWorld().AddComponent<BoxColliderComponent>(e, MeshFactory::ComputeAABB(MeshType::Cube));
    GetWorld().AddComponent<ScriptComponent>(e, ScriptComponent{
        .on_init = nullptr,
        .on_update = [timer = 0.0f, dropped = false](const Entity self, World &w, const float dt) mutable {
            if (dropped) return;
            timer += dt;
            if (timer >= 3.0f) {
                w.AddComponent<RigidBodyComponent>(self, RigidBodyComponent{
                    .mass        = 1.0f,
                    .restitution = 0.4f,
                    .linear_drag = 0.995f,
                    .use_gravity = true,
                });
                dropped = true;
            }
        },
        .on_collision = nullptr,
    });

    std::cout << "[MyGame] Ready" << std::endl;
}

void MyGame::OnUpdate(float /*dt*/) {}

void MyGame::OnShutdown() { std::cout << "[MyGame] Shutting down\n"; }