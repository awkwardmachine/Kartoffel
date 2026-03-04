//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include <kartoffel/ecs/MeshFactory.hpp>

#include <algorithm>
#include <limits>
#include <optional>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <glad/glad.h>

#include <stb_image.h>
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace fs = std::filesystem;

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
    // layout: x, y, z, nx, ny, nz, u, v
    constexpr float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    };
    constexpr unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    MeshComponent mesh;
    UploadIndexed(mesh, vertices, sizeof(vertices), indices, sizeof(indices), 6);
    return mesh;
}

MeshComponent MeshFactory::CreateCube() {
    // layout: x, y, z, nx, ny, nz, u, v
    constexpr float vertices[] = {
            // Front
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,

            // Back
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,

            // Left
            -0.5f,
            -0.5f,
            -0.5f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            -0.5f,
            -0.5f,
            0.5f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.5f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,

            // Right
            0.5f,
            -0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.5f,
            0.5f,
            0.5f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,

            // Top
            -0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,

            // Bottom
            -0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            -0.5f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.5f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            1.0f,
            -0.5f,
            -0.5f,
            0.5f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
    };
    constexpr unsigned int indices[] = {
            0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
            12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
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
    constexpr int stride = 8 * sizeof(float); // x,y,z, nx,ny,nz, u,v

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

std::string MeshFactory::KAssetPathFor(const std::string &src_path) {
    const fs::path p(src_path);
    return (p.parent_path() / p.stem()).string() + ".kasset";
}

std::vector<MeshResult> MeshFactory::LoadFromFile(const std::string &path) {
    const fs::path p(path);
    std::string    ext = p.extension().string();
    std::ranges::transform(ext, ext.begin(), ::tolower);

    std::vector<KAssetMesh> meshes;

    if (ext == ".kasset") {
        KAsset asset;
        if (!LoadKAsset(path, asset)) {
            std::cerr << "[MeshFactory] Failed to load kasset: " << path << "\n";
            return {};
        }
        meshes = std::move(asset.meshes);
    } else {
        const std::string cache_path = KAssetPathFor(path);

        if (fs::exists(cache_path)) {
            const bool source_exists = fs::exists(path);
            const bool cache_fresh   = !source_exists || fs::last_write_time(cache_path) >= fs::last_write_time(path);

            if (cache_fresh) {
                if (KAsset asset; LoadKAsset(cache_path, asset)) {
                    meshes = std::move(asset.meshes);
                }
            }
        }

        if (meshes.empty()) {
            if (!fs::exists(path)) {
                std::cerr << "[MeshFactory] No .kasset cache and source file not found: " << path << "\n";
                return {};
            }

            if (ext == ".gltf" || ext == ".glb") {
                meshes = ParseGltf(path);
            } else if (ext == ".obj") {
                meshes = ParseObj(path);
            } else {
                std::cerr << "[MeshFactory] Unsupported format: " << ext << "\n";
                return {};
            }

            if (meshes.empty()) {
                std::cerr << "[MeshFactory] No meshes parsed from: " << path << "\n";
                return {};
            }

            if (!WriteKAsset(cache_path, meshes)) {
                std::cerr << "[MeshFactory] Warning: could not write kasset cache: " << cache_path << "\n";
            }
        }
    }

    std::vector<MeshResult> results;
    results.reserve(meshes.size());
    for (const KAssetMesh &m: meshes) {
        MeshResult r;
        r.mesh    = UploadKAssetMesh(m);
        r.texture = UploadKAssetTexture(m);
        results.push_back(std::move(r));
    }
    return results;
}

std::vector<KAssetMesh> MeshFactory::ParseGltf(const std::string &path) {
    tinygltf::Model    model;
    tinygltf::TinyGLTF loader;
    std::string        err, warn;

    loader.SetImageLoader(
            [](tinygltf::Image *img, const int, std::string *, std::string *, int, int, const unsigned char *bytes,
               int size, void *) -> bool {
                int            w, h, ch;
                unsigned char *data = stbi_load_from_memory(bytes, size, &w, &h, &ch, 4);
                if (!data)
                    return false;
                img->width      = w;
                img->height     = h;
                img->component  = 4;
                img->bits       = 8;
                img->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
                img->image.assign(data, data + w * h * 4);
                stbi_image_free(data);
                return true;
            },
            nullptr);

    fs::path    p(path);
    std::string ext = p.extension().string();
    std::ranges::transform(ext, ext.begin(), ::tolower);

    bool ok = (ext == ".glb") ? loader.LoadBinaryFromFile(&model, &err, &warn, path)
                              : loader.LoadASCIIFromFile(&model, &err, &warn, path);

    if (!warn.empty())
        std::cerr << "[MeshFactory] glTF warning: " << warn << "\n";
    if (!ok) {
        std::cerr << "[MeshFactory] glTF error loading '" << path << "': " << err << "\n";
        return {};
    }

    std::vector<KAssetMesh> result;

    for (const tinygltf::Mesh &gltf_mesh: model.meshes) {
        for (size_t pi = 0; pi < gltf_mesh.primitives.size(); ++pi) {
            const tinygltf::Primitive &prim = gltf_mesh.primitives[pi];

            if (prim.mode != TINYGLTF_MODE_TRIANGLES) {
                std::cerr << "[MeshFactory] Skipping non-triangle primitive in '" << gltf_mesh.name << "'\n";
                continue;
            }

            KAssetMesh mesh;
            mesh.name = gltf_mesh.name.empty()
                                ? ("mesh_" + std::to_string(result.size()))
                                : (gltf_mesh.primitives.size() == 1 ? gltf_mesh.name
                                                                    : gltf_mesh.name + "_" + std::to_string(pi));

            // positions
            auto pos_it = prim.attributes.find("POSITION");
            if (pos_it == prim.attributes.end()) {
                std::cerr << "[MeshFactory] Primitive has no POSITION, skipping\n";
                continue;
            }
            const tinygltf::Accessor   &pos_acc  = model.accessors[pos_it->second];
            const tinygltf::BufferView &pos_view = model.bufferViews[pos_acc.bufferView];
            const unsigned char        *pos_data =
                    model.buffers[pos_view.buffer].data.data() + pos_view.byteOffset + pos_acc.byteOffset;
            int pos_stride = pos_acc.ByteStride(pos_view);
            if (pos_stride <= 0)
                pos_stride = 3 * sizeof(float);

            const size_t vertex_count = pos_acc.count;
            mesh.vertices.resize(vertex_count);

            for (size_t i = 0; i < vertex_count; ++i) {
                const auto *p_f     = reinterpret_cast<const float *>(pos_data + i * pos_stride);
                mesh.vertices[i].x  = p_f[0];
                mesh.vertices[i].y  = p_f[1];
                mesh.vertices[i].z  = p_f[2];
                mesh.vertices[i].nx = 0.0f;
                mesh.vertices[i].ny = 0.0f;
                mesh.vertices[i].nz = 1.0f;
                mesh.vertices[i].u  = 0.0f;
                mesh.vertices[i].v  = 0.0f;
            }

            // normals
            if (auto nrm_it = prim.attributes.find("NORMAL"); nrm_it != prim.attributes.end()) {
                const tinygltf::Accessor   &nrm_acc  = model.accessors[nrm_it->second];
                const tinygltf::BufferView &nrm_view = model.bufferViews[nrm_acc.bufferView];
                const unsigned char        *nrm_data =
                        model.buffers[nrm_view.buffer].data.data() + nrm_view.byteOffset + nrm_acc.byteOffset;
                int nrm_stride = nrm_acc.ByteStride(nrm_view);
                if (nrm_stride <= 0)
                    nrm_stride = 3 * sizeof(float);

                for (size_t i = 0; i < vertex_count; ++i) {
                    const auto *n_f     = reinterpret_cast<const float *>(nrm_data + i * nrm_stride);
                    mesh.vertices[i].nx = n_f[0];
                    mesh.vertices[i].ny = n_f[1];
                    mesh.vertices[i].nz = n_f[2];
                }
            }

            // texcoords
            if (auto uv_it = prim.attributes.find("TEXCOORD_0"); uv_it != prim.attributes.end()) {
                const tinygltf::Accessor   &uv_acc  = model.accessors[uv_it->second];
                const tinygltf::BufferView &uv_view = model.bufferViews[uv_acc.bufferView];
                const unsigned char        *uv_data =
                        model.buffers[uv_view.buffer].data.data() + uv_view.byteOffset + uv_acc.byteOffset;
                int uv_stride = uv_acc.ByteStride(uv_view);
                if (uv_stride <= 0)
                    uv_stride = 2 * sizeof(float);

                for (size_t i = 0; i < vertex_count; ++i) {
                    const auto *uv_f   = reinterpret_cast<const float *>(uv_data + i * uv_stride);
                    mesh.vertices[i].u = uv_f[0];
                    mesh.vertices[i].v = uv_f[1];
                }
            }

            // indices
            if (prim.indices >= 0) {
                const tinygltf::Accessor   &idx_acc  = model.accessors[prim.indices];
                const tinygltf::BufferView &idx_view = model.bufferViews[idx_acc.bufferView];
                const unsigned char        *idx_data =
                        model.buffers[idx_view.buffer].data.data() + idx_view.byteOffset + idx_acc.byteOffset;
                const size_t index_count = idx_acc.count;
                mesh.indices.resize(index_count);

                for (size_t i = 0; i < index_count; ++i) {
                    switch (idx_acc.componentType) {
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                            mesh.indices[i] = static_cast<uint32_t>(idx_data[i]);
                            break;
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                            mesh.indices[i] = static_cast<uint32_t>(reinterpret_cast<const uint16_t *>(idx_data)[i]);
                            break;
                        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                            mesh.indices[i] = reinterpret_cast<const uint32_t *>(idx_data)[i];
                            break;
                        default:
                            mesh.indices[i] = 0;
                            break;
                    }
                }
            }

            // textures
            if (prim.material >= 0) {
                const tinygltf::Material &mat = model.materials[prim.material];
                if (int tex_index = mat.pbrMetallicRoughness.baseColorTexture.index; tex_index >= 0) {
                    if (const tinygltf::Texture &tex = model.textures[tex_index]; tex.source >= 0) {
                        if (const tinygltf::Image &img = model.images[tex.source]; !img.uri.empty()) {
                            fs::path base     = fs::path(path).parent_path();
                            mesh.texture_path = (base / img.uri).string();
                        } else if (!img.image.empty()) {
                            mesh.texture_data   = img.image;
                            mesh.texture_width  = img.width;
                            mesh.texture_height = img.height;
                        }
                    }
                }
            }

            result.push_back(std::move(mesh));
        }
    }

    return result;
}

std::vector<KAssetMesh> MeshFactory::ParseObj(const std::string &path) {
    tinyobj::ObjReaderConfig cfg;
    cfg.mtl_search_path = fs::path(path).parent_path().string();
    cfg.triangulate     = true;

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path, cfg)) {
        std::cerr << "[MeshFactory] OBJ error: " << reader.Error() << "\n";
        return {};
    }
    if (!reader.Warning().empty()) {
        std::cerr << "[MeshFactory] OBJ warning: " << reader.Warning() << "\n";
    }

    const tinyobj::attrib_t             &attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t> &shapes = reader.GetShapes();

    std::vector<KAssetMesh> result;
    result.reserve(shapes.size());

    for (const tinyobj::shape_t &shape: shapes) {
        KAssetMesh mesh;
        mesh.name = shape.name.empty() ? ("shape_" + std::to_string(result.size())) : shape.name;

        struct VertKey {
            int vi, ni, ti;
        };
        struct VertKeyHash {
            size_t operator()(const VertKey &k) const {
                return std::hash<int>()(k.vi) ^ (std::hash<int>()(k.ni) << 10) ^ (std::hash<int>()(k.ti) << 20);
            }
        };
        struct VertKeyEq {
            bool operator()(const VertKey &a, const VertKey &b) const {
                return a.vi == b.vi && a.ni == b.ni && a.ti == b.ti;
            }
        };
        std::unordered_map<VertKey, uint32_t, VertKeyHash, VertKeyEq> index_map;

        for (const auto &[vertex_index, normal_index, texcoord_index]: shape.mesh.indices) {
            VertKey key{vertex_index, normal_index, texcoord_index};
            auto    it = index_map.find(key);
            if (it != index_map.end()) {
                mesh.indices.push_back(it->second);
            } else {
                KAssetVertex v{};
                if (vertex_index >= 0) {
                    v.x = attrib.vertices[3 * vertex_index + 0];
                    v.y = attrib.vertices[3 * vertex_index + 1];
                    v.z = attrib.vertices[3 * vertex_index + 2];
                }
                if (normal_index >= 0) {
                    v.nx = attrib.normals[3 * normal_index + 0];
                    v.ny = attrib.normals[3 * normal_index + 1];
                    v.nz = attrib.normals[3 * normal_index + 2];
                } else {
                    v.nz = 1.0f;
                }
                if (texcoord_index >= 0) {
                    v.u = attrib.texcoords[2 * texcoord_index + 0];
                    v.v = 1.0f - attrib.texcoords[2 * texcoord_index + 1];
                }
                auto new_idx = static_cast<uint32_t>(mesh.vertices.size());
                mesh.vertices.push_back(v);
                index_map[key] = new_idx;
                mesh.indices.push_back(new_idx);
            }
        }

        // texture from .mtl file
        if (!reader.GetMaterials().empty()) {
            if (const tinyobj::material_t &mat = reader.GetMaterials()[0]; !mat.diffuse_texname.empty()) {
                fs::path base     = fs::path(path).parent_path();
                mesh.texture_path = (base / mat.diffuse_texname).string();
            }
        }

        result.push_back(std::move(mesh));
    }

    return result;
}

