#include "Window.h"
#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const char *title) :
    _width(width), _height(height), _resized(false), _input(*this) {
    glfwSetErrorCallback([](auto error, auto desc) {
        std::cerr << "GLFW Error #" << error << ": " << desc << std::endl;
    });

    if (!glfwInit()) {
        throw std::runtime_error("Could not initialize GLFW.");
    }

    // Specify the desired version (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the window
    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        throw std::runtime_error("Could not create window.");
    }

    // Allows access of this from a GLFWwindow*
    glfwSetWindowUserPointer(_window, this);

    // Set callbacks
    glfwSetFramebufferSizeCallback(_window, [](auto glfwWindow, int width, int height) {
        auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
        window->setResized(true);
        window->_width = width;
        window->_height = height;
    });

    // hide and capture the mouse
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Setup OpenGL
    glfwMakeContextCurrent(_window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSwapInterval(1); // enable vsync
    glClearColor(0.3, 0.3, 0.3, 1.0);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    //transparency
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enable face culling (only render visible faces)
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    // Enable polygon mode -- wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Window::~Window() {
    glfwDestroyWindow(_window);
}

void Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(_window);
}

Input &Window::input() {
    return _input;
}

const Input &Window::input() const {
    return _input;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(_window);
}

void Window::setShouldClose(bool value) {
    glfwSetWindowShouldClose(_window, value);
}

int Window::width() const {
    return _width;
}

int Window::height() const {
    return _height;
}

bool Window::isResized() const {
    return _resized;
}

void Window::setResized(bool value) {
    _resized = value;
}

GLFWwindow *Window::rawWindow() {
    return _window;
}

const GLFWwindow *Window::rawWindow() const {
    return _window;
}
