//
// Created by Noah Belton (awkwardmachine) on 03/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "SceneLoader.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AssetManager.hpp"
#include "ecs/Components.hpp"
#include "ecs/MeshFactory.hpp"
#include "ecs/World.hpp"

namespace {

    std::string TrimLine(const std::string &line) {
        const std::string s     = line.substr(0, line.find('#'));
        const size_t      start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
            return "";
        const size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    std::vector<std::string> Tokenise(const std::string &line) {
        std::vector<std::string> tokens;
        size_t                   i = 0;
        while (i < line.size()) {
            while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
                ++i;
            if (i >= line.size())
                break;
            if (line[i] == '"') {
                ++i;
                const size_t start = i;
                while (i < line.size() && line[i] != '"')
                    ++i;
                tokens.push_back(line.substr(start, i - start));
                if (i < line.size())
                    ++i;
            } else {
                size_t start = i;
                while (i < line.size() && line[i] != ' ' && line[i] != '\t')
                    ++i;
                tokens.push_back(line.substr(start, i - start));
            }
        }
        return tokens;
    }

    float ToFloat(const std::string &s) {
        try {
            return std::stof(s);
        } catch (...) {
            return 0.0f;
        }
    }

    struct EntityDesc {
        std::string kasset_path;
        std::string vert_shader = "shaders/triangle.vert";
        std::string frag_shader = "shaders/triangle.frag";
        glm::vec3   position    = {0.0f, 0.0f, 0.0f};
        glm::vec3   rotation    = {0.0f, 0.0f, 0.0f};
        glm::vec3   scale       = {1.0f, 1.0f, 1.0f};
    };

} // namespace

bool SceneLoader::Load(const std::string &path, World &world) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "[SceneLoader] Cannot open: " << path << "\n";
        return false;
    }

    std::vector<EntityDesc> entities;
    EntityDesc              current;
    bool                    in_entity = false;
    int                     line_num  = 0;

    std::string raw_line;
    while (std::getline(file, raw_line)) {
        ++line_num;
        std::string line = TrimLine(raw_line);
        if (line.empty())
            continue;
        auto tokens = Tokenise(line);
        if (tokens.empty())
            continue;

        if (tokens[0] == "entity") {
            in_entity = true;
            current   = EntityDesc{};
            continue;
        }
        if (tokens[0] == "{")
            continue;
        if (tokens[0] == "}") {
            if (!in_entity)
                continue;
            if (current.kasset_path.empty()) {
                std::cerr << "[SceneLoader] " << path << ":" << line_num << " entity missing model, skipping\n";
            } else {
                entities.push_back(current);
            }
            in_entity = false;
            continue;
        }
        if (!in_entity)
            continue;

        const std::string &key = tokens[0];
        if (key == "model" && tokens.size() >= 2) {
            current.kasset_path = tokens[1];
        } else if (key == "shader" && tokens.size() >= 3) {
            current.vert_shader = tokens[1];
            current.frag_shader = tokens[2];
        } else if (key == "position" && tokens.size() >= 4) {
            current.position = {ToFloat(tokens[1]), ToFloat(tokens[2]), ToFloat(tokens[3])};
        } else if (key == "rotation" && tokens.size() >= 4) {
            current.rotation = {ToFloat(tokens[1]), ToFloat(tokens[2]), ToFloat(tokens[3])};
        } else if (key == "scale" && tokens.size() >= 4) {
            current.scale = {ToFloat(tokens[1]), ToFloat(tokens[2]), ToFloat(tokens[3])};
        } else {
            std::cerr << "[SceneLoader] " << path << ":" << line_num << " unknown field '" << key << "'\n";
        }
    }

    if (entities.empty()) {
        std::cerr << "[SceneLoader] " << path << " no valid entities\n";
        return false;
    }

    int spawned = 0;
    for (const auto &[kasset_path, vert_shader, frag_shader, position, rotation, scale]: entities) {
        const unsigned int shader_id = AssetManager::GetShader(vert_shader, frag_shader);
        if (!shader_id)
            continue;

        const auto &meshes = AssetManager::GetMeshes(kasset_path);
        if (meshes.empty())
            continue;

        const glm::vec3 rotation_rad = glm::radians(rotation);

        for (const MeshResult &r: meshes) {
            const Entity e = world.CreateEntity();
            world.AddComponent<TransformComponent>(e, TransformComponent{
                                                              .position = position,
                                                              .rotation = rotation_rad,
                                                              .scale    = scale,
                                                      });
            world.AddComponent<MeshComponent>(e, r.mesh);
            world.AddComponent<ShaderComponent>(e, ShaderComponent{.id = shader_id});
            world.AddComponent<ModelComponent>(e, ModelComponent{
                                                          .kasset_path = kasset_path,
                                                          .vert_shader = vert_shader,
                                                          .frag_shader = frag_shader,
                                                  });
            if (r.texture.has_value())
                world.AddComponent<TextureComponent>(e, r.texture.value());
        }
        ++spawned;
    }

    std::cout << "[SceneLoader] Loaded '" << path << "' " << spawned << " entity/entities\n";
    return spawned > 0;
}

bool SceneLoader::Save(const std::string &path, const World &world) {
    std::ofstream file(path, std::ios::trunc);
    if (!file) {
        std::cerr << "[SceneLoader] Cannot write: " << path << "\n";
        return false;
    }

    struct Written {
        std::string kasset_path;
        glm::vec3   position;
        glm::vec3   rotation;
    };
    Written last{};
    bool    first = true;
    int     count = 0;

    const auto p = [](const float f) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(4) << f;
        return ss.str();
    };

    for (const Entity e: world.Query<ModelComponent, TransformComponent>()) {
        const auto &[kasset_path, vert_shader, frag_shader] = world.GetComponent<ModelComponent>(e);
        const auto &[position, rotation, scale]             = world.GetComponent<TransformComponent>(e);

        if (!first && kasset_path == last.kasset_path && position == last.position && rotation == last.rotation) {
            continue;
        }

        const glm::vec3 rot_deg = glm::degrees(rotation);

        file << "entity {\n";
        file << "    model    \"" << kasset_path << "\"\n";
        file << "    shader   \"" << vert_shader << "\" \"" << frag_shader << "\"\n";
        file << "    position " << p(position.x) << " " << p(position.y) << " " << p(position.z) << "\n";
        file << "    rotation " << p(rot_deg.x) << " " << p(rot_deg.y) << " " << p(rot_deg.z) << "\n";
        file << "    scale    " << p(scale.x) << " " << p(scale.y) << " " << p(scale.z) << "\n";
        file << "}\n\n";

        last  = {kasset_path, position, rotation};
        first = false;
        ++count;
    }

    std::cout << "[SceneLoader] Saved '" << path << "' " << count << " entity/entities\n";
    return file.good();
}
