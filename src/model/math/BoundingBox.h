//
// Created by beni on 2026. 09. 11..
//
#pragma once
#include <glm/vec3.hpp>
#include <cassert>


struct BoundingBox {
    BoundingBox() = default;
    BoundingBox(const glm::ivec3 &min, const glm::ivec3 &max);

    bool isVisible = false;

    [[nodiscard]] bool isInside(glm::ivec3 position) const;
    [[nodiscard]] bool isIntersecting(const BoundingBox &bounding_box) const;

    [[nodiscard]] const glm::ivec3& getMinCorner() const { return minCorner; }
    [[nodiscard]] const glm::ivec3& getMaxCorner() const { return maxCorner; }
    [[nodiscard]] glm::ivec3 getSize() const { return maxCorner - minCorner; }

    void set(const glm::ivec3 &min, const glm::ivec3 &max);
    void setMinCorner(const glm::ivec3 &min);
    void setMaxCorner(const glm::ivec3 &max);

    template<typename Func>
    void IterateOverAllPositions(Func action) const
    {
        for (int x = minCorner.x; x <= maxCorner.x; ++x) {
            for (int y = minCorner.y; y <= maxCorner.y; ++y) {
                for (int z = minCorner.z; z <= maxCorner.z; ++z) {
                    action(glm::ivec3(x, y, z));
                }
            }
        }
    }



private:
    glm::ivec3 minCorner{};
    glm::ivec3 maxCorner{};

    void assertValidParameters() const;
};

