#pragma once

#include <array>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Window;

class Input {
private:
    std::unordered_map<int, bool> _activeKeys{};
    // considering how there are only 8 mouse buttons, an unordered_map is unnecessary
    std::array<bool, GLFW_MOUSE_BUTTON_LAST> _activeMouseButtons{};
    glm::vec2 _lastMousePos{0.0f, 0.0f};
    glm::vec2 _mouseOffset{0.0f, 0.0f};
    bool _firstMouse{true};

public:
    Input(Window &window);

    bool isKeyPressed(int key) const;
    void setKeyPressed(int key, bool pressed);

    bool isMouseButtonPressed(int button) const;
    void setMouseButtonPressed(int button, bool pressed);

    glm::vec2 mousePos() const;
    void setMousePos(const glm::vec2 &pos);
    glm::vec2 mouseOffset() const;

private:
    void setupCallbacks(Window &window);

};


