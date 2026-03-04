//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include <kartoffel/assets/SceneLoader.hpp>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <kartoffel/assets/AssetManager.hpp>
#include <kartoffel/ecs/Components.hpp>
#include <kartoffel/ecs/MeshFactory.hpp>
#include <kartoffel/ecs/World.hpp>

namespace {

std::string TrimLine(const std::string &line) {
    const std::string s     = line.substr(0, line.find('#'));
    const size_t      start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::vector<std::string> Tokenise(const std::string &line) {
    std::vector<std::string> tokens;
    size_t i = 0;
    while (i < line.size()) {
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
        if (i >= line.size()) break;
        if (line[i] == '"') {
            ++i;
            const size_t start = i;
            while (i < line.size() && line[i] != '"') ++i;
            tokens.push_back(line.substr(start, i - start));
            if (i < line.size()) ++i;
        } else {
            const size_t start = i;
            while (i < line.size() && line[i] != ' ' && line[i] != '\t') ++i;
            tokens.push_back(line.substr(start, i - start));
        }
    }
    return tokens;
}

float ToFloat(const std::string &s) {
    try { return std::stof(s); } catch (...) { return 0.0f; }
}

// Serialize helpers
std::string P(const float f) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(4) << f;
    return ss.str();
}

std::string V3(const glm::vec3 &v) {
    return P(v.x) + " " + P(v.y) + " " + P(v.z);
}

struct EntityDesc {
    // tag
    std::string              name;
    std::vector<std::string> tags;

    // model
    std::string kasset_path;
    std::string vert_shader = "assets/shaders/triangle.vert";
    std::string frag_shader = "assets/shaders/triangle.frag";
    glm::vec3   position    = {0.0f, 0.0f, 0.0f};
    glm::vec3   rotation    = {0.0f, 0.0f, 0.0f};
    glm::vec3   scale       = {1.0f, 1.0f, 1.0f};

    bool has_model = false;

    // rigidbody
    bool      has_rigidbody = false;
    float     mass          = 1.0f;
    float     restitution   = 0.3f;
    float     linear_drag   = 0.98f;
    bool      use_gravity   = true;
    bool      is_kinematic  = false;
    glm::vec3 velocity      = {0.0f, 0.0f, 0.0f};

    // box collider
    bool           has_collider  = false;
    glm::vec3      half_extents  = {0.5f, 0.5f, 0.5f};
    glm::vec3      col_offset    = {0.0f, 0.0f, 0.0f};
    bool           is_trigger    = false;
    CollisionLayer col_layer     = LAYER_DEFAULT;
    CollisionLayer col_mask      = LAYER_ALL;
};

struct LightDesc {
    // tag
    std::string              name;
    std::vector<std::string> tags;

    LightType type      = LightType::Point;
    glm::vec3 color     = {1.0f, 1.0f, 1.0f};
    float     intensity = 1.0f;
    glm::vec3 position  = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, -1.0f, 0.0f};
    float     constant  = 1.0f;
    float     linear    = 0.09f;
    float     quadratic = 0.032f;
    float     inner_cutoff_deg = 12.5f;
    float     outer_cutoff_deg = 17.5f;
};

LightType ParseLightType(const std::string &s) {
    if (s == "directional") return LightType::Directional;
    if (s == "spot")        return LightType::Spot;
    return LightType::Point; // default
}

std::string LightTypeName(LightType t) {
    switch (t) {
        case LightType::Directional: return "directional";
        case LightType::Spot:        return "spot";
        default:                     return "point";
    }
}

} // namespace

