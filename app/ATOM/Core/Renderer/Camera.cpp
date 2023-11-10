//
// Created by toor on 11/10/23.
//

#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"

namespace Atom {


    Camera::Camera(glm::vec3 position) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
                                         MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = position;

        updateCameraVectors();
    }


    glm::mat4 Camera::GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::vec3 Camera::GetCameraPos() {
        return Position;
    }

    glm::vec3 Camera::GetCameraFront() {
        return Front;
    }


    void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == Camera_Movement::FORWARD)
            Position += Front * velocity;
        if (direction == Camera_Movement::BACKWARD)
            Position -= Front * velocity;
        if (direction == Camera_Movement::LEFT)
            Position -= Right * velocity;
        if (direction == Camera_Movement::RIGHT)
            Position += Right * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset) {
        Zoom -= (float) yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void Camera::Update() {

        //set camera movment
        float deltaTime = 0.0f;    // Time between current frame and last frame
        float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        if (Keyboard::key(GLFW_KEY_S)) {
            ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        }
        if (Keyboard::key(GLFW_KEY_W)) {
            ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        }
        if (Keyboard::key(GLFW_KEY_A)) {
            ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        }
        if (Keyboard::key(GLFW_KEY_D)) {
            ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
        }
        double dx = Mouse::getDX();
        double dy = Mouse::getDY();

        ProcessMouseMovement(dx, -dy, true);

    }

    void Camera::updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

}