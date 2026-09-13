#version 430 core

// Instanced Attributes
layout(std430, binding = 0) readonly buffer FaceBuffer {
    uvec2 faces[];
};

uniform mat4 world;
uniform mat4 viewProj;

out vec3 TexCoords;
out vec3 Normal;
out float Lighting; // Or vec4 for RGBA unpacked lighting

// Lookup tables to generate vertices dynamically based on Direction
const vec3 faceCorners[24] = vec3[](
vec3(1,0,1), vec3(1,0,0), vec3(1,1,0), vec3(1,1,1), // +X
vec3(0,0,0), vec3(0,0,1), vec3(0,1,1), vec3(0,1,0), // -X
vec3(0,1,1), vec3(1,1,1), vec3(1,1,0), vec3(0,1,0), // +Y
vec3(0,0,0), vec3(1,0,0), vec3(1,0,1), vec3(0,0,1), // -Y
vec3(0,0,1), vec3(1,0,1), vec3(1,1,1), vec3(0,1,1), // +Z
vec3(1,0,0), vec3(0,0,0), vec3(0,1,0), vec3(1,1,0)  // -Z
);

const vec3 faceNormals[6] = vec3[](
vec3(1,0,0), vec3(-1,0,0), vec3(0,1,0), vec3(0,-1,0), vec3(0,0,1), vec3(0,0,-1)
);

// Standard CCW Quad UV map: Bottom-Left, Bottom-Right, Top-Right, Top-Left
const vec2 uvCorners[4] = vec2[](
vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(1.0, 1.0), vec2(0.0, 1.0)
);

// Triangles mapping mapping (0,1,2, 0,2,3)
const int indices[6] = int[](0, 1, 2, 0, 2, 3);

// TextureAtlas indexing
const vec2 atlasGrid = vec2(8.0, 1.0);
const vec2 tileSize = 1.0 / atlasGrid;

void main()
{
    uint faceIndex = gl_VertexID / 6;
    int cornerIdx = indices[gl_VertexID % 6];

    // 2. Fetch the 64-bit FaceData struct directly from the SSBO
    uvec2 face = faces[faceIndex]; // ignore the error
    uint pos15Norm3Var4TexID10 = face.x; // pos15Norm3Var4TexID10
    // uint d1 = face.y; // light16Flags16 (Ready when you implement lighting)

    // 3. Unpack bits
    uint x = (pos15Norm3Var4TexID10 >> 27) & 0x1Fu;
    uint y = (pos15Norm3Var4TexID10 >> 22) & 0x1Fu;
    uint z = (pos15Norm3Var4TexID10 >> 17) & 0x1Fu;
    uint dir = (pos15Norm3Var4TexID10 >> 14) & 0x7u;
    uint texID = pos15Norm3Var4TexID10 & 0x3FFu;


    // 4. Local & World Position Calculation
    vec3 localPos = faceCorners[dir * 4u + cornerIdx]; // ignore the error
    vec3 worldPos = vec3(x, y, z) + localPos;


    // 3. Final Position Calculation
    gl_Position = viewProj * world * vec4(worldPos, 1.0);

    Normal = mat3(world) * faceNormals[dir];
/*
    // Map the 10-bit texID dynamically into your Texture Atlas grid
    // 1. Calculate integer tile column and row
    float col = mod(float(texID), atlasGrid.x);
    float row = floor(float(texID) / atlasGrid.x);

    // 2. Base UV (bottom-left corner of the target tile)
    vec2 atlasBaseUV = vec2(col, row) * tileSize;

    // 3. Final vertex UV: base tile origin + offset into the tile
    TexCoords = atlasBaseUV + (uvCorners[cornerIdx] * tileSize);
*/
    TexCoords = vec3(uvCorners[cornerIdx], float(texID));
}