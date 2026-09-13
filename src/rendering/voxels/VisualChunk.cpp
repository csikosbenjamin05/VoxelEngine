//
// Created by beni on 2026. 09. 05..
//

#include "VisualChunk.h"

#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model/voxels/VoxelType.h"
#include "rendering/wrapper/shader.h"

VisualChunk::VisualChunk(TextureAtlas *textureAtlas, ChunkData *newChunk)
: textureAtlas(textureAtlas), chunk(newChunk)
{
    SetChunkReference(newChunk);
}

VisualChunk::~VisualChunk() = default;




void VisualChunk::Init() {
    Bake();
}

void VisualChunk::SetChunkReference(ChunkData *newChunk) {

    chunk = newChunk;

    const glm::vec3 chunkVisualPosition = glm::vec3(chunk->getChunkPosition()) * static_cast<float>(ChunkData::SIZE);

    SetWorldTransform(
        glm::translate(glm::mat4(1.0), chunkVisualPosition)
    );
}

void VisualChunk::GenerateInstanced(const std::vector<FaceData> &faces) {

    if (isGenerated) CleanOGLObject(gpuObject);

    // 1. Create and populate the buffer (used as an SSBO)
    glCreateBuffers(1, &gpuObject.vboID);
    glNamedBufferData(gpuObject.vboID, faces.size() * sizeof(FaceData), faces.data(), GL_STATIC_DRAW);

    // 2. Create an empty VAO strictly to satisfy OpenGL Core profile requirements
    glCreateVertexArrays(1, &gpuObject.vaoID);

    gpuObject.count = static_cast<GLsizei>(faces.size());
    isGenerated = true;
}

void VisualChunk::DrawObject() const {
    glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(world));
    glUniformMatrix4fv(ul("worldInvTranspose"), 1, GL_FALSE, glm::value_ptr(invWorld));

    // 1. Bind our buffer to SSBO Binding Point 0
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gpuObject.vboID);

    // 2. Bind the empty VAO
    glBindVertexArray(gpuObject.vaoID);

    // 3. Issue a single bulk draw call (6 vertices per face)
    glDrawArrays(GL_TRIANGLES, 0, gpuObject.count * 6);

    glBindVertexArray(0);
}


void VisualChunk::Bake() {

    std::vector<FaceData> faces;
    // Reserve an estimated capacity to prevent reallocation overhead
    faces.reserve(4096);

    for (int y = 0; y < ChunkData::SIZE; y++) {
        for (int x = 0; x < ChunkData::SIZE; x++) {
            for (int z = 0; z < ChunkData::SIZE; z++) {

                const VoxelData voxel = chunk->get(x, y, z);
                if (voxel.getID() == VoxelType::Air.id) continue;

                const auto position = glm::ivec3(x, y, z);
                const uint16_t texID = voxel.getID();
                const LightData lighting = voxel.getLightData();
                const uint8_t variant = voxel.getVariant();

                auto isTransparent = [&](const int nx, const int ny, const int nz) {
                    if (nx < 0 || ny < 0 || nz < 0 || nx >= ChunkData::SIZE || ny >= ChunkData::SIZE || nz >= ChunkData::SIZE)
                        return true;
                    return !VoxelType::FromID(chunk->get(nx, ny, nz).getID())->isSolid;
                };

                FaceData face;
                if (isTransparent(x, y + 1, z)) { face.set(position, posY, variant, texID, lighting, 0); faces.push_back(face); }
                if (isTransparent(x, y - 1, z)) { face.set(position, negY, variant, texID, lighting, 0); faces.push_back(face); }
                if (isTransparent(x + 1, y, z)) { face.set(position, posX, variant, texID, lighting, 0); faces.push_back(face); }
                if (isTransparent(x - 1, y, z)) { face.set(position, negX, variant, texID, lighting, 0); faces.push_back(face); }
                if (isTransparent(x, y, z + 1)) { face.set(position, posZ, variant, texID, lighting, 0); faces.push_back(face); }
                if (isTransparent(x, y, z - 1)) { face.set(position, negZ, variant, texID, lighting, 0); faces.push_back(face); }
            }
        }
    }

    std::cout << "VisualChunk baked with " << faces.size() << " faces" << std::endl;
    GenerateInstanced(faces);
}

/*
void VisualChunk::CreateFace(MeshObject<VertexPos3Tex2> &meshCPU, const glm::ivec3 position,
    const DIRECTION direction, const TextureCoordinate tc) {

    const auto x = static_cast<float>(position.x);
    const auto y = static_cast<float>(position.y);
    const auto z = static_cast<float>(position.z);

    const float x2 = x + 1.0f;
    const float y2 = y + 1.0f;
    const float z2 = z + 1.0f;

    const float uM = tc.uMin, uX = tc.uMax;
    const float vM = tc.vMin, vX = tc.vMax;

    const int startIndex = static_cast<int>(meshCPU.vertexArray.size());

    // Vertices order: Bottom-Left, Bottom-Right, Top-Right, Top-Left
    switch (direction) {
        case DIRECTION::posY: // Top Face (+Y)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z2), glm::vec2(uM, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z2), glm::vec2(uX, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z ), glm::vec2(uX, vX)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z ), glm::vec2(uM, vX)});
            break;
        case DIRECTION::negY: // Bottom Face (-Y)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z ), glm::vec2(uM, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z ), glm::vec2(uX, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z2), glm::vec2(uX, vX)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z2), glm::vec2(uM, vX)});
            break;
        case DIRECTION::posZ: // Front Face (+Z)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z2), glm::vec2(uM, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z2), glm::vec2(uX, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z2), glm::vec2(uX, vX)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z2), glm::vec2(uM, vX)});
            break;
        case DIRECTION::negZ: // Back Face (-Z)
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z ), glm::vec2(uM, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z ), glm::vec2(uX, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z ), glm::vec2(uX, vX)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z ), glm::vec2(uM, vX)});
            break;
        case DIRECTION::posX: // Right Face (+X)
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z2), glm::vec2(uM, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z ), glm::vec2(uX, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z ), glm::vec2(uX, vX)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z2), glm::vec2(uM, vX)});
            break;
        case DIRECTION::negX: // Left Face (-X)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z ), glm::vec2(uM, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z2), glm::vec2(uX, vM)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z2), glm::vec2(uX, vX)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z ), glm::vec2(uM, vX)});
            break;
    }

    // CCW Triangle 1
    meshCPU.indexArray.push_back(startIndex + 0);
    meshCPU.indexArray.push_back(startIndex + 1);
    meshCPU.indexArray.push_back(startIndex + 2);

    // CCW Triangle 2
    meshCPU.indexArray.push_back(startIndex + 0);
    meshCPU.indexArray.push_back(startIndex + 2);
    meshCPU.indexArray.push_back(startIndex + 3);
} */