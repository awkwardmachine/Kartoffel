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

// Material
uniform sampler2D uTexture;
uniform int       uHasTexture;

// Camera
uniform vec3      uViewPos;

// Lights
#define MAX_LIGHTS 8

struct Light {
    int   type;

    vec3  color;
    float intensity;

    vec3  position;
    vec3  direction;

    float constant;
    float linear;
    float quadratic;

    float innerCutoff;
    float outerCutoff;
};

uniform Light uLights[MAX_LIGHTS];
uniform int   uLightCount;

// Helpers
vec3 CalcDirectional(Light light, vec3 norm, vec3 viewDir, vec3 baseColor) {
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = 0.05 * baseColor * light.color;

    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * baseColor * light.color;

    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), 64.0);
    vec3  specular = 0.3 * spec * light.color;

    return (ambient + diffuse + specular) * light.intensity;
}

vec3 CalcPoint(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 baseColor) {
    vec3  lightDir = normalize(light.position - fragPos);
    float dist     = length(light.position - fragPos);
    float atten    = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient  = 0.05 * baseColor * light.color;
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * baseColor * light.color;

    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), 64.0);
    vec3  specular = 0.3 * spec * light.color;

    return (ambient + diffuse + specular) * atten * light.intensity;
}

vec3 CalcSpot(Light light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 baseColor) {
    vec3  lightDir = normalize(light.position - fragPos);
    float dist     = length(light.position - fragPos);
    float atten    = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    float theta   = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float cone    = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 ambient  = 0.05 * baseColor * light.color;
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * baseColor * light.color;

    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), 64.0);
    vec3  specular = 0.3 * spec * light.color;

    return (ambient + (diffuse + specular) * cone) * atten * light.intensity;
}

// Main
void main() {
    vec3 baseColor = (uHasTexture == 1)
        ? texture(uTexture, TexCoord).rgb
        : vec3(0.72, 0.72, 0.72);

    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(uViewPos - FragPos);

    vec3 result = vec3(0.0);

    if (uLightCount == 0) {
        result = 0.15 * baseColor;
    } else {
        for (int i = 0; i < uLightCount && i < MAX_LIGHTS; ++i) {
            if (uLights[i].type == 0)
                result += CalcDirectional(uLights[i], norm, viewDir, baseColor);
            else if (uLights[i].type == 1)
                result += CalcPoint(uLights[i], norm, FragPos, viewDir, baseColor);
            else if (uLights[i].type == 2)
                result += CalcSpot(uLights[i], norm, FragPos, viewDir, baseColor);
        }
    }

    FragColor = vec4(result, 1.0);
}
