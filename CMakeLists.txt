##
## Created by Noah Belton (awkwardmachine) on 02/03/2026.
## Copyright (C) 2026 Noah Belton (awkwardmachine)
## SPDX-License-Identifier: GPL-3.0-only
##

cmake_minimum_required(VERSION 4.1)
project(Kartoffel)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
        tinygltf
        GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
        GIT_TAG v2.9.7
)
FetchContent_MakeAvailable(tinygltf)

FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG v2.0.0rc14.dev0
)
FetchContent_MakeAvailable(tinyobjloader)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

add_library(glad OBJECT external/glad/src/glad.c)
target_include_directories(glad PRIVATE external/glad/include)

# =============================================================================
#  Kartoffel - main engine executable
# =============================================================================

add_executable(Kartoffel
        src/main.cpp
        src/app/Window.cpp
        src/app/Window.hpp
        src/rendering/Renderer.cpp
        src/rendering/Renderer.hpp
        src/rendering/Shader.cpp
        src/rendering/Shader.hpp
        src/rendering/Texture.cpp
        src/rendering/Texture.hpp
        src/camera/Camera.cpp
        src/camera/Camera.hpp
        src/input/InputManager.cpp
        src/input/InputManager.hpp
        src/ecs/World.cpp
        src/ecs/World.hpp
        src/ecs/Entity.cpp
        src/ecs/Entity.hpp
        src/ecs/ComponentArray.hpp
        src/ecs/ComponentArray.inl
        src/ecs/ComponentRegistry.cpp
        src/ecs/ComponentRegistry.hpp
        src/ecs/ComponentRegistry.inl
        src/ecs/System.hpp
        src/ecs/Components.hpp
        src/ecs/systems/InputSystem.hpp
        src/ecs/systems/RenderSystem.hpp
        src/ecs/systems/InputSystem.cpp
        src/ecs/systems/RenderSystem.cpp
        src/ecs/MeshFactory.cpp
        src/ecs/MeshFactory.hpp
        src/ecs/systems/MeshCleanupSystem.cpp
        src/ecs/systems/MeshCleanupSystem.hpp
        src/assets/KAsset.hpp
        src/assets/AssetManager.hpp
        src/assets/AssetManager.cpp
        src/assets/SceneLoader.cpp
        src/assets/SceneLoader.hpp
)

target_link_libraries(Kartoffel PRIVATE
        glfw
        OpenGL::GL
        glad
        glm::glm
)

target_include_directories(Kartoffel PRIVATE
        external/glad/include
        ${stb_SOURCE_DIR}
        ${tinygltf_SOURCE_DIR}
        ${tinyobjloader_SOURCE_DIR}
        src
        src/assets
        src/camera
        src/input
        src/rendering
        src/app
        src/ecs
        src/ecs/systems
)

add_custom_command(TARGET Kartoffel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Kartoffel>/shaders
        COMMENT "Copying shaders to output directory"
)

add_custom_command(TARGET Kartoffel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:Kartoffel>/textures
        COMMENT "Copying textures to output directory"
)

# models/ is populated by BakeMeshes writing .kasset files directly into the
# build directory. No POST_BUILD copy needed for models.

add_custom_command(TARGET Kartoffel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/scenes
        $<TARGET_FILE_DIR:Kartoffel>/scenes
        COMMENT "Copying scenes to output directory"
)

# =============================================================================
#  BakeMeshes - standalone .kasset pre-baker, no GL context required
#  Usage: ./BakeMeshes [models_directory]   (defaults to ./models)
# =============================================================================

add_executable(BakeMeshes
        src/bake_main.cpp
        src/ecs/MeshFactory.cpp
        src/ecs/MeshFactory.hpp
        src/assets/KAsset.hpp
        src/rendering/Texture.cpp
        src/rendering/Texture.hpp
)

target_compile_definitions(BakeMeshes PRIVATE
        KARTOFFEL_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        KARTOFFEL_BUILD_DIR="$<TARGET_FILE_DIR:BakeMeshes>"
)

target_link_libraries(BakeMeshes PRIVATE
        glad
        glm::glm
)

target_include_directories(BakeMeshes PRIVATE
        external/glad/include
        ${stb_SOURCE_DIR}
        ${tinygltf_SOURCE_DIR}
        ${tinyobjloader_SOURCE_DIR}
        src
        src/assets
        src/ecs
        src/rendering
)

# BakeMeshes bakes directly into the source tree (root/models/) via the
# KARTOFFEL_SOURCE_DIR compile definition. No working directory setup needed.
