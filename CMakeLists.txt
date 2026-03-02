cmake_minimum_required(VERSION 4.1)
project(Kartoffel)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad OBJECT external/glad/src/glad.c)
target_include_directories(glad PRIVATE external/glad/include)

add_executable(Kartoffel
        src/main.cpp
        src/app/Window.cpp
        src/app/Window.hpp
        src/rendering/Renderer.cpp
        src/rendering/Renderer.hpp
        src/rendering/Shader.cpp
        src/rendering/Shader.hpp
        src/rendering/Mesh.cpp
        src/rendering/Mesh.hpp
        src/camera/Camera.cpp
        src/camera/Camera.hpp
        src/input/InputManager.cpp
        src/input/InputManager.hpp
)

target_link_libraries(Kartoffel PRIVATE
        glfw
        OpenGL::GL
        glad
        glm::glm
)

target_include_directories(Kartoffel PRIVATE
        external/glad/include
        src
        src/camera
        src/input
        src/rendering
        src/app
)

add_custom_command(TARGET Kartoffel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Kartoffel>/shaders
        COMMENT "Copying shaders to output directory"
)