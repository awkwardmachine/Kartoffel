//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "MeshCleanupSystem.hpp"

#include <glad/glad.h>

#include "Components.hpp"
#include "World.hpp"

void MeshCleanupSystem::Shutdown(World &world) {
    for (const Entity e: world.Query<MeshComponent>()) {
        MeshComponent &mesh = world.GetComponent<MeshComponent>(e);
        if (mesh.ebo)
            glDeleteBuffers(1, &mesh.ebo);
        if (mesh.vbo)
            glDeleteBuffers(1, &mesh.vbo);
        if (mesh.vao)
            glDeleteVertexArrays(1, &mesh.vao);
    }
}
