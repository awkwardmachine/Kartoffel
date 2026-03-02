#include "Renderer.hpp"
#include "Window.hpp"
#include <iostream>

int main() {
    Window window;

    if (!window.Initialize(800, 600, "Kartoffel")) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    if (Renderer renderer; !renderer.Initialize(window.GetNativeWindow())) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    while (!window.ShouldClose()) {
        Window::PollEvents();
        Renderer::Clear(0.2f, 0.3f, 0.3f, 1.0f);

        window.SwapBuffers();
    }

    return 0;
}