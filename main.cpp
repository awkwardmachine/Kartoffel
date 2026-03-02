#include <iostream>

#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Window.hpp"

int main() {
    Window window;
    Shader shader;
    Mesh   triangle;

    if (!window.Initialize(800, 600, "Kartoffel")) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    if (Renderer renderer; !renderer.Initialize(window.GetNativeWindow())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    if (!shader.LoadFromFile("Shaders/triangle.vert", "Shaders/triangle.frag")) {
        std::cerr << "Failed to load Shaders" << std::endl;
        return -1;
    }

    if (!triangle.CreateTriangle()) {
        std::cerr << "Failed to create triangle" << std::endl;
        return -1;
    }

    while (!window.ShouldClose()) {
        Window::PollEvents();
        Renderer::Clear(0.2f, 0.3f, 0.3f, 1.0f);

        shader.Use();
        triangle.Render();

        window.SwapBuffers();
    }

    return 0;
}
