//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/ecs/systems/RenderSystem.hpp"

#include <algorithm>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kartoffel/ecs/Components.hpp"
#include "kartoffel/ecs/World.hpp"
#include "kartoffel/rendering/Renderer.hpp"
#include "kartoffel/app/Window.hpp"

RenderSystem::RenderSystem(Window *window) : window_(window) {}

static GLint Loc(const GLuint prog, const std::string &name) { return glGetUniformLocation(prog, name.c_str()); }

void RenderSystem::Update(World &world, float /*dt*/) {
    Renderer::Clear(0.6196f, 0.8784f, 1.0f, 1.0f);

    const auto cameras = world.Query<CameraComponent>();
    if (cameras.empty())
        return;

    const CameraComponent &cam    = world.GetComponent<CameraComponent>(cameras[0]);
    const float            aspect = static_cast<float>(window_->GetWidth()) / static_cast<float>(window_->GetHeight());

    const glm::mat4 view       = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
    const glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspect, 0.1f, 500.0f);

    struct GpuLight {
        int       type;
        glm::vec3 color;
        float     intensity;
        glm::vec3 position;
        glm::vec3 direction;
        float     constant;
        float     linear;
        float     quadratic;
        float     innerCutoff;
        float     outerCutoff;
    };

    std::vector<GpuLight> lights;
    for (const Entity le: world.Query<LightComponent>()) {
        if (constexpr int MAX_LIGHTS = 8; static_cast<int>(lights.size()) >= MAX_LIGHTS)
            break;

        const LightComponent &lc = world.GetComponent<LightComponent>(le);

        GpuLight gl{};
        gl.type        = static_cast<int>(lc.type);
        gl.color       = lc.color;
        gl.intensity   = lc.intensity;
        gl.direction   = glm::normalize(lc.direction);
        gl.constant    = lc.constant;
        gl.linear      = lc.linear;
        gl.quadratic   = lc.quadratic;
        gl.innerCutoff = std::cos(glm::radians(lc.inner_cutoff_deg));
        gl.outerCutoff = std::cos(glm::radians(lc.outer_cutoff_deg));

        if (world.HasComponent<TransformComponent>(le))
            gl.position = world.GetComponent<TransformComponent>(le).position;

        lights.push_back(gl);
    }
    const int light_count = static_cast<int>(lights.size());

    for (const Entity e: world.Query<TransformComponent, MeshComponent, ShaderComponent>()) {
        const auto &[position, rotation, scale] = world.GetComponent<TransformComponent>(e);
        const MeshComponent   &mesh             = world.GetComponent<MeshComponent>(e);
        const ShaderComponent &shd              = world.GetComponent<ShaderComponent>(e);
        const GLuint           id               = shd.id;

        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, position);
        model      = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
        model      = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
        model      = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
        model      = glm::scale(model, scale);

        glUseProgram(id);

        glUniformMatrix4fv(Loc(id, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(Loc(id, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(Loc(id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(Loc(id, "uViewPos"), 1, glm::value_ptr(cam.position));

        const bool has_tex = world.HasComponent<TextureComponent>(e);
        glUniform1i(Loc(id, "uHasTexture"), has_tex ? 1 : 0);
        if (has_tex) {
            const TextureComponent &tex = world.GetComponent<TextureComponent>(e);
            glActiveTexture(GL_TEXTURE0 + tex.slot);
            glBindTexture(GL_TEXTURE_2D, tex.id);
            glUniform1i(Loc(id, "uTexture"), static_cast<int>(tex.slot));
        }

        glUniform1i(Loc(id, "uLightCount"), light_count);
        for (int i = 0; i < light_count; ++i) {
            const std::string base = "uLights[" + std::to_string(i) + "].";
            const GpuLight   &gl   = lights[i];
            glUniform1i(Loc(id, base + "type"), gl.type);
            glUniform3fv(Loc(id, base + "color"), 1, glm::value_ptr(gl.color));
            glUniform1f(Loc(id, base + "intensity"), gl.intensity);
            glUniform3fv(Loc(id, base + "position"), 1, glm::value_ptr(gl.position));
            glUniform3fv(Loc(id, base + "direction"), 1, glm::value_ptr(gl.direction));
            glUniform1f(Loc(id, base + "constant"), gl.constant);
            glUniform1f(Loc(id, base + "linear"), gl.linear);
            glUniform1f(Loc(id, base + "quadratic"), gl.quadratic);
            glUniform1f(Loc(id, base + "innerCutoff"), gl.innerCutoff);
            glUniform1f(Loc(id, base + "outerCutoff"), gl.outerCutoff);
        }

        glBindVertexArray(mesh.vao);
        if (mesh.use_indices)
            glDrawElements(GL_TRIANGLES, mesh.vertex_count, GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
