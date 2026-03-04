//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/ecs/systems/MeshCleanupSystem.hpp"

#include <glad/glad.h>

#include "kartoffel/ecs/Components.hpp"
#include "kartoffel/ecs/World.hpp"

void MeshCleanupSystem::Shutdown(World &world) {
    for (const Entity e: world.Query<MeshComponent>()) {
        auto &mesh = world.GetComponent<MeshComponent>(e);
        if (mesh.ebo)
            glDeleteBuffers(1, &mesh.ebo);
        if (mesh.vbo)
            glDeleteBuffers(1, &mesh.vbo);
        if (mesh.vao)
            glDeleteVertexArrays(1, &mesh.vao);
    }
}