bool MeshFactory::WriteKAsset(const std::string &path, const std::vector<KAssetMesh> &meshes) {
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f) {
        std::cerr << "[MeshFactory] Cannot open for writing: " << path << "\n";
        return false;
    }

    const auto mesh_count = static_cast<uint32_t>(meshes.size());

    // header
    const uint64_t data_start = sizeof(KAssetFileHeader) + static_cast<uint64_t>(mesh_count) * sizeof(KAssetMeshEntry);

    std::vector<KAssetMeshEntry> entries(mesh_count);
    uint64_t                     cursor = data_start;

    for (uint32_t i = 0; i < mesh_count; ++i) {
        const KAssetMesh &m = meshes[i];
        KAssetMeshEntry  &e = entries[i];

        std::memset(&e, 0, sizeof(e));
        std::strncpy(e.name, m.name.c_str(), sizeof(e.name) - 1);

        // geometry
        e.vertex_offset = cursor;
        e.vertex_count  = static_cast<uint32_t>(m.vertices.size());
        cursor += e.vertex_count * sizeof(KAssetVertex);

        if (!m.indices.empty()) {
            e.has_indices  = 1;
            e.index_offset = cursor;
            e.index_count  = static_cast<uint32_t>(m.indices.size());
            cursor += e.index_count * sizeof(uint32_t);
        }

        // texture
        if (!m.texture_data.empty()) {
            e.tex_storage    = KASSET_TEX_EMBEDDED;
            e.texture_offset = cursor;
            e.tex_byte_count = static_cast<uint32_t>(m.texture_data.size());
            e.tex_width      = m.texture_width;
            e.tex_height     = m.texture_height;
            cursor += e.tex_byte_count;
        } else if (!m.texture_path.empty()) {
            e.tex_storage    = KASSET_TEX_PATH;
            e.texture_offset = cursor;
            e.tex_byte_count = static_cast<uint32_t>(m.texture_path.size());
            e.tex_width      = 0;
            e.tex_height     = 0;
            cursor += e.tex_byte_count;
        }
    }

    // write file header
    KAssetFileHeader header{};
    header.magic      = KASSET_MAGIC;
    header.version    = KASSET_VERSION;
    header.mesh_count = mesh_count;
    header.reserved   = 0;
    f.write(reinterpret_cast<const char *>(&header), sizeof(header));

    // write mesh entries
    for (const KAssetMeshEntry &e: entries) {
        f.write(reinterpret_cast<const char *>(&e), sizeof(e));
    }

    // write data blob
    for (uint32_t i = 0; i < mesh_count; ++i) {
        const KAssetMesh      &m = meshes[i];
        const KAssetMeshEntry &e = entries[i];

        f.write(reinterpret_cast<const char *>(m.vertices.data()),
                static_cast<std::streamsize>(m.vertices.size() * sizeof(KAssetVertex)));

        if (e.has_indices) {
            f.write(reinterpret_cast<const char *>(m.indices.data()),
                    static_cast<std::streamsize>(m.indices.size() * sizeof(uint32_t)));
        }

        if (e.tex_storage == KASSET_TEX_EMBEDDED) {
            f.write(reinterpret_cast<const char *>(m.texture_data.data()),
                    static_cast<std::streamsize>(m.texture_data.size()));
        } else if (e.tex_storage == KASSET_TEX_PATH) {
            f.write(m.texture_path.data(), static_cast<std::streamsize>(m.texture_path.size()));
        }
    }

    return f.good();
}

