//
// Created by beni on 2026. 09. 11..
//

#include "BoundingBox.h"
#include <cassert>

BoundingBox::BoundingBox(const glm::ivec3 &min, const glm::ivec3 &max)
    : minCorner(min), maxCorner(max){
    assertValidParameters();
}

bool BoundingBox::isInside(glm::ivec3 p) const {
    return p.x >= minCorner.x && p.y >= minCorner.y && p.z >= minCorner.z
    && p.x <= maxCorner.x && p.y <= maxCorner.y && p.z <= maxCorner.z;
}

bool BoundingBox::isIntersecting(const BoundingBox &bounding_box) const {
    return minCorner.x <= bounding_box.maxCorner.x
        && minCorner.y <= bounding_box.maxCorner.y
        && minCorner.z <= bounding_box.maxCorner.z
        && maxCorner.x >= bounding_box.minCorner.x
        && maxCorner.y >= bounding_box.minCorner.y
        && maxCorner.z >= bounding_box.minCorner.z;
}

void BoundingBox::set(const glm::ivec3 &min, const glm::ivec3 &max) {
    minCorner = min;
    maxCorner = max;
    assertValidParameters();
}

void BoundingBox::setMinCorner(const glm::ivec3 &min) {
    minCorner = min;
    assertValidParameters();
}

void BoundingBox::setMaxCorner(const glm::ivec3 &max) {
    maxCorner = max;
    assertValidParameters();
}

void BoundingBox::assertValidParameters() const {
    assert(
        (minCorner.x < maxCorner.x && minCorner.y < maxCorner.y && minCorner.z < maxCorner.z)
        && "Invalid bounds: minCorner must be <= maxCorner on all axes"
    );
}
