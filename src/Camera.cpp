#include "Camera.h"

#include <GLFW/glfw3.h>
#include "Input.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up, float pitch, float yaw) :
    position(std::move(_position)),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(std::move(_up)),
    worldUp(up),
    pitch(pitch),
    yaw(yaw),
    movementSpeed(2.5f),
    mouseSensitivity(0.1f) {
    updateCameraVectors();
}

glm::mat4 Camera::viewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processInput(const Input &input, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (input.isKeyPressed(GLFW_KEY_W)) {
        position += front * velocity;
    }
    if (input.isKeyPressed(GLFW_KEY_S)) {
        position -= front * velocity;
    }
    if (input.isKeyPressed(GLFW_KEY_A)) {
        position -= right * velocity;
    }
    if (input.isKeyPressed(GLFW_KEY_S)) {
        position += right * velocity;
    }

    glm::vec2 mouseOffset = input.mouseOffset();
    mouseOffset *= mouseSensitivity;
    pitch += mouseOffset.y;
    yaw += mouseOffset.x;

    // constrain the pitch so the screen doesnt get flipped
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateCameraVectors();
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
    // calculate the new front vector
    front.x = cos(glm::radians(pitch) * cos(glm::radians(yaw)));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(front);

    // also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}
