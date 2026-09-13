#version 430 core
layout(early_fragment_tests) in;

out vec4 FragColor;

in vec3 TexCoords;
in vec3 Normal;
// in float Lighting;

layout(binding = 0) uniform sampler2DArray texture_atlas;

void main()
{
    FragColor = texture(texture_atlas, TexCoords);
}