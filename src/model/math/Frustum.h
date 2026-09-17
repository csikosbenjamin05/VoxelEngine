//
// Created by beni on 2026. 09. 12..
//

#pragma once
#include <array>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "BoundingBox.h"

// https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling

struct Plane {
    glm::vec3 normal{0.0f};
    float distance = 0.0f;

    Plane() = default;

    Plane(const glm::vec3& p1, const glm::vec3& norm)
    : normal(glm::normalize(norm)), distance(-glm::dot(normal, p1))
    {}

    void set(float dist, const glm::vec3& norm);
    void set(const Plane& other) {set(other.distance, other.normal);}


    // Normalize plane coefficients so distance tests produce exact metric offsets
    void normalize();

    [[nodiscard]] float getSignedDistanceToPlane(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }
};


struct Frustum {
    std::array<const Plane*, 6> planes{};

    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;

    void set(const Frustum& other);

    void updateFrustumFromCamera(
        glm::vec3 Position, glm::vec3 Front, glm::vec3 Up, glm::vec3 Right,
        float aspect, float fovY, float zNear, float zFar
    );

    void updateBoxVisibility(BoundingBox* bounding_box) const;
};