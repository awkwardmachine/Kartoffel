//
// Created by Noah Belton (awkwardmachine) on 02/03/2026.
// Copyright (C) 2026 Noah Belton (awkwardmachine)
// SPDX-License-Identifier: GPL-3.0-only
//

#ifndef KARTOFFEL_WINDOW_HPP
#define KARTOFFEL_WINDOW_HPP

struct GLFWwindow;

class Window {
public:
    Window();
    ~Window();

    bool Initialize(int width, int height, const char *title);
    void Shutdown();

    [[nodiscard]] bool ShouldClose() const;
    void               SetShouldClose(bool close) const;
    static void        PollEvents();
    void               SwapBuffers() const;

    [[nodiscard]] int         GetWidth() const;
    [[nodiscard]] int         GetHeight() const;
    [[nodiscard]] GLFWwindow *GetNativeWindow() const { return window_; }

    void               SetMouseCursorEnabled(bool enable);
    [[nodiscard]] bool GetMouseCursorEnabled() const { return mouse_cursor_enabled_; }

private:
    GLFWwindow *window_;
    bool        mouse_cursor_enabled_;

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
};

#endif // KARTOFFEL_WINDOW_HPP
