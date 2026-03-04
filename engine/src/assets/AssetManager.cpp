//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include <kartoffel/assets/AssetManager.hpp>

#include <cassert>
#include <iostream>
#include <ranges>

#include <glad/glad.h>

static AssetManager *s_instance = nullptr;

AssetManager &AssetManager::Instance() {
    assert(s_instance && "AssetManager::Init() has not been called");
    return *s_instance;
}

void AssetManager::Init() {
    assert(!s_instance && "AssetManager::Init() called twice");
    s_instance = new AssetManager();
}

void AssetManager::Shutdown() {
    if (!s_instance)
        return;

    for (auto &cache: s_instance->mesh_cache_ | std::views::values)
        FreeMeshCache(cache);

    s_instance->shader_cache_.clear();

    delete s_instance;
    s_instance = nullptr;
}

const std::vector<MeshResult> &AssetManager::GetMeshes(const std::string &path) {
    AssetManager &self = Instance();

    if (const auto it = self.mesh_cache_.find(path); it != self.mesh_cache_.end())
        return it->second.results;

    std::vector<MeshResult> loaded = MeshFactory::LoadFromFile(path);

    if (loaded.empty())
        std::cerr << "[AssetManager] Failed to load: " << path << "\n";
    else
        std::cout << "[AssetManager] Loaded " << loaded.size() << " primitive(s) from: " << path << "\n";

    MeshCache cache;
    cache.results = std::move(loaded);
    auto [ins, _] = self.mesh_cache_.emplace(path, std::move(cache));
    return ins->second.results;
}

bool AssetManager::IsMeshCached(const std::string &path) { return Instance().mesh_cache_.contains(path); }

void AssetManager::EvictMesh(const std::string &path) {
    AssetManager &self = Instance();
    auto          it   = self.mesh_cache_.find(path);
    if (it == self.mesh_cache_.end())
        return;
    FreeMeshCache(it->second);
    self.mesh_cache_.erase(it);
    std::cout << "[AssetManager] Evicted mesh: " << path << "\n";
}

void AssetManager::EvictAll() {
    AssetManager &self = Instance();
    for (auto &[key, cache]: self.mesh_cache_)
        FreeMeshCache(cache);
    self.mesh_cache_.clear();
    std::cout << "[AssetManager] Evicted all mesh caches.\n";
}

void AssetManager::FreeMeshCache(MeshCache &cache) {
    for (auto &[mesh, texture]: cache.results) {
        if (mesh.ebo)
            glDeleteBuffers(1, &mesh.ebo);
        if (mesh.vbo)
            glDeleteBuffers(1, &mesh.vbo);
        if (mesh.vao)
            glDeleteVertexArrays(1, &mesh.vao);
        if (texture.has_value() && texture->id)
            glDeleteTextures(1, &texture->id);
    }
    cache.results.clear();
}

std::string AssetManager::ShaderKey(const std::string &v, const std::string &f) { return v + "|" + f; }

unsigned int AssetManager::GetShader(const std::string &vert, const std::string &frag) {
    AssetManager     &self = Instance();
    const std::string key  = ShaderKey(vert, frag);

    if (const auto it = self.shader_cache_.find(key); it != self.shader_cache_.end())
        return it->second.GetId();

    Shader shader;
    if (!shader.LoadFromFile(vert, frag)) {
        std::cerr << "[AssetManager] Failed to compile shader: " << vert << " + " << frag << "\n";
        return 0;
    }

    const unsigned int id = shader.GetId();
    self.shader_cache_.emplace(key, std::move(shader));
    std::cout << "[AssetManager] Compiled shader: " << vert << "\n";
    return id;
}

bool AssetManager::IsShaderCached(const std::string &vert, const std::string &frag) {
    return Instance().shader_cache_.contains(ShaderKey(vert, frag));
}
