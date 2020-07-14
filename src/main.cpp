#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Game.h"

int main() {
    Window window(800, 600, "Block Game");
    Game game;

    float lastFrame = 0.0f;
    float deltaTime;
    while (!window.shouldClose()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (window.isResized()) {
            glViewport(0, 0, window.width(), window.height());
            window.setResized(false);
        }

        game.processInput(window, deltaTime);
        game.update(deltaTime);
        game.render();
        window.update();
    }
}
