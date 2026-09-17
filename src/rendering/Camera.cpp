//
// Created by beni on 2026. 09. 06..
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float near, float far)
: Position(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(up), Yaw(yaw), Pitch(pitch),
MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), zNear(near), zFar(far) {
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float near, float far)
: Position(glm::vec3(posX, posY, posZ)), Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(glm::vec3(upX, upY, upZ)),
Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED),  MouseSensitivity(SENSITIVITY), Zoom(ZOOM), zNear(near), zFar(far) {
    updateCameraVectors();
}

void Camera::ProcessKeyboard(const Camera_Movement direction, const float deltaTime) {
    const float velocity = MovementSpeed * deltaTime;

    switch (direction) {
        case FORWARD:
            Position += Front * velocity;
            break;

        case BACKWARD:
            Position -= Front * velocity;
            break;

        case LEFT:
            Position -= Right * velocity;
            break;

        case RIGHT:
            Position += Right * velocity;
            break;

        case UP:
            Position += Up * velocity;
            break;

        case DOWN:
            Position -= Up * velocity;
            break;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, const GLboolean constrainPitch) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw   += xOffset;
    Pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(const float yOffset) {
    Zoom -= yOffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::UpdateFrustum(const float SCR_WIDTH, const float SCR_HEIGHT) {
    frustum.updateFrustumFromCamera(
              Position, Front, Up, Right,
        SCR_WIDTH / SCR_HEIGHT, glm::radians(Zoom),
              zNear, zFar
    );
}

void inline Camera::UpdateBoundingBoxVisibility(BoundingBox *bounding_box) const {
    frustum.updateBoxVisibility(bounding_box);
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    front.y = glm::sin(glm::radians(Pitch));
    front.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

