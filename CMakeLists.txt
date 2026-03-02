cmake_minimum_required(VERSION 4.1)
project(Kartoffel)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad OBJECT External/glad/src/glad.c)
target_include_directories(glad PRIVATE External/glad/include)

add_executable(Kartoffel
        main.cpp
        Window.cpp
        Window.hpp
        Renderer.cpp
        Renderer.hpp
        Shader.cpp
        Shader.hpp
        Mesh.cpp
        Mesh.hpp
)

target_link_libraries(Kartoffel PRIVATE
        glfw
        OpenGL::GL
        glad
)

target_include_directories(Kartoffel PRIVATE
        External/glad/include
)

add_custom_command(TARGET Kartoffel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
        $<TARGET_FILE_DIR:Kartoffel>/Shaders
        COMMENT "Copying shaders to output directory"
)