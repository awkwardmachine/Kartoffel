//
// Created by nil on 02/03/2026.
//

#include "InputManager.hpp"
#include <iostream>

void InputManager::Initialize(GLFWwindow *window) {
    window_ = window;

    // Set callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetMouseButtonCallback(window_, MouseButtonCallback);
    glfwSetCursorPosCallback(window_, CursorPositionCallback);

    // Store pointer to this instance in GLFW window user pointer
    glfwSetWindowUserPointer(window_, this);
}

void InputManager::Update() {
    // Update key states
    prev_key_state_ = current_key_state_;

    // Update mouse states
    prev_mouse_state_ = current_mouse_state_;

    // Calculate mouse delta
    if (first_mouse_) {
        last_mouse_X_ = mouse_X_;
        last_mouse_Y_ = mouse_Y_;
        first_mouse_  = false;
    }

    mouse_delta_X_ = mouse_X_ - last_mouse_X_;
    mouse_delta_Y_ = last_mouse_Y_ - mouse_Y_;

    last_mouse_X_ = mouse_X_;
    last_mouse_Y_ = mouse_Y_;
}

void InputManager::ShutDown() {
    window_ = nullptr;
    current_key_state_.clear();
    prev_key_state_.clear();
    current_mouse_state_.clear();
    prev_mouse_state_.clear();
}

bool InputManager::IsKeyPressed(const int key) const {
    const auto it = current_key_state_.find(key);
    return it != current_key_state_.end() && it->second;
}

bool InputManager::IsKeyJustPressed(const int key) const {
    const auto current_it = current_key_state_.find(key);
    const auto prev_it    = prev_key_state_.find(key);

    const bool currently_pressed  = (current_it != current_key_state_.end() && current_it->second);
    const bool previously_pressed = (prev_it != prev_key_state_.end() && prev_it->second);

    return currently_pressed && !previously_pressed;
}

bool InputManager::IsKeyReleased(const int key) const {
    const auto current_it = current_key_state_.find(key);
    const auto prev_it    = prev_key_state_.find(key);

    const bool currently_pressed  = (current_it != current_key_state_.end() && current_it->second);
    const bool previously_pressed = (prev_it != prev_key_state_.end() && prev_it->second);

    return !currently_pressed && previously_pressed;
}

bool InputManager::IsMouseButtonPressed(const int button) const {
    const auto it = current_mouse_state_.find(button);
    return it != current_mouse_state_.end() && it->second;
}

double InputManager::GetMouseX() const { return mouse_X_; }

double InputManager::GetMouseY() const { return mouse_Y_; }

double InputManager::GetMouseDeltaX() const { return mouse_delta_X_; }

double InputManager::GetMouseDeltaY() const { return mouse_delta_Y_; }

void InputManager::KeyCallback(GLFWwindow *window, const int key, int scancode, const int action, int mods) {
    auto *input = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
    if (!input)
        return;

    if (action == GLFW_PRESS) {
        input->current_key_state_[key] = true;
    } else if (action == GLFW_RELEASE) {
        input->current_key_state_[key] = false;
    }
}

void InputManager::MouseButtonCallback(GLFWwindow *window, const int button, const int action, int mods) {
    auto *input = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
    if (!input)
        return;

    if (action == GLFW_PRESS) {
        input->current_mouse_state_[button] = true;
    } else if (action == GLFW_RELEASE) {
        input->current_mouse_state_[button] = false;
    }
}

void InputManager::CursorPositionCallback(GLFWwindow *window, const double x_pos, const double y_pos) {
    auto *input = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
    if (!input)
        return;

    input->mouse_X_ = x_pos;
    input->mouse_Y_ = y_pos;
}
