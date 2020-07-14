#pragma once

#include "Camera.h"

class Window;

class Game {
private:
    Camera _camera;

public:
    void processInput(Window &input, float deltaTime);
    void update(float deltaTime);
    void render();
};


