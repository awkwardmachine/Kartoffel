//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_MESHFACTORY_HPP
#define KARTOFFEL_MESHFACTORY_HPP

#include "Components.hpp"

enum class MeshType {
    Quad,
    Cube,
};

class MeshFactory {
public:
    static MeshComponent Create(MeshType type);

private:
    static MeshComponent CreateQuad();
    static MeshComponent CreateCube();

    static void UploadIndexed(MeshComponent &mesh, const float *vertices, int vertex_data_size,
                              const unsigned int *indices, int index_data_size, int vertex_count);

    static void UploadVertices(MeshComponent &mesh, const float *vertices, int vertex_data_size, int vertex_count);

    static void SetupAttributes();
};

#endif // KARTOFFEL_MESHFACTORY_HPP
