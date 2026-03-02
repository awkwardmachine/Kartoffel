#include <glad/glad.h>
#include <iostream>

#include "Camera.hpp"
#include "InputManager.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Window.hpp"

int main() {
    Window       window;
    Shader       shader;
    Mesh         triangle;
    Camera       camera;
    InputManager input;

    if (!window.Initialize(800, 600, "Kartoffel")) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    if (Renderer renderer; !renderer.Initialize(window.GetNativeWindow())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    input.Initialize(window.GetNativeWindow());

    if (!shader.LoadFromFile("Shaders/triangle.vert", "Shaders/triangle.frag")) {
        std::cerr << "Failed to load Shaders" << std::endl;
        return -1;
    }

    if (!triangle.CreateTriangle()) {
        std::cerr << "Failed to create triangle" << std::endl;
        return -1;
    }

    camera.SetPosition(0.0f, 0.0f, 3.0f);
    auto model = glm::mat4(1.0f); // position triangle in world space

    while (!window.ShouldClose()) {
        Window::PollEvents();
        input.Update();

        if (input.IsKeyPressed(GLFW_KEY_W))
            camera.Move(0.015f, 0.0f);
        if (input.IsKeyPressed(GLFW_KEY_S))
            camera.Move(-0.015f, 0.0f);
        if (input.IsKeyPressed(GLFW_KEY_A))
            camera.Move(0.0f, -0.015f);
        if (input.IsKeyPressed(GLFW_KEY_D))
            camera.Move(0.0f, 0.015f);

        Renderer::Clear(0.2f, 0.3f, 0.3f, 1.0f);

        const float aspect_ratio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
        glm::mat4   view         = camera.GetViewMatrix();
        glm::mat4   projection   = Camera::GetProjectionMatrix(aspect_ratio);

        shader.Use();

        const GLint model_loc = glGetUniformLocation(shader.GetId(), "model");
        const GLint view_loc  = glGetUniformLocation(shader.GetId(), "view");
        const GLint proj_loc  = glGetUniformLocation(shader.GetId(), "projection");

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &projection[0][0]);

        triangle.Render();

        window.SwapBuffers();
    }

    return 0;
}
