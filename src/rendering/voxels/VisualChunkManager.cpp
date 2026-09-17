//
// Created by beni on 2026. 09. 14..
//

#include <model/voxels/ChunkData.h>
#include <model/voxels/ChunkManager.h>

#include "VisualChunkManager.h"

#include <ranges>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "TextureAtlas.h"

VisualChunkManager::VisualChunkManager(ChunkManager *chunkManager, TextureAtlas *textureAtlas)
    : _chunkManager(chunkManager), _textureAtlas(textureAtlas)
{
}

void VisualChunkManager::Init() {
    voxelShader.initShader("shaders/voxel.vert", "shaders/voxel.frag");


    activeVisualChunkMap.clear();
    activeVisualChunkMap.reserve(_chunkManager->GetTotalChunkCount());
    visualChunkPool.reserve(_chunkManager->GetTotalChunkCount());

    _chunkManager->boundingBox.IterateOverAllPositions([this](const glm::vec3 position) {
        auto vChunk = std::make_unique<VisualChunk>(_chunkManager->GetChunkAt(position));
        vChunk->Init();
        activeVisualChunkMap[position] = std::move(vChunk);
    });

}

void VisualChunkManager::Render(glm::mat4 viewProj) const {

    voxelShader.use();
    glUniformMatrix4fv(
        voxelShader.getUniformLocation("viewProj"),
        1,
        GL_FALSE,
        glm::value_ptr(viewProj)
    );

    _textureAtlas->UseVoxelAtlas(0);

    for (const auto &val: activeVisualChunkMap | std::views::values) {
        if (val->GetBoundingBoxReference().isVisible)
            val->DrawObject();
    }

}

void VisualChunkManager::UpdateFrustum(const Frustum &camera_frustum) {
    frustum.set(camera_frustum);

    for (const auto &val: activeVisualChunkMap | std::views::values) {
        frustum.updateBoxVisibility(&val->GetBoundingBoxReference());
    }

}

void VisualChunkManager::UpdateCenterChunk(const glm::ivec3 centerChunk) {
    auto [unloadedPositions, loadedPositions] = _chunkManager->SetCenterPosition(centerChunk);

    // 1. Recycle unloaded visual chunks back into the pool
    for (const auto& pos : unloadedPositions) {
        if (auto it = activeVisualChunkMap.find(pos); it != activeVisualChunkMap.end()) {
            visualChunkPool.push_back(std::move(it->second));
            activeVisualChunkMap.erase(it);
        }
    }

    // 2. Assign chunks from the pool to new positions
    for (const auto& pos : loadedPositions) {
        std::unique_ptr<VisualChunk> vChunk;

        if (!visualChunkPool.empty()) {
            vChunk = std::move(visualChunkPool.back());
            visualChunkPool.pop_back();
            vChunk->SetChunkReference(_chunkManager->GetChunkAt(pos));
        } else {
            vChunk = std::make_unique<VisualChunk>(_chunkManager->GetChunkAt(pos));
            vChunk->Init();
        }

        vChunk->Bake(); // TODO : MESHING THREAD
        activeVisualChunkMap.emplace(pos, std::move(vChunk));
    }
}

void VisualChunkManager::ChangeRenderDistance(const int verticalRenderDistance, const int horizontalRenderDistance) {

    // 1. Move all visual chunks back to the pool
    for (auto &val: activeVisualChunkMap | std::views::values) {
        val->Clean(); // might not get used again for a while, so clean it
        visualChunkPool.push_back(std::move(val));
    }
    activeVisualChunkMap.clear(); // remove all the leftover positions and null pointers


    _chunkManager->SetRenderDistance(verticalRenderDistance, horizontalRenderDistance);

    _chunkManager->boundingBox.IterateOverAllPositions([this](const glm::ivec3 position) {

        std::unique_ptr<VisualChunk> vChunk;

        if (!visualChunkPool.empty()) {
            vChunk = std::move(visualChunkPool.back());
            visualChunkPool.pop_back();
            vChunk->SetChunkReference(_chunkManager->GetChunkAt(position));
            vChunk->Bake(); // TODO : MESHING THREAD
        } else {
            vChunk = std::make_unique<VisualChunk>(_chunkManager->GetChunkAt(position));
            vChunk->Init(); // TODO : MESHING THREAD
        }
        activeVisualChunkMap.emplace(position, std::move(vChunk));
    });

}



void VisualChunkManager::Clean() {

    for (const auto &val: activeVisualChunkMap | std::views::values)
        val->Clean();

    for (const auto &val : visualChunkPool)
        val->Clean();

    activeVisualChunkMap.clear();
    visualChunkPool.clear();

}