bool MeshFactory::LoadKAsset(const std::string &path, KAsset &out) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        std::cerr << "[MeshFactory] Cannot open kasset: " << path << "\n";
        return false;
    }

    KAssetFileHeader header{};
    f.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!f || header.magic != KASSET_MAGIC) {
        std::cerr << "[MeshFactory] Bad kasset magic in: " << path << "\n";
        return false;
    }
    if (header.version != KASSET_VERSION) {
        std::cerr << "[MeshFactory] Unsupported kasset version " << header.version << " in: " << path << "\n";
        return false;
    }

    std::vector<KAssetMeshEntry> entries(header.mesh_count);
    for (uint32_t i = 0; i < header.mesh_count; ++i) {
        f.read(reinterpret_cast<char *>(&entries[i]), sizeof(KAssetMeshEntry));
    }

    out.meshes.resize(header.mesh_count);
    for (uint32_t i = 0; i < header.mesh_count; ++i) {
        const KAssetMeshEntry &e                                                                   = entries[i];
        auto &[name, vertices, indices, texture_path, texture_data, texture_width, texture_height] = out.meshes[i];

        name = e.name;

        // geometry
        vertices.resize(e.vertex_count);
        f.seekg(static_cast<std::streamoff>(e.vertex_offset));
        f.read(reinterpret_cast<char *>(vertices.data()),
               static_cast<std::streamsize>(e.vertex_count * sizeof(KAssetVertex)));

        if (e.has_indices) {
            indices.resize(e.index_count);
            f.seekg(static_cast<std::streamoff>(e.index_offset));
            f.read(reinterpret_cast<char *>(indices.data()),
                   static_cast<std::streamsize>(e.index_count * sizeof(uint32_t)));
        }

        // texture
        if (e.tex_storage == KASSET_TEX_EMBEDDED && e.tex_byte_count > 0) {
            texture_data.resize(e.tex_byte_count);
            f.seekg(static_cast<std::streamoff>(e.texture_offset));
            f.read(reinterpret_cast<char *>(texture_data.data()), e.tex_byte_count);
            texture_width  = e.tex_width;
            texture_height = e.tex_height;
        } else if (e.tex_storage == KASSET_TEX_PATH && e.tex_byte_count > 0) {
            texture_path.resize(e.tex_byte_count);
            f.seekg(static_cast<std::streamoff>(e.texture_offset));
            f.read(texture_path.data(), e.tex_byte_count);
        }
    }

    return f.good() || f.eof();
}


