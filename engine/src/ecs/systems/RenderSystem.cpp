//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/ecs/systems/RenderSystem.hpp"

#include <cmath>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kartoffel/ecs/Components.hpp"
#include "kartoffel/ecs/World.hpp"
#include "kartoffel/rendering/Renderer.hpp"
#include "kartoffel/app/Window.hpp"

struct alignas(16) FrameUBOData {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 view_pos;
};

struct alignas(16) GpuLight {
    glm::vec4 color_intensity;
    glm::vec4 position_type;
    glm::vec4 direction_pad;
    glm::vec4 attenuation;
    glm::vec4 cutoffs;
};

struct alignas(16) LightsUBOData {
    GpuLight lights[RenderSystem::MAX_LIGHTS];
    int      count;
    float    _pad[3];
};

RenderSystem::RenderSystem(Window *window) : window_(window) {}

void RenderSystem::Init(World & /*world*/) { CreateUBOs(); }

void RenderSystem::CreateUBOs() {
    glGenBuffers(1, &ubo_frame_);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_frame_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(FrameUBOData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, UBO_FRAME_BP, ubo_frame_);

    glGenBuffers(1, &ubo_lights_);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_lights_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightsUBOData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, UBO_LIGHTS_BP, ubo_lights_);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderSystem::UploadFrameUBO(const FrameUBOData &data) const {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_frame_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FrameUBOData), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderSystem::UploadLightsUBO(const LightsUBOData &data) const {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_lights_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightsUBOData), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderSystem::BindShaderUBOBlocks(const GLuint prog) {
    if (const GLuint frame_idx = glGetUniformBlockIndex(prog, "FrameUBO"); frame_idx != GL_INVALID_INDEX)
        glUniformBlockBinding(prog, frame_idx, UBO_FRAME_BP);
    else
        std::cerr << "[RenderSystem] shader " << prog << " missing 'FrameUBO' block\n";

    if (const GLuint lights_idx = glGetUniformBlockIndex(prog, "LightsUBO"); lights_idx != GL_INVALID_INDEX)
        glUniformBlockBinding(prog, lights_idx, UBO_LIGHTS_BP);
    else
        std::cerr << "[RenderSystem] shader " << prog << " missing 'LightsUBO' block\n";
}

void RenderSystem::Shutdown(World & /*world*/) {
    glDeleteBuffers(1, &ubo_frame_);
    glDeleteBuffers(1, &ubo_lights_);
    ubo_frame_ = ubo_lights_ = 0;
}

void RenderSystem::Update(World &world, float /*dt*/) {
    Renderer::Clear(0.6196f, 0.8784f, 1.0f, 1.0f);

    const auto cameras = world.Query<CameraComponent>();
    if (cameras.empty())
        return;

    const CameraComponent &cam    = world.GetComponent<CameraComponent>(cameras[0]);
    const float            aspect = static_cast<float>(window_->GetWidth()) / static_cast<float>(window_->GetHeight());

    // FrameUBO
    FrameUBOData frame{};
    frame.view       = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
    frame.projection = glm::perspective(glm::radians(cam.fov), aspect, 0.1f, 500.0f);
    frame.view_pos   = glm::vec4(cam.position, 0.0f);
    UploadFrameUBO(frame);

    // LightsUBO
    LightsUBOData lights_data{};
    lights_data.count = 0;

    for (const Entity le: world.Query<LightComponent>()) {
        if (lights_data.count >= MAX_LIGHTS)
            break;

        const LightComponent &lc = world.GetComponent<LightComponent>(le);
        auto &[color_intensity, position_type, direction_pad, attenuation, cutoffs] =
                lights_data.lights[lights_data.count++];

        color_intensity = glm::vec4(lc.color, lc.intensity);
        direction_pad   = glm::vec4(glm::normalize(lc.direction), 0.0f);
        attenuation     = glm::vec4(lc.constant, lc.linear, lc.quadratic, 0.0f);
        cutoffs = glm::vec4(std::cos(glm::radians(lc.inner_cutoff_deg)), std::cos(glm::radians(lc.outer_cutoff_deg)),
                            0.0f, 0.0f);

        glm::vec3 pos{0.0f};
        if (world.HasComponent<TransformComponent>(le))
            pos = world.GetComponent<TransformComponent>(le).position;

        position_type = glm::vec4(pos, static_cast<float>(static_cast<int>(lc.type)));
    }
    UploadLightsUBO(lights_data);

    // Draw Mesh
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

        BindShaderUBOBlocks(id);

        glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, glm::value_ptr(model));

        const bool has_tex = world.HasComponent<TextureComponent>(e);
        glUniform1i(glGetUniformLocation(id, "uHasTexture"), has_tex ? 1 : 0);
        if (has_tex) {
            const TextureComponent &tex = world.GetComponent<TextureComponent>(e);
            glActiveTexture(GL_TEXTURE0 + tex.slot);
            glBindTexture(GL_TEXTURE_2D, tex.id);
            glUniform1i(glGetUniformLocation(id, "uTexture"), static_cast<int>(tex.slot));
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