bool SceneLoader::Load(const std::string &path, World &world) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "[SceneLoader] Cannot open: " << path << "\n";
        return false;
    }

    enum class BlockType { None, Entity, Light };

    BlockType   block     = BlockType::None;
    EntityDesc  cur_entity{};
    LightDesc   cur_light{};
    int         line_num  = 0;

    std::vector<EntityDesc> entities;
    std::vector<LightDesc>  lights;

    auto flush_entity = [&]() {
        if (cur_entity.has_model)
            entities.push_back(cur_entity);
        else
            std::cerr << "[SceneLoader] " << path << " entity missing model, skipping" << std::endl;
        cur_entity = {};
    };

    auto flush_light = [&]() {
        lights.push_back(cur_light);
        cur_light = {};
    };

    std::string raw;
    while (std::getline(file, raw)) {
        ++line_num;
        std::string line = TrimLine(raw);
        if (line.empty()) continue;
        auto tok = Tokenise(line);
        if (tok.empty()) continue;

        const std::string &key = tok[0];

        if (key == "entity") { block = BlockType::Entity; cur_entity = {}; continue; }
        if (key == "light")  { block = BlockType::Light;  cur_light  = {}; continue; }
        if (key == "{")      { continue; }
        if (key == "}") {
            if      (block == BlockType::Entity) flush_entity();
            else if (block == BlockType::Light)  flush_light();
            block = BlockType::None;
            continue;
        }

        if (block == BlockType::Entity) {
            if (key == "name"     && tok.size() >= 2) { cur_entity.name = tok[1]; }
            else if (key == "tags") { for (size_t i=1;i<tok.size();++i) cur_entity.tags.push_back(tok[i]); }
            else if (key == "model"    && tok.size() >= 2) { cur_entity.kasset_path = tok[1]; cur_entity.has_model = true; }
            else if (key == "shader"   && tok.size() >= 3) { cur_entity.vert_shader = tok[1]; cur_entity.frag_shader = tok[2]; }
            else if (key == "position" && tok.size() >= 4) { cur_entity.position = {ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            else if (key == "rotation" && tok.size() >= 4) { cur_entity.rotation = {ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            else if (key == "scale"    && tok.size() >= 4) { cur_entity.scale    = {ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            // rigidbody fields
            else if (key == "rigidbody")                    { cur_entity.has_rigidbody = true; }
            else if (key == "mass"        && tok.size()>=2) { cur_entity.mass = ToFloat(tok[1]); cur_entity.has_rigidbody = true; }
            else if (key == "restitution" && tok.size()>=2) { cur_entity.restitution = ToFloat(tok[1]); }
            else if (key == "linear_drag" && tok.size()>=2) { cur_entity.linear_drag = ToFloat(tok[1]); }
            else if (key == "use_gravity" && tok.size()>=2) { cur_entity.use_gravity = (tok[1]=="true"||tok[1]=="1"); }
            else if (key == "is_kinematic"&& tok.size()>=2) { cur_entity.is_kinematic=(tok[1]=="true"||tok[1]=="1"); }
            else if (key == "velocity"    && tok.size()>=4) { cur_entity.velocity={ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            // box collider fields
            else if (key == "collider")                     { cur_entity.has_collider = true; }
            else if (key == "half_extents"&& tok.size()>=4) { cur_entity.half_extents={ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; cur_entity.has_collider=true; }
            else if (key == "col_offset"  && tok.size()>=4) { cur_entity.col_offset={ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            else if (key == "is_trigger"  && tok.size()>=2) { cur_entity.is_trigger=(tok[1]=="true"||tok[1]=="1"); }
            else if (key == "col_layer"   && tok.size()>=2) { cur_entity.col_layer=static_cast<CollisionLayer>(std::stoul(tok[1])); }
            else if (key == "col_mask"    && tok.size()>=2) { cur_entity.col_mask =static_cast<CollisionLayer>(std::stoul(tok[1])); }
            else { std::cerr << "[SceneLoader] " << path << ":" << line_num << " unknown entity field '" << key << "'" << std::endl; }
        }
        else if (block == BlockType::Light) {
            if      (key == "name"      && tok.size()>=2) { cur_light.name = tok[1]; }
            else if (key == "tags")                        { for (size_t i=1;i<tok.size();++i) cur_light.tags.push_back(tok[i]); }
            else if (key == "type"      && tok.size()>=2) { cur_light.type = ParseLightType(tok[1]); }
            else if (key == "color"     && tok.size()>=4) { cur_light.color = {ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            else if (key == "intensity" && tok.size()>=2) { cur_light.intensity = ToFloat(tok[1]); }
            else if (key == "position"  && tok.size()>=4) { cur_light.position = {ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            else if (key == "direction" && tok.size()>=4) { cur_light.direction = {ToFloat(tok[1]),ToFloat(tok[2]),ToFloat(tok[3])}; }
            else if (key == "constant"  && tok.size()>=2) { cur_light.constant = ToFloat(tok[1]); }
            else if (key == "linear"    && tok.size()>=2) { cur_light.linear   = ToFloat(tok[1]); }
            else if (key == "quadratic" && tok.size()>=2) { cur_light.quadratic= ToFloat(tok[1]); }
            else if (key == "inner_cutoff" && tok.size()>=2) { cur_light.inner_cutoff_deg = ToFloat(tok[1]); }
            else if (key == "outer_cutoff" && tok.size()>=2) { cur_light.outer_cutoff_deg = ToFloat(tok[1]); }
            else { std::cerr << "[SceneLoader] " << path << ":" << line_num << " unknown light field '" << key << std::endl; }
        }
    }

    if (entities.empty() && lights.empty()) {
        std::cerr << "[SceneLoader] " << path << " no valid entities or lights\n";
        return false;
    }

    int spawned = 0;

    // spawn model entities
    for (const auto &ed : entities) {
        const unsigned int shader_id = AssetManager::GetShader(ed.vert_shader, ed.frag_shader);
        if (!shader_id) continue;

        const auto &meshes = AssetManager::GetMeshes(ed.kasset_path);
        if (meshes.empty()) continue;

        const glm::vec3 rot_rad = glm::radians(ed.rotation);

        for (const auto &[mesh, texture] : meshes) {
            const Entity e = world.CreateEntity();
            if (!ed.name.empty() || !ed.tags.empty())
                world.AddComponent<TagComponent>(e, TagComponent{.name = ed.name, .tags = ed.tags});
            world.AddComponent<TransformComponent>(e, TransformComponent{.position=ed.position,.rotation=rot_rad,.scale=ed.scale});
            world.AddComponent<MeshComponent>(e, mesh);
            world.AddComponent<ShaderComponent>(e, ShaderComponent{.id = shader_id});
            world.AddComponent<ModelComponent>(e, ModelComponent{.kasset_path=ed.kasset_path,.vert_shader=ed.vert_shader,.frag_shader=ed.frag_shader});
            if (texture.has_value())
                world.AddComponent<TextureComponent>(e, texture.value());
            if (ed.has_rigidbody)
                world.AddComponent<RigidBodyComponent>(e, RigidBodyComponent{
                    .velocity     = ed.velocity,
                    .mass         = ed.mass,
                    .restitution  = ed.restitution,
                    .linear_drag  = ed.linear_drag,
                    .use_gravity  = ed.use_gravity,
                    .is_kinematic = ed.is_kinematic,
                });
            if (ed.has_collider)
                world.AddComponent<BoxColliderComponent>(e, BoxColliderComponent{
                    .half_extents = ed.half_extents,
                    .offset       = ed.col_offset,
                    .is_trigger   = ed.is_trigger,
                    .layer        = ed.col_layer,
                    .mask         = ed.col_mask,
                });
        }
        ++spawned;
    }

    // Spawn light entities
    for (const auto &
        [name, tags, type, color, intensity,
            position, direction, constant, linear, quadratic,
                      inner_cutoff_deg, outer_cutoff_deg] : lights) {
        const Entity e = world.CreateEntity();
        if (!name.empty() || !tags.empty())
            world.AddComponent<TagComponent>(e, TagComponent{.name = name, .tags = tags});
        world.AddComponent<TransformComponent>(e, TransformComponent{.position = position});
        world.AddComponent<LightComponent>(e, LightComponent{
            .type              = type,
            .color             = color,
            .intensity         = intensity,
            .direction         = direction,
            .constant          = constant,
            .linear            = linear,
            .quadratic         = quadratic,
            .inner_cutoff_deg  = inner_cutoff_deg,
            .outer_cutoff_deg  = outer_cutoff_deg,
        });
        ++spawned;
    }

    std::cout << "[SceneLoader] Loaded '" << path << std::endl;
    return spawned > 0;
}

bool SceneLoader::Save(const std::string &path, const World &world) {
    std::ofstream file(path, std::ios::trunc);
    if (!file) {
        std::cerr << "[SceneLoader] Cannot write: " << path << std::endl;
        return false;
    }

    int count = 0;

    // Model entities
    struct WrittenModel { std::string kasset_path; glm::vec3 position; glm::vec3 rotation; };
    WrittenModel last_model{};
    bool         first_model = true;

    for (const Entity e : world.Query<ModelComponent, TransformComponent>()) {
        const auto &[kasset_path, vert_shader, frag_shader] = world.GetComponent<ModelComponent>(e);
        const auto &[position, rotation, scale]             = world.GetComponent<TransformComponent>(e);

        if (!first_model && kasset_path == last_model.kasset_path &&
            position == last_model.position && rotation == last_model.rotation)
            continue;

        const glm::vec3 rot_deg = glm::degrees(rotation);

        file << "entity {\n";
        if (world.HasComponent<TagComponent>(e)) {
            const auto &[name, tags] = world.GetComponent<TagComponent>(e);
            if (!name.empty()) file << "    name     \"" << name << "\"\n";
            if (!tags.empty()) {
                file << "    tags    ";
                for (const auto &t : tags) file << " \"" << t << "\"";
                file << "\n";
            }
        }
        file << "    model    \"" << kasset_path << "\"\n";
        file << "    shader   \"" << vert_shader << "\" \"" << frag_shader << "\"\n";
        file << "    position " << V3(position)  << "\n";
        file << "    rotation " << V3(rot_deg)   << "\n";
        file << "    scale    " << V3(scale)      << "\n";
        if (world.HasComponent<RigidBodyComponent>(e)) {
            const auto &rb = world.GetComponent<RigidBodyComponent>(e);
            file << "    mass         " << P(rb.mass)        << "\n";
            file << "    restitution  " << P(rb.restitution) << "\n";
            file << "    linear_drag  " << P(rb.linear_drag) << "\n";
            file << "    use_gravity  " << (rb.use_gravity  ? "true" : "false") << "\n";
            file << "    is_kinematic " << (rb.is_kinematic ? "true" : "false") << "\n";
            file << "    velocity     " << V3(rb.velocity)   << "\n";
        }
        if (world.HasComponent<BoxColliderComponent>(e)) {
            const auto &col = world.GetComponent<BoxColliderComponent>(e);
            file << "    half_extents " << V3(col.half_extents) << "\n";
            file << "    col_offset   " << V3(col.offset)       << "\n";
            file << "    is_trigger   " << (col.is_trigger ? "true" : "false") << "\n";
            file << "    col_layer    " << col.layer << "\n";
            file << "    col_mask     " << col.mask  << "\n";
        }
        file << "}\n\n";

        last_model  = {kasset_path, position, rotation};
        first_model = false;
        ++count;
    }

    // Light entities
    for (const Entity e : world.Query<LightComponent>()) {
        const auto &lc = world.GetComponent<LightComponent>(e);
        glm::vec3 pos = {};
        if (world.HasComponent<TransformComponent>(e))
            pos = world.GetComponent<TransformComponent>(e).position;

        file << "light {\n";
        if (world.HasComponent<TagComponent>(e)) {
            const auto &[name, tags] = world.GetComponent<TagComponent>(e);
            if (!name.empty()) file << "    name     \"" << name << "\"\n";
            if (!tags.empty()) {
                file << "    tags    ";
                for (const auto &t : tags) file << " \"" << t << "\"";
                file << "\n";
            }
        }
        file << "    type      " << LightTypeName(lc.type)        << "\n";
        file << "    color     " << V3(lc.color)                  << "\n";
        file << "    intensity " << P(lc.intensity)               << "\n";
        file << "    position  " << V3(pos)                       << "\n";
        file << "    direction " << V3(lc.direction)              << "\n";
        file << "    constant  " << P(lc.constant)                << "\n";
        file << "    linear    " << P(lc.linear)                  << "\n";
        file << "    quadratic " << P(lc.quadratic)               << "\n";
        if (lc.type == LightType::Spot) {
            file << "    inner_cutoff " << P(lc.inner_cutoff_deg) << "\n";
            file << "    outer_cutoff " << P(lc.outer_cutoff_deg) << "\n";
        }
        file << "}\n\n";
        ++count;
    }

    std::cout << "[SceneLoader] Saved '" << path << "' " << count << " object(s)" << std::endl;
    return file.good();
}