bool MeshFactory::BakeToKAsset(const std::string &src_path, const std::string &dst_path) {
    const fs::path p(src_path);
    std::string    ext = p.extension().string();
    std::ranges::transform(ext, ext.begin(), ::tolower);

    std::vector<KAssetMesh> meshes;
    if (ext == ".gltf" || ext == ".glb") {
        meshes = ParseGltf(src_path);
    } else if (ext == ".obj") {
        meshes = ParseObj(src_path);
    } else {
        std::cerr << "[MeshFactory] BakeToKAsset: unsupported format '" << ext << "'\n";
        return false;
    }

    if (meshes.empty()) {
        std::cerr << "[MeshFactory] BakeToKAsset: no meshes in '" << src_path << "'\n";
        return false;
    }

    return WriteKAsset(dst_path, meshes);
}

MeshComponent MeshFactory::UploadKAssetMesh(const KAssetMesh &mesh) {
    MeshComponent comp{};

    const bool indexed = !mesh.indices.empty();

    glGenVertexArrays(1, &comp.vao);
    glGenBuffers(1, &comp.vbo);
    glBindVertexArray(comp.vao);

    glBindBuffer(GL_ARRAY_BUFFER, comp.vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.vertices.size() * sizeof(KAssetVertex)),
                 mesh.vertices.data(), GL_STATIC_DRAW);

    if (indexed) {
        glGenBuffers(1, &comp.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.indices.size() * sizeof(uint32_t)),
                     mesh.indices.data(), GL_STATIC_DRAW);
        comp.vertex_count = static_cast<int>(mesh.indices.size());
        comp.use_indices  = true;
    } else {
        comp.vertex_count = static_cast<int>(mesh.vertices.size());
        comp.use_indices  = false;
    }

    SetupAttributes();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return comp;
}

