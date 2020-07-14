#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"

class Window {
private:
    GLFWwindow *_window;
    int _width;
    int _height;
    bool _resized;
    Input _input;

public:
    Window(int glfwWindow, int height, const char *title);
    ~Window();
    Window(const Window &other) = delete;
    Window& operator=(const Window &other) = delete;
    Window(Window&& other) = default;
    Window& operator=(Window&&) = default;

    void update();

    Input &input();
    const Input &input() const;

    bool shouldClose() const;
    void setShouldClose(bool value);

    int width() const;
    int height() const;

    bool isResized() const;
    void setResized(bool value);

    GLFWwindow *rawWindow();
    const GLFWwindow *rawWindow() const;
};


