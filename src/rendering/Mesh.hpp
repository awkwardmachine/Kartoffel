//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_MESH_HPP
#define KARTOFFEL_MESH_HPP

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool CreateTriangle();
    bool CreateQuad();
    bool CreateCube();
    void Render() const;
    void Cleanup();

private:
    unsigned int VAO_, VBO_, EBO_;
    int          vertex_count_;
    bool         created_;
};

#endif // KARTOFFEL_MESH_HPP
