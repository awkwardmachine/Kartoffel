//
// Created by nil on 02/03/2026.
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
    static void        PollEvents();
    void               SwapBuffers() const;

    [[nodiscard]] int         GetWidth() const { return width_; }
    [[nodiscard]] int         GetHeight() const { return height_; }
    [[nodiscard]] GLFWwindow *GetNativeWindow() const { return window_; }

private:
    GLFWwindow *window_;
    int         width_;
    int         height_;

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
};

#endif // KARTOFFEL_WINDOW_HPP
