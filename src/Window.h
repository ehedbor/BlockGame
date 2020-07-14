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

    [[nodiscard]] bool isKeyPressed(int keyCode) const;

    [[nodiscard]] bool shouldClose() const;
    void setShouldClose(bool value);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

    [[nodiscard]] bool isResized() const;
    void setResized(bool value);

};


