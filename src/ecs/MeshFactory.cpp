//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "MeshFactory.hpp"

#include <cassert>
#include <glad/glad.h>

MeshComponent MeshFactory::Create(const MeshType type) {
    switch (type) {
        case MeshType::Quad:
            return CreateQuad();
        case MeshType::Cube:
            return CreateCube();
    }
    assert(false && "Unknown MeshType");
}

MeshComponent MeshFactory::CreateQuad() {
    // layout: x, y, z, u, v
    constexpr float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
    };
    constexpr unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    MeshComponent mesh;
    UploadIndexed(mesh, vertices, sizeof(vertices), indices, sizeof(indices), 6);
    return mesh;
}

MeshComponent MeshFactory::CreateCube() {
    // layout: x, y, z, u, v
    constexpr float vertices[] = {
            // Front face
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,

            // Back face
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,

            // Left face
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.5f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,

            // Right face
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,

            // Top face
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,

            // Bottom face
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            1.0f,
    };
    constexpr unsigned int indices[] = {
            0,  1,  2,  0,  2,  3,  // front
            4,  5,  6,  4,  6,  7,  // back
            8,  9,  10, 8,  10, 11, // left
            12, 13, 14, 12, 14, 15, // right
            16, 17, 18, 16, 18, 19, // top
            20, 21, 22, 20, 22, 23, // bottom
    };

    MeshComponent mesh;
    UploadIndexed(mesh, vertices, sizeof(vertices), indices, sizeof(indices), 36);
    return mesh;
}

void MeshFactory::UploadIndexed(MeshComponent &mesh, const float *vertices, const int vertex_data_size,
                                const unsigned int *indices, const int index_data_size, const int vertex_count) {
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data_size, indices, GL_STATIC_DRAW);

    SetupAttributes();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mesh.vertex_count = vertex_count;
    mesh.use_indices  = true;
}

void MeshFactory::UploadVertices(MeshComponent &mesh, const float *vertices, const int vertex_data_size,
                                 const int vertex_count) {
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertices, GL_STATIC_DRAW);

    SetupAttributes();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mesh.vertex_count = vertex_count;
    mesh.use_indices  = false;
}

void MeshFactory::SetupAttributes() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
