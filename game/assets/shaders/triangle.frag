//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#version 420 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D uTexture;
uniform int       uHasTexture;

layout(std140, binding = 0) uniform FrameUBO {
    mat4 view;
    mat4 projection;
    vec4 view_pos;
};

#define MAX_LIGHTS 8

struct Light {
    vec4 color_intensity;
    vec4 position_type;
    vec4 direction_pad;
    vec4 attenuation;
    vec4 cutoffs;
};

layout(std140, binding = 1) uniform LightsUBO {
    Light uLights[MAX_LIGHTS];
    int   uLightCount;
};

// Helpers
vec3 CalcDirectional(Light light, vec3 norm, vec3 viewDir, vec3 baseColor) {
    vec3  col      = light.color_intensity.xyz;
    float intens   = light.color_intensity.w;
    vec3  lightDir = normalize(-light.direction_pad.xyz);

    vec3  ambient  = 0.05 * baseColor * col;
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = diff * baseColor * col;

    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), 64.0);
    vec3  specular = 0.3 * spec * col;

    return (ambient + diffuse + specular) * intens;
}

vec3 CalcPoint(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 baseColor) {
    vec3  col      = light.color_intensity.xyz;
    float intens   = light.color_intensity.w;
    vec3  pos      = light.position_type.xyz;
    float con      = light.attenuation.x;
    float lin      = light.attenuation.y;
    float quad     = light.attenuation.z;

    vec3  lightDir = normalize(pos - fragPos);
    float dist     = length(pos - fragPos);
    float atten    = 1.0 / (con + lin * dist + quad * dist * dist);

    vec3  ambient  = 0.05 * baseColor * col;
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = diff * baseColor * col;

    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), 64.0);
    vec3  specular = 0.3 * spec * col;

    return (ambient + diffuse + specular) * atten * intens;
}

vec3 CalcSpot(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 baseColor) {
    vec3  col      = light.color_intensity.xyz;
    float intens   = light.color_intensity.w;
    vec3  pos      = light.position_type.xyz;
    float con      = light.attenuation.x;
    float lin      = light.attenuation.y;
    float quad     = light.attenuation.z;
    float inner    = light.cutoffs.x;
    float outer    = light.cutoffs.y;

    vec3  lightDir = normalize(pos - fragPos);
    float dist     = length(pos - fragPos);
    float atten    = 1.0 / (con + lin * dist + quad * dist * dist);

    float theta   = dot(lightDir, normalize(-light.direction_pad.xyz));
    float epsilon = inner - outer;
    float cone    = clamp((theta - outer) / epsilon, 0.0, 1.0);

    vec3  ambient  = 0.05 * baseColor * col;
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = diff * baseColor * col;

    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), 64.0);
    vec3  specular = 0.3 * spec * col;

    return (ambient + (diffuse + specular) * cone) * atten * intens;
}

void main() {
    vec3 baseColor = (uHasTexture == 1)
    ? texture(uTexture, TexCoord).rgb
    : vec3(0.72, 0.72, 0.72);

    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(view_pos.xyz - FragPos);

    vec3 result = vec3(0.0);

    if (uLightCount == 0) {
        result = 0.15 * baseColor;
    } else {
        for (int i = 0; i < uLightCount && i < MAX_LIGHTS; ++i) {
            int type = int(uLights[i].position_type.w);
            if (type == 0)
            result += CalcDirectional(uLights[i], norm, viewDir, baseColor);
            else if (type == 1)
            result += CalcPoint(uLights[i], norm, FragPos, viewDir, baseColor);
            else if (type == 2)
            result += CalcSpot(uLights[i], norm, FragPos, viewDir, baseColor);
        }
    }

    FragColor = vec4(result, 1.0);
}
