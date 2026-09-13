//
// Created by beni on 2026. 09. 04..
//

#include "intersections.h"

#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/quaternion_geometric.hpp>

// sík metszése sugárral
bool HitPlane(const Ray& ray, const glm::vec3& planeQ, const glm::vec3& planeI, const glm::vec3& planeJ, Intersection& result)
{
    // sík parametrikus egyenlete: planeQ + u * planeI + v * planeJ
    const glm::mat3 A(-ray.direction, planeI, planeJ);
    const glm::vec3 B = ray.origin - planeQ;

    if (fabsf(glm::determinant(A)) < 1e-6) return false;
    const glm::vec3 X = glm::inverse(A) * B;

    if (X.x < 0.0) {
        return false;
    }
    result.t = X.x;
    result.uv.x = X.y;
    result.uv.y = X.z;

    result.position = ray.origin + result.t * ray.direction;

    return true;
}

// gömb metszése sugárral
bool HitSphere(const Ray& ray, const glm::vec3& sphereCenter, float sphereRadius, Intersection& result)
{
    const glm::vec3 p_m_c = ray.origin - sphereCenter;
    const float a = glm::dot(ray.direction, ray.direction);
    const float b = 2.0f * glm::dot(ray.direction, p_m_c);
    const float c = glm::dot(p_m_c, p_m_c) - sphereRadius * sphereRadius;

    const float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f)
        return false;

    const float sqrtDiscriminant = sqrtf(discriminant);

    // Mivel 2*a és sqrt(D) mindig pozitívak, ezért tudjuk, hogy t0 < t1
    const float t0 = (-b - sqrtDiscriminant) / (2.0f * a);
    const float t1 = (-b + sqrtDiscriminant) / (2.0f * a);

    if (t1 < 0.0f) // mivel t0 < t1, ha t1 negatív, akkor t0 is az
        return false;

    if (t0 < 0.0f)
        result.t = t1;
    else
        result.t = t0;

    result.position = ray.origin + result.t * ray.direction;

    return true;
}
