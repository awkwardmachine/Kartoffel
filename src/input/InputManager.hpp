//
// Created by nil on 02/03/2026.
//

#ifndef KARTOFFEL_INPUTMANAGER_HPP
#define KARTOFFEL_INPUTMANAGER_HPP

#include <unordered_map>
#include <GLFW/glfw3.h>

class InputManager {
public:
    InputManager() = default;

    void Initialize(GLFWwindow *window);
    void Update();
    void ShutDown();

    [[nodiscard]] bool IsKeyPressed(int key) const;
    [[nodiscard]] bool IsKeyJustPressed(int key) const;
    [[nodiscard]] bool IsKeyReleased(int key) const;

    [[nodiscard]] bool   IsMouseButtonPressed(int button) const;
    [[nodiscard]] double GetMouseX() const;
    [[nodiscard]] double GetMouseY() const;
    [[nodiscard]] double GetMouseDeltaX() const;
    [[nodiscard]] double GetMouseDeltaY() const;

private:
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow *window, double x_pos, double y_pos);

    GLFWwindow *window_{nullptr};
    // Key States
    std::unordered_map<int, bool> current_key_state_;
    std::unordered_map<int, bool> prev_key_state_;

    // Mouse States
    std::unordered_map<int, bool> current_mouse_state_;
    std::unordered_map<int, bool> prev_mouse_state_;

    double mouse_X_{0.0};
    double mouse_Y_{0.0};
    double mouse_delta_X_{0.0};
    double mouse_delta_Y_{0.0};
    double last_mouse_X_{0.0};
    double last_mouse_Y_{0.0};
    bool   first_mouse_{true};
};

#endif // KARTOFFEL_INPUTMANAGER_HPP
