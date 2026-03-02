//
// Created by nil on 02/03/2026.
//

#include "Mesh.hpp"
#include <iostream>
#include "glad/glad.h"

Mesh::Mesh() : VAO_(0), VBO_(0), EBO_(0), vertex_count_(0), created_(false) {}

Mesh::~Mesh() { Cleanup(); }

bool Mesh::CreateTriangle() {
    constexpr float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

    vertex_count_ = 3;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    created_ = true;
    std::cout << "Triangle mesh created successfully" << std::endl;
    return true;
}

bool Mesh::CreateQuad() {
    constexpr float        vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    created_ = true;
    std::cout << "Quad mesh created successfully" << std::endl;
    return true;
}

bool Mesh::CreateCube() {
    constexpr float vertices[] = {
            -0.5f, -0.5f, -0.5f, // back bottom left
            0.5f,  -0.5f, -0.5f, // back bottom right
            0.5f,  0.5f,  -0.5f, // back top right
            -0.5f, 0.5f,  -0.5f, // back top left
            -0.5f, -0.5f, 0.5f,  // front bottom left
            0.5f,  -0.5f, 0.5f,  // front bottom right
            0.5f,  0.5f,  0.5f,  // front top right
            -0.5f, 0.5f,  0.5f   // front top left
    };

    constexpr unsigned int indices[] = {// Front face
                                        4, 5, 6, 4, 6, 7,

                                        // Back face
                                        1, 0, 3, 1, 3, 2,

                                        // Left face
                                        0, 4, 7, 0, 7, 3,

                                        // Right face
                                        5, 1, 2, 5, 2, 6,

                                        // Top face
                                        7, 6, 2, 7, 2, 3,

                                        // Bottom face
                                        0, 1, 5, 0, 5, 4};

    vertex_count_ = 36;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

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
