//
// Created by beni on 2026. 09. 06..
//

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "model/math/Frustum.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
constexpr float YAW         = -90.0f;
constexpr float PITCH       =  0.0f;
constexpr float SPEED       =  15.0f;
constexpr float SENSITIVITY =  0.2f;
constexpr float ZOOM        =  45.0f;

constexpr float NEAR        =  0.01f;
constexpr float FAR        =  100.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up{};
    glm::vec3 Right{};
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float zNear;
    float zFar;

    Frustum frustum;

    // constructor with vectors
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float near = NEAR, float far=FAR);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float near = NEAR, float far=FAR);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    [[nodiscard]] glm::mat4 GetViewProjMatrix(const float SCR_WIDTH, const float SCR_HEIGHT) const {
        return
            glm::perspective(glm::radians(Zoom), SCR_WIDTH / SCR_HEIGHT, zNear, zFar) *
            glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset);


    void UpdateFrustum(float SCR_WIDTH, float SCR_HEIGHT);
    void UpdateBoundingBoxVisibility(BoundingBox& bounding_box) const;

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};