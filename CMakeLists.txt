##
## Created by Noah Belton (awkwardmachine) on 02/03/2026.
## Copyright (C) 2026 Noah Belton (awkwardmachine)
## SPDX-License-Identifier: GPL-3.0-only
##

cmake_minimum_required(VERSION 3.20)
project(Kartoffel VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(tinygltf
        GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
        GIT_TAG v2.9.7)
FetchContent_MakeAvailable(tinygltf)

FetchContent_Declare(tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG v2.0.0rc13)
FetchContent_MakeAvailable(tinyobjloader)

find_package(OpenGL REQUIRED)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(tools)
