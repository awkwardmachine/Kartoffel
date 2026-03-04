##
## Created by Noah Belton (awkwardmachine) on 03/03/2026.
## Copyright (C) 2026 Noah Belton (awkwardmachine)
## SPDX-License-Identifier: GPL-3.0-only
##

set(ENGINE_SOURCES
        src/app/Application.cpp
    src/app/Window.cpp
    src/rendering/Renderer.cpp
    src/rendering/Shader.cpp
    src/rendering/Texture.cpp
    src/camera/Camera.cpp
    src/input/InputManager.cpp
    src/ecs/Entity.cpp
    src/ecs/ComponentRegistry.cpp
    src/ecs/World.cpp
    src/ecs/MeshFactory.cpp
    src/ecs/systems/InputSystem.cpp
    src/ecs/systems/RenderSystem.cpp
    src/ecs/systems/MeshCleanupSystem.cpp
    src/ecs/systems/PhysicsSystem.cpp
    src/ecs/systems/ScriptSystem.cpp
    src/assets/AssetManager.cpp
    src/assets/SceneLoader.cpp
)

add_library(KartoffelEngine STATIC ${ENGINE_SOURCES})

target_include_directories(KartoffelEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering
        ${CMAKE_CURRENT_SOURCE_DIR}/src/camera
        ${CMAKE_CURRENT_SOURCE_DIR}/src/input
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/systems
        ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
        ${CMAKE_CURRENT_SOURCE_DIR}/include/kartoffel
        ${stb_SOURCE_DIR}
        ${tinygltf_SOURCE_DIR}
        ${tinyobjloader_SOURCE_DIR}
)

target_link_libraries(KartoffelEngine
    PUBLIC
        glad
        glfw
        OpenGL::GL
        glm::glm
)
