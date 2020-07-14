#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class Input;

struct Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float pitch;
    float yaw;

    float movementSpeed;
    float mouseSensitivity;

public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float pitch = 0.0f, float yaw = -90.0f);

    glm::mat4 viewMatrix() const;

    void processInput(const Input &input, float deltaTime);

private:
    void updateCameraVectors();
};


