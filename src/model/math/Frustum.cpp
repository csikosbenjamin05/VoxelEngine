//
// Created by beni on 2026. 09. 12..
//

#include "Frustum.h"

#include <algorithm>
#include <array>



void Plane::set(const float dist, const glm::vec3 &norm) {
    distance = dist;
    normal = glm::normalize(norm);
}

void Plane::normalize() {
    const float length = glm::length(normal);
    normal /= length;
    distance /= length;
}

void Frustum::set(const Frustum &other) {
    leftFace.set(other.leftFace);
    rightFace.set(other.rightFace);
    bottomFace.set(other.bottomFace);
    topFace.set(other.topFace);
    nearFace.set(other.nearFace);
    farFace.set(other.farFace);

    planes = {
        &leftFace, &rightFace, &bottomFace, &topFace, &nearFace, &farFace
    };
}

void Frustum::updateFrustumFromCamera(const glm::vec3 Position, const glm::vec3 Front, const glm::vec3 Up,
                                      const glm::vec3 Right, const float aspect, const float fovY, const float zNear, const float zFar)
{
    const float halfVSide = zFar * tanf(fovY * .5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = zFar * Front;

    nearFace = { Position + zNear * Front, Front };

    farFace = { Position + frontMultFar, -Front };

    rightFace = { Position,
                    glm::cross(frontMultFar - Right * halfHSide, Up) };

    leftFace = { Position,
                    glm::cross(Up,frontMultFar + Right * halfHSide) };

    topFace = { Position,
                    glm::cross(Right, frontMultFar - Up * halfVSide) };

    bottomFace = { Position,
                    glm::cross(frontMultFar + Up * halfVSide, Right) };

    planes = {
        &leftFace, &rightFace, &bottomFace, &topFace, &nearFace, &farFace
    };
}


void Frustum::updateBoxVisibility(BoundingBox* bounding_box) const {
    const glm::vec3 min = bounding_box->getMinCorner();
    const glm::vec3 max = bounding_box->getMaxCorner();

    const bool isVisible = std::ranges::all_of(planes, [&](const Plane* plane) {
        // Choose the corner furthest in the direction of the normal (P-vertex)
        const glm::vec3 pVertex(
            plane->normal.x >= 0.0f ? max.x : min.x,
            plane->normal.y >= 0.0f ? max.y : min.y,
            plane->normal.z >= 0.0f ? max.z : min.z
        );
        // If the most positive point is behind the plane, the whole AABB is culled
        return plane->getSignedDistanceToPlane(pVertex) >= 0.0f;
    });

    bounding_box->isVisible = isVisible;
}
