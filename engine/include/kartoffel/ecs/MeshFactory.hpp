//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_MESHFACTORY_HPP
#define KARTOFFEL_MESHFACTORY_HPP

#include <optional>
#include <string>
#include <vector>

#include "Components.hpp"
#include "kartoffel/assets/KAsset.hpp"

enum class MeshType {
    Quad,
    Cube,
};

struct MeshResult {
    MeshComponent                   mesh;
    std::optional<TextureComponent> texture;
};

class MeshFactory {
public:
    static MeshComponent           Create(MeshType type);
    static std::vector<MeshResult> LoadFromFile(const std::string &path);
    static bool                    BakeToKAsset(const std::string &src_path, const std::string &dst_path);
    static bool                    LoadKAsset(const std::string &path, KAsset &out);

    // Compute a tight-fitting BoxColliderComponent by scanning all vertex
    // positions in a .kasset file.  Returns a collider with half_extents and
    // offset set to exactly wrap the mesh; all other fields left at defaults.
    static BoxColliderComponent ComputeAABB(const std::string &kasset_path);

    // Overload for procedural mesh types (no file needed).
    static BoxColliderComponent ComputeAABB(MeshType type);

private:
    static MeshComponent CreateQuad();
    static MeshComponent CreateCube();

    static void UploadIndexed(MeshComponent &mesh, const float *vertices, int vertex_data_size,
                              const unsigned int *indices, int index_data_size, int vertex_count);
    static void UploadVertices(MeshComponent &mesh, const float *vertices, int vertex_data_size, int vertex_count);
    static void SetupAttributes();

    static std::vector<KAssetMesh> ParseGltf(const std::string &path);
    static std::vector<KAssetMesh> ParseObj(const std::string &path);

    static bool WriteKAsset(const std::string &path, const std::vector<KAssetMesh> &meshes);

    static MeshComponent                   UploadKAssetMesh(const KAssetMesh &mesh);
    static std::optional<TextureComponent> UploadKAssetTexture(const KAssetMesh &mesh);

    static std::string KAssetPathFor(const std::string &src_path);
};

#endif // KARTOFFEL_MESHFACTORY_HPP