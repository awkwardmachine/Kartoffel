//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_ASSETMANAGER_HPP
#define KARTOFFEL_ASSETMANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <kartoffel/ecs/MeshFactory.hpp>
#include <kartoffel/rendering/Shader.hpp>

class AssetManager {
public:
    static void Init();
    static void Shutdown();

    static const std::vector<MeshResult> &GetMeshes(const std::string &path);
    static unsigned int                   GetShader(const std::string &vert_path, const std::string &frag_path);

    static bool IsMeshCached(const std::string &path);
    static bool IsShaderCached(const std::string &vert_path, const std::string &frag_path);

    static void EvictMesh(const std::string &path);
    static void EvictAll();

    AssetManager()                                = default;
    AssetManager(const AssetManager &)            = delete;
    AssetManager &operator=(const AssetManager &) = delete;

private:
    static AssetManager &Instance();

    struct MeshCache {
        std::vector<MeshResult> results;
    };

    std::unordered_map<std::string, MeshCache> mesh_cache_;
    std::unordered_map<std::string, Shader>    shader_cache_;

    static std::string ShaderKey(const std::string &vert, const std::string &frag);
    static void        FreeMeshCache(MeshCache &cache);
};

#endif // KARTOFFEL_ASSETMANAGER_HPP
