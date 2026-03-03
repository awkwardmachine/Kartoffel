//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "RenderSystem.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Components.hpp"
#include "World.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

RenderSystem::RenderSystem(Window *window) : window_(window) {}

void RenderSystem::Update(World &world, float dt) {
    Renderer::Clear(0.6196f, 0.8784f, 1.0f, 1.0f);

    const auto cameras = world.Query<CameraComponent>();
    if (cameras.empty())
        return;

    const CameraComponent &cam = world.GetComponent<CameraComponent>(cameras[0]);

    const float aspect = static_cast<float>(window_->GetWidth()) / static_cast<float>(window_->GetHeight());

    glm::mat4 view       = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), aspect, 0.1f, 100.0f);

    for (const Entity e: world.Query<TransformComponent, MeshComponent, ShaderComponent>()) {
        const auto &[position, rotation, scale] = world.GetComponent<TransformComponent>(e);
        const MeshComponent   &mesh             = world.GetComponent<MeshComponent>(e);
        const ShaderComponent &shd              = world.GetComponent<ShaderComponent>(e);
        const unsigned int     id               = shd.id;

        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, position);
        model      = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
        model      = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
        model      = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
        model      = glm::scale(model, scale);

        glUseProgram(id);

        const bool has_tex = world.HasComponent<TextureComponent>(e);
        glUniform1i(glGetUniformLocation(id, "uHasTexture"), has_tex ? 1 : 0);
        if (has_tex) {
            const TextureComponent &tex = world.GetComponent<TextureComponent>(e);
            glActiveTexture(GL_TEXTURE0 + tex.slot);
            glBindTexture(GL_TEXTURE_2D, tex.id);
            glUniform1i(glGetUniformLocation(id, "uTexture"), static_cast<int>(tex.slot));
        }

        glUniform3f(glGetUniformLocation(id, "uLightPos"), 4.0f, 6.0f, 4.0f);
        glUniform3fv(glGetUniformLocation(id, "uViewPos"), 1, &cam.position[0]);
        glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(mesh.vao);
        if (mesh.use_indices)
            glDrawElements(GL_TRIANGLES, mesh.vertex_count, GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
