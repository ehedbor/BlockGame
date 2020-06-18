#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
private:
    GLFWwindow *_window;
    int _width;
    int _height;
    bool _resized;

public:
    Window(int glfwWindow, int height, const char *title);
    ~Window();
    Window(const Window &other) = delete;
    Window& operator=(const Window &other) = delete;
    Window(Window&& other) = default;
    Window& operator=(Window&&) = default;

    void update();

    void setClearColor(float r, float g, float b, float a);

    void clear();

    [[nodiscard]] bool shouldClose() const;

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

    [[nodiscard]] bool isResized() const;
    void setResized(bool value);

};


