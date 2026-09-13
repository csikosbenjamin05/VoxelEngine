//
// Created by beni on 2026. 09. 05..
//

#pragma once
#include <array>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "model/math/direction.h"
#include "model/math/BoundingBox.h"

// Represent 16 bit lighting data : 0xSRGB : Sky, Red, Green, Blue
struct LightData {
private:
    uint16_t data{};
public:
    LightData() = default;
    LightData(const int sky, const int red, const int green, const int blue) {
        set(sky, red, green, blue);
    }

    void set(int sky, int red, int green, int blue);
    void setSky(int sky);
    void setRGB(int red, int green, int blue);
    [[nodiscard]] int getSky() const;
    [[nodiscard]] int getRed() const;
    [[nodiscard]] int getGreen() const;
    [[nodiscard]] int getBlue() const;
    [[nodiscard]] uint16_t getSRGB() const;

    bool operator==(const LightData & light_data) const;
};

// Represent 32 bit voxel data : 0b 00 voxelId10 Variant4 LightData16
struct VoxelData {
private:
    // i=id, v=variant :: 0b 00ii iiii iiii vvvv
    uint16_t voxAndVariant{};
    // Represent 16 bit lighting data : 0xSRGB : Sky, Red, Green, Blue
    LightData lightData{};
public:
    VoxelData() = default;
    VoxelData(const int id, const int variant) {
        setVoxel(id, variant);
    }
    VoxelData(const int id, const int variant, const LightData lightData) {
        setVoxel(id, variant);
        setLightData(lightData);
    }

    void setVoxel(int id, int variant);
    void setLightData(LightData light_data);
    void setID(int voxelId);
    void setVariant(int variant);

    [[nodiscard]] int getID() const;
    [[nodiscard]] int getVariant() const;
    [[nodiscard]] uint16_t getIDAndVariant() const;
    [[nodiscard]] LightData getLightData() const;

    bool operator==(const VoxelData & value) const;
};

/*
 Represent where a model is at in a chunk in 63 bits
 9b rotation (3 perc axis, 8 rots)
 10b modelID
 13b variant

 15b positions (5/axis)

 16b lightData
===========
 63 bits
*/
struct ModelData {
private:
    // r=Rotation, i=ID, f=Flag :: 0b rrrr rrrr riii iiii iiif ffff ffff ffff
    uint32_t rotation9ModelId10Variant13{};
    // x,y,z :: 0b 0xxx xxyy yyyz zzzz
    uint16_t position{};
    // Represent 16 bit lighting data : 0xSRGB : Sky, Red, Green, Blue
    LightData lightData{};
public:
    ModelData() = default;
    ModelData(const int model_id, const int variant, const glm::ivec3 position,
        const int rotation, const LightData lightData)
    {
        set(model_id, variant, position, rotation, lightData);
    }

    void set(int model_id, int variant, glm::ivec3 inp_position, int rotation, LightData inp_lightData);

    void setRotation(int rotation);
    void setModelId(int model_id);
    void setVariant(int variant);
    void setPosition(glm::ivec3 inp_position);
    void setLightData(LightData light_data);

    [[nodiscard]] uint32_t getRotation() const;
    [[nodiscard]] uint32_t getModelId() const;
    [[nodiscard]] uint32_t getVariant() const;
    [[nodiscard]] glm::ivec3 getPosition() const;
    [[nodiscard]] LightData getLightData() const;

};

struct FaceData {
private:
    // Pos (15b): X(5b)|Y(5b)|Z(5b), Norm(3b), Var(4b), TexID(10b)
    uint32_t pos15Norm3Var4TexID10{};
    // Light(16b), Flags(16b)
    uint32_t light16Flags16{};
public:
    FaceData() = default;
    void set(glm::ivec3 position, DIRECTION direction, uint8_t variant, uint16_t textureID, LightData lighting, uint16_t flags);

    void setPosition(glm::ivec3 position);
    void setDirection(DIRECTION direction);
    void setVariant(uint8_t variant);
    void setTextureID(uint16_t textureID);
    void setLighting(uint16_t lighting);
    void setFlags(uint16_t flags);

    [[nodiscard]] glm::ivec3 getPosition() const;
    [[nodiscard]] DIRECTION getDirection() const;
    [[nodiscard]] uint8_t getVariant() const;
    [[nodiscard]] uint16_t getTextureID() const;
    [[nodiscard]] uint16_t getLighting() const;
    [[nodiscard]] uint16_t getFlags() const;
};

struct ChunkData {
    static constexpr int SIZE = 32;

    explicit ChunkData(const glm::ivec3& chunk_position);

    [[nodiscard]] VoxelData get(int x, int y, int z) const;
    void set( int x, int y, int z, VoxelData value);
    void set(const glm::ivec3& p, const VoxelData value) {set(p.x, p.y, p.z, value);}


    [[nodiscard]] glm::ivec3 getChunkPosition() const {return chunkPosition;}

    void generateNoise(glm::ivec3 chunk_position);
    void updateEdges();

    [[nodiscard]] const BoundingBox* getBoundingBox() const {
        return &bounding_box;
    }

private:

    // If nullptr -> uniform layer using uniformVoxel[y]
    // If allocated -> points to a contiguous array of 32*32 voxels
    std::unique_ptr<std::array<VoxelData, SIZE * SIZE>> layers[SIZE];
    VoxelData uniformVoxel[SIZE]{}; // Used when layers[y] == nullptr

    glm::ivec3 chunkPosition;
    BoundingBox bounding_box;
    std::unique_ptr<std::array<uint32_t, SIZE>> solidEdges[DIRECTION_COUNT];
};
