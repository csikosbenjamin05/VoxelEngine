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

    visualChunkMap.clear();
    visualChunkMap.reserve(_chunkManager->GetTotalChunkCount());

    _chunkManager->boundingBox.IterateOverAllPositions([this](const glm::vec3 position) {
        auto vChunk = std::make_unique<VisualChunk>(_chunkManager->GetChunkAt(position));
        vChunk->Init();
        visualChunkMap[position] = std::move(vChunk);
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

    for (const auto &val: visualChunkMap | std::views::values)
        val->DrawObject();

}

void VisualChunkManager::UpdateCenterChunk(const glm::ivec3 centerChunk) const {
    auto [unloadedPositions, loadedPositions] = _chunkManager->SetCenterPosition(centerChunk);

    assert(loadedPositions.size() == unloadedPositions.size() && "FATAL :: Mismatch between loaded positions and unloaded positions.");

    auto loadedIterator = loadedPositions.begin();
    auto unloadedIterator = loadedPositions.begin();

    while (unloadedIterator != unloadedPositions.end()) {
        visualChunkMap.at(*unloadedIterator)->SetChunkReference(_chunkManager->GetChunkAt(*loadedIterator));
        visualChunkMap.at(*unloadedIterator)->Bake();
        ++unloadedIterator;
        ++loadedIterator;
    }
}

void VisualChunkManager::ChangeRenderDistance(int verticalRenderDistance, int horizontalRenderDistance) {


}



void VisualChunkManager::Clean() {

    for (const auto &val: visualChunkMap | std::views::values)
        val->Clean();

    visualChunkMap.clear();

}