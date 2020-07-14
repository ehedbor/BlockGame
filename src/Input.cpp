#include "Input.h"
#include <algorithm>
#include "Window.h"


Input::Input(Window &window) {
    for (std::size_t i = 0; i < _activeKeys.size(); i++) {
        _activeKeys[i] = false;
    }

    for (std::size_t i = 0; i < _activeMouseButtons.size(); i++) {
        _activeMouseButtons[i] = false;
    }

    _lastMousePos = glm::vec2(0.0f, 0.0f);

    setupCallbacks(window);
}

bool Input::isKeyPressed(int key) const {
    auto iterator = _activeKeys.find(key);
    if (iterator != _activeKeys.end()) {
        return iterator->second;
    }
    return false;
}


void Input::setKeyPressed(int key, bool pressed) {
    _activeKeys[key] = pressed;
}

bool Input::isMouseButtonPressed(int button) const {
    return _activeMouseButtons[button];
}

void Input::setMouseButtonPressed(int button, bool pressed) {
    _activeMouseButtons[button] = pressed;
}

glm::vec2 Input::mousePos() const {
    return _lastMousePos;
}

void Input::setMousePos(const glm::vec2 &pos) {
    if (_firstMouse) {
        _lastMousePos = pos;
        _firstMouse = false;
    }

    _mouseOffset.x = pos.x - _lastMousePos.x;
    _mouseOffset.y = _lastMousePos.y - pos.y; // reversed because y-coords go from bottom to top
    _lastMousePos = pos;
}

glm::vec2 Input::mouseOffset() const {
    return _mouseOffset;
}

void keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods) {
    auto window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
    auto input = window->input();
    input.setKeyPressed(key, action != GLFW_RELEASE);
}

void mouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods) {
    auto window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
    auto input = window->input();
    input.setMouseButtonPressed(button, action != GLFW_RELEASE);
}

void cursorPosCallback(GLFWwindow *glfwWindow, double xpos, double ypos) {
    auto window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
    auto input = window->input();
    input.setMousePos({xpos, ypos});
}

void Input::setupCallbacks(Window &window) {
    auto glfwWindow = window.rawWindow();
    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
    glfwSetCursorPosCallback(glfwWindow, cursorPosCallback);
}



