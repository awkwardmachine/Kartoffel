//
// Created by nil on 02/03/2026.
//

#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window() : window_(nullptr), width_(0), height_(0) {}

Window::~Window() { Shutdown(); }

bool Window::Initialize(const int width, const int height, const char *title) {
    width_  = width;
    height_ = height;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);
    glfwSetWindowUserPointer(window_, this);

    return true;
}

void Window::Shutdown() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(window_); }

void Window::PollEvents() { glfwPollEvents(); }

void Window::SwapBuffers() const { glfwSwapBuffers(window_); }

void Window::FramebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
    if (auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window))) {
        win->width_  = width;
        win->height_ = height;
    }
}
