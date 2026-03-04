//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_RENDERSYSTEM_HPP
#define KARTOFFEL_RENDERSYSTEM_HPP

#include <glad/glad.h>
#include "ecs/System.hpp"

class Window;
class World;

class RenderSystem : public System {
public:
    explicit RenderSystem(Window *window);

    static constexpr int MAX_LIGHTS    = 8;

    void Init(World &world) override;
    void Update(World &world, float dt) override;
    void Shutdown(World &world) override;

private:
    Window  *window_;
    GLuint   ubo_frame_{0};   // binding point 0 – FrameUBO
    GLuint   ubo_lights_{0};  // binding point 1 – LightsUBO
    static constexpr int UBO_FRAME_BP  = 0;
    static constexpr int UBO_LIGHTS_BP = 1;

    void CreateUBOs();
    void UploadFrameUBO(const struct FrameUBOData &data) const;
    void UploadLightsUBO(const struct LightsUBOData &data) const;
    static void BindShaderUBOBlocks(GLuint prog);
};

#endif // KARTOFFEL_RENDERSYSTEM_HPP
