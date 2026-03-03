//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos   = model * vec4(aPos, 1.0);
    FragPos         = worldPos.xyz;
    Normal          = mat3(transpose(inverse(model))) * aNormal;
    TexCoord        = aTexCoord;
    gl_Position     = projection * view * worldPos;
}