std::optional<TextureComponent> MeshFactory::UploadKAssetTexture(const KAssetMesh &mesh) {
    if (mesh.texture_path.empty() && mesh.texture_data.empty())
        return std::nullopt;

    unsigned int tex_id = 0;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bool ok = false;

    if (!mesh.texture_path.empty()) {
        stbi_set_flip_vertically_on_load(true);
        int w, h, ch;
        if (unsigned char *data = stbi_load(mesh.texture_path.c_str(), &w, &h, &ch, 0)) {
            const GLenum fmt = (ch == 4) ? GL_RGBA : (ch == 1 ? GL_RED : GL_RGB);
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(fmt), w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
            ok = true;
        } else {
            std::cerr << "[MeshFactory] Failed to load texture: " << mesh.texture_path << "\n";
        }
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mesh.texture_width, mesh.texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     mesh.texture_data.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        ok = true;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    if (!ok) {
        glDeleteTextures(1, &tex_id);
        return std::nullopt;
    }

    return TextureComponent{.id = tex_id, .slot = 0};
}

BoxColliderComponent MeshFactory::ComputeAABB(const std::string &kasset_path) {
    KAsset asset;
    if (!LoadKAsset(kasset_path, asset) || asset.meshes.empty()) {
        std::cerr << "[MeshFactory] ComputeAABB: failed to load " << kasset_path << "\n";
        return BoxColliderComponent{};
    }

    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    float min_z = std::numeric_limits<float>::max();
    float max_x = -std::numeric_limits<float>::max();
    float max_y = -std::numeric_limits<float>::max();
    float max_z = -std::numeric_limits<float>::max();

    for (const auto &mesh: asset.meshes) {
        for (const auto &v: mesh.vertices) {
            min_x = std::min(min_x, v.x);
            max_x = std::max(max_x, v.x);
            min_y = std::min(min_y, v.y);
            max_y = std::max(max_y, v.y);
            min_z = std::min(min_z, v.z);
            max_z = std::max(max_z, v.z);
        }
    }

    const glm::vec3 half_extents = {
            (max_x - min_x) * 0.5f,
            (max_y - min_y) * 0.5f,
            (max_z - min_z) * 0.5f,
    };
    const glm::vec3 offset = {
            (max_x + min_x) * 0.5f,
            (max_y + min_y) * 0.5f,
            (max_z + min_z) * 0.5f,
    };

    return BoxColliderComponent{.half_extents = half_extents, .offset = offset};
}

BoxColliderComponent MeshFactory::ComputeAABB(const MeshType type) {
    switch (type) {
        case MeshType::Cube:
            return BoxColliderComponent{.half_extents = {0.5f, 0.5f, 0.5f}};
        case MeshType::Quad:
            return BoxColliderComponent{.half_extents = {0.5f, 0.5f, 0.0f}};
    }
    return BoxColliderComponent{};
}
