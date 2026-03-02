//
// Created by nil on 02/03/2026.
//

#include "Mesh.hpp"
#include <iostream>
#include "glad/glad.h"

Mesh::Mesh() : VAO_(0), VBO_(0), vertex_count_(0), created_(false) {}

Mesh::~Mesh() { Cleanup(); }

bool Mesh::CreateTriangle() {
    constexpr float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

    vertex_count_ = 3;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    created_ = true;
    std::cout << "Triangle mesh created successfully" << std::endl;
    return true;
}

void Mesh::Render() const {
    if (!created_) {
        std::cerr << "Cannot render uninitialized mesh!" << std::endl;
        return;
    }

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
    glBindVertexArray(0);
}

void Mesh::Cleanup() {
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
