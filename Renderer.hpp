//
// Created by nil on 02/03/2026.
//

#ifndef KARTOFFEL_RENDERER_HPP
#define KARTOFFEL_RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize(const GLFWwindow* window);
    void Shutdown();

    static void Clear(float r, float g, float b, float a);

    [[nodiscard]] bool IsInitialized() const { return initialized_; }

private:
    bool initialized_;
};

#endif //KARTOFFEL_RENDERER_HPP