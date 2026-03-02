//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "Mesh.hpp"
#include <iostream>
#include "glad/glad.h"

Mesh::Mesh() : VAO_(0), VBO_(0), EBO_(0), vertex_count_(0), created_(false) {}

Mesh::~Mesh() { Cleanup(); }

bool Mesh::CreateTriangle() {
    // layout: x, y, z, u, v
    constexpr float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
                                  1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.5f, 1.0f};

    vertex_count_ = 3;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    created_ = true;
    std::cout << "Triangle mesh created successfully" << std::endl;
    return true;
}

bool Mesh::CreateQuad() {
    // layout: x, y, z, u, v
    constexpr float        vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                         0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};
    constexpr unsigned int indices[]  = {0, 1, 2, 0, 2, 3};

    vertex_count_ = 6;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    created_ = true;
    std::cout << "Quad mesh created successfully" << std::endl;
    return true;
}

bool Mesh::CreateCube() {
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

    vertex_count_ = 36;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    created_ = true;
    std::cout << "Cube mesh created successfully" << std::endl;
    return true;
}

void Mesh::Render() const {
    if (!created_) {
        std::cerr << "Cannot render uninitialized mesh!" << std::endl;
        return;
    }

    glBindVertexArray(VAO_);
    if (EBO_ != 0) {
        glDrawElements(GL_TRIANGLES, vertex_count_, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
    }
    glBindVertexArray(0);
}

void Mesh::Cleanup() {
    if (EBO_ != 0) {
        glDeleteBuffers(1, &EBO_);
        EBO_ = 0;
    }
    if (VBO_ != 0) {
        glDeleteBuffers(1, &VBO_);
        VBO_ = 0;
    }
    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
        VAO_ = 0;
    }
    vertex_count_ = 0;
    created_      = false;
}
