//
// Created by beni on 2026. 09. 12..
//
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "ChunkData.h"

class IWorldGenerator;

class ChunkManager {
public:
    ~ChunkManager();

    explicit ChunkManager(IWorldGenerator* worldGenerator, glm::ivec3 centerIndex, int renderDistanceHorizontal, int renderDistanceVertical);

    int GetRenderDistanceHorizontal() const {return renderDistanceHorizontal;}
    int GetRenderDistanceVertical() const {return renderDistanceVertical;}

    BoundingBox boundingBox;

    ChunkData* GetChunkAt(const glm::ivec3& position) const;
    bool IsChunkAt(const glm::ivec3& position) const;

    void SetCenterPosition(const glm::ivec3& newPosition);

    std::unordered_set<glm::ivec3> dirtyChunkList;

private:
    // chunks are loaded around the centerIndex
    glm::ivec3 centerChunkCoordinate;

    // how many chunks get loaded in each direction
    int renderDistanceHorizontal;
    int renderDistanceVertical;
    int loadedChunkCount;

    std::unordered_map<glm::ivec3, std::unique_ptr<ChunkData>> chunkMap;

    IWorldGenerator* worldGenerator;

    void CalculateTotalAndMinAndMaxCorner();
    void LoadOrGenerateChunkAt(glm::ivec3 position);
    void UnloadChunkAt(const glm::ivec3&  position);
};


