//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

layout(std140, binding = 0) uniform FrameUBO {
    mat4 view;
    mat4 projection;
    vec4 view_pos;
};

uniform mat4 model;

void main()
{
    vec4 worldPos   = model * vec4(aPos, 1.0);
    FragPos         = worldPos.xyz;
    Normal          = mat3(transpose(inverse(model))) * aNormal;
    TexCoord        = aTexCoord;
    gl_Position     = projection * view * worldPos;
}
