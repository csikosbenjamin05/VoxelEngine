//
// Created by beni on 2026. 09. 11..
//
#pragma once
#include <glm/vec3.hpp>
#include <cassert>


struct BoundingBox {
    BoundingBox() = default;
    BoundingBox(const glm::ivec3 &min, const glm::ivec3 &max);

    bool isVisible;

    [[nodiscard]] bool isInside(glm::ivec3 position) const;
    [[nodiscard]] bool isIntersecting(const BoundingBox &bounding_box) const;

    [[nodiscard]] const glm::ivec3& getMinCorner() const { return minCorner; }
    [[nodiscard]] const glm::ivec3& getMaxCorner() const { return maxCorner; }
    [[nodiscard]] glm::ivec3 getSize() const { return maxCorner - minCorner; }

    void set(const glm::ivec3 &min, const glm::ivec3 &max);
    void setMinCorner(const glm::ivec3 &min);
    void setMaxCorner(const glm::ivec3 &max);

private:
    glm::ivec3 minCorner;
    glm::ivec3 maxCorner;



    void assertValidParameters() const;
};

