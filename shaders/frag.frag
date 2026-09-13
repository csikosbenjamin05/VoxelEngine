#version 430
out vec4 FragColor;

//in vec3 ourColor;
in vec2 TexCoord;

// Binds directly to Texture Unit 0
layout(binding = 0) uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) ; //* vec4(ourColor, 1.0);
}
