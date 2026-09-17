//
// Created by beni on 2026. 09. 14..
//
#pragma once

#include <rendering/voxels/VisualChunk.h>

#include "model/math/Frustum.h"
#include "rendering/wrapper/shader.h"

class TextureAtlas;
class ChunkManager;

class VisualChunkManager {
public:
    explicit VisualChunkManager(ChunkManager* chunkManager, TextureAtlas* textureAtlas);

    void Init();
    void Render(glm::mat4 viewProj) const;
    void Clean();

    void UpdateCenterChunk(glm::ivec3 centerChunk);
    void ChangeRenderDistance(int verticalRenderDistance, int horizontalRenderDistance);

    void UpdateFrustum(const Frustum &frustum);

private:
    ChunkManager* _chunkManager;
    TextureAtlas* _textureAtlas;

    Shader voxelShader;

    std::unordered_map<glm::ivec3, std::unique_ptr<VisualChunk>> activeVisualChunkMap;
    std::vector<std::unique_ptr<VisualChunk>> visualChunkPool;

    Frustum frustum;
};

