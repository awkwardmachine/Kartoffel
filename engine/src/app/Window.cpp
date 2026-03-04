//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#include "kartoffel/app/Window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window() : window_(nullptr), mouse_cursor_enabled_(true) {}

Window::~Window() { Shutdown(); }

bool Window::Initialize(const int width, const int height, const char *title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8); // MSAA

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

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
void Window::SetShouldClose(const bool close) const {
    glfwSetWindowShouldClose(window_, close ? GLFW_TRUE : GLFW_FALSE);
}
void Window::PollEvents() { glfwPollEvents(); }
void Window::SwapBuffers() const { glfwSwapBuffers(window_); }

int Window::GetWidth() const {
    int w, h;
    glfwGetWindowSize(window_, &w, &h);
    return w;
}

int Window::GetHeight() const {
    int w, h;
    glfwGetWindowSize(window_, &w, &h);
    return h;
}

void Window::SetMouseCursorEnabled(const bool enable) {
    glfwSetInputMode(window_, GLFW_CURSOR, enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    mouse_cursor_enabled_ = enable;
}

void Window::FramebufferSizeCallback(GLFWwindow * /*window*/, const int width, const int height) {
    glViewport(0, 0, width, height);
}
