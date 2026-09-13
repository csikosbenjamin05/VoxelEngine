#version 430
layout (location = 0) in vec3 InPos;

uniform mat4 world;
uniform mat4 viewProj;

void main() {
    gl_Position = viewProj * world * vec4(InPos, 1.0);
}