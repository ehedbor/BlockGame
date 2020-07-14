#include "Game.h"
#include "Window.h"
#include "Input.h"

void Game::processInput(Window &window, float deltaTime) {
    if (window.input().isKeyPressed(GLFW_KEY_ESCAPE)) {
        window.setShouldClose(true);
    }
    _camera.processInput(window, deltaTime);
}

void Game::update(float deltaTime) {

}

void Game::render() {

}