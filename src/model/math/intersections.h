//
// Created by beni on 2026. 09. 04..
//
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// sugár
struct Ray
{
    glm::vec3 origin; // kiinduló pont
    glm::vec3 direction; // irányvektor (nem feltétlen egységhosszú)
};

// metszéspont
struct Intersection
{
    glm::vec2 uv; // sík paraméterei (u,v)
    glm::vec3 position; // metszéspont pozíciója
    float t; // "távolság" a sugár kiinduló pontjától
};

// sík metszése sugárral
bool HitPlane( const Ray& ray, const glm::vec3& planeQ, const glm::vec3& planeI, const glm::vec3& planeJ, Intersection& result );

// gömb metszése sugárral
bool HitSphere( const Ray& ray, const glm::vec3& sphereCenter, float sphereRadius, Intersection& result );

