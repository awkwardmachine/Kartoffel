//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_KASSET_HPP
#define KARTOFFEL_KASSET_HPP

#include <cstdint>
#include <string>
#include <vector>

static constexpr uint32_t KASSET_MAGIC   = 0x4B415354u;
static constexpr uint32_t KASSET_VERSION = 3u;

#pragma pack(push, 1)
struct KAssetVertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};
#pragma pack(pop)

static_assert(sizeof(KAssetVertex) == 32, "KAssetVertex must be 32 bytes");

#pragma pack(push, 1)
struct KAssetFileHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t mesh_count;
    uint32_t reserved;
};
#pragma pack(pop)

static_assert(sizeof(KAssetFileHeader) == 16, "KAssetFileHeader must be 16 bytes");

static constexpr uint8_t KASSET_TEX_NONE     = 0;
static constexpr uint8_t KASSET_TEX_EMBEDDED = 1;
static constexpr uint8_t KASSET_TEX_PATH     = 2;

#pragma pack(push, 1)
struct KAssetMeshEntry {
    char name[64];

    uint64_t vertex_offset;
    uint32_t vertex_count;
    uint64_t index_offset;
    uint32_t index_count;
    uint8_t  has_indices;

    uint8_t  tex_storage;
    uint8_t  pad[6];
    uint64_t texture_offset;
    uint32_t tex_byte_count;
    int32_t  tex_width;
    int32_t  tex_height;
    uint8_t  pad2[12];
};
#pragma pack(pop)

static_assert(sizeof(KAssetMeshEntry) == 128, "KAssetMeshEntry must be 128 bytes");

struct KAssetMesh {
    std::string               name;
    std::vector<KAssetVertex> vertices;
    std::vector<uint32_t>     indices;

    std::string          texture_path;
    std::vector<uint8_t> texture_data;
    int                  texture_width  = 0;
    int                  texture_height = 0;
};

struct KAsset {
    std::vector<KAssetMesh> meshes;
};

#endif // KARTOFFEL_KASSET_HPP
