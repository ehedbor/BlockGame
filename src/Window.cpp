#include "Window.h"
#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const char *title) : _width(width), _height(height) {
    glfwSetErrorCallback([](auto error, auto desc) {
        std::cerr << "GLFW Error #" << error << ": " << desc << std::endl;
    });

    if (!glfwInit()) {
        throw std::runtime_error("Could not initialize GLFW.");
    }

    // Specify the desired version (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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

    glfwSetKeyCallback(_window, [](auto window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    });

    // Center the window
    auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!vidmode) {
        throw std::runtime_error("Failed to get video mode.");
    }

    glfwSetWindowPos(_window,
        (vidmode->width - width) / 2,
        (vidmode->height - height) / 2);

    // Setup OpenGL
    glfwMakeContextCurrent(_window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSwapInterval(1); // enable vsyng
    glClearColor(0.3, 0.3, 0.3, 1.0);

    // Enable various features
    //glEnable(GL_DEPTH_TEST);
    // use to enable alpha
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enable face culling (only render visible faces)
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    // Enable polygon mode -- wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
}

Window::~Window() {
    glfwDestroyWindow(_window);
}

void Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(_window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(_window);
}

void Window::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
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



