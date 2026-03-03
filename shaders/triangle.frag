//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D uTexture;
uniform int       uHasTexture;
uniform vec3      uLightPos;
uniform vec3      uViewPos;

void main()
{
    vec3 baseColor = (uHasTexture == 1)
    ? texture(uTexture, TexCoord).rgb
    : vec3(0.72, 0.72, 0.72);

    vec3  norm     = normalize(Normal);
    vec3  lightDir = normalize(uLightPos - FragPos);

    // ambient
    vec3 ambient = 0.15 * baseColor;

    // diffuse
    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * baseColor;

    // specular
    vec3  viewDir    = normalize(uViewPos - FragPos);
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3  specular   = 0.4 * spec * vec3(1.0);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
