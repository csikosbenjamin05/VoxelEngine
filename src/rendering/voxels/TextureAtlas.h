//
// Created by beni on 2026. 09. 05..
//
#pragma once

#include <string>
#include <unordered_map>

#include "model/voxels/ChunkData.h"
#include "glad/glad.h"


#include "../wrapper/sampler2D.h"
#include "../wrapper/texture2D.h"

class VoxelType;

struct TextureCoordinate
{
    float uMin = 0;
    float uMax = 0;
    float vMin = 0;
    float vMax = 0;

    [[nodiscard]] std::string toString() const;
};

class TextureAtlas {
public:
    const float voxelTextureSize;


    explicit TextureAtlas(int voxelTextureSize = 32);

    void Init(const std::filesystem::path& voxelTexturesPath);
    void Clean();

    void UseVoxelAtlas(unsigned int textureUnit) const;

    //TextureCoordinate GetUVRangeForVoxelType(const VoxelType* voxelType) const;
    //TextureCoordinate GetUVRangeForVoxelID(uint16_t id) const;

private:
    int voxelAtlasWidth = -1;
    int voxelAtlasHeight = -1;

    Sampler2D voxelSampler;
    Texture2DArray voxelAtlasTexture;

    //TextureCoordinate CalculateUVRangeForVoxelId(uint16_t id) const;
    //void CalculateUVsForAllVoxels();

    //std::unordered_map<uint16_t, TextureCoordinate> VoxelIdToTextureCoordsMap;
};