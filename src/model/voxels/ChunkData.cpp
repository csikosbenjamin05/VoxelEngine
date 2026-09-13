//
// Created by beni on 2026. 09. 05..
//

#include "ChunkData.h"
#include "model/voxels/VoxelType.h"
#include <random>

//--------------------------------------------------------
//                  LightData
//--------------------------------------------------------

void LightData::set(const int sky, const int red, const int green, const int blue) {
    data = static_cast<uint16_t>(
        (sky & 0x0F) << 12 |
        (red & 0x0F) << 8 |
        (green & 0x0F) << 4 |
        (blue & 0x0F)
    );
}

void LightData::setSky(const int sky) {
    data = static_cast<uint16_t>((data & 0x0FFF) | ((sky & 0x0F) << 12));
}

void LightData::setRGB(const int red, const int green, const int blue) {
    data = static_cast<uint16_t>(
            (data & 0xF000) |
            (red & 0x0F) << 8 |
            (green & 0x0F) << 4 |
            (blue & 0x0F)
        );
}


int LightData::getSky() const {
    return (data & 0xF000) >> 12;
}

int LightData::getRed() const {
    return (data & 0x0F00) >> 8;
}

int LightData::getGreen() const {
    return (data & 0x00F0) >> 4;
}

int LightData::getBlue() const {
    return (data & 0x000F);
}

uint16_t LightData::getSRGB() const {
    return data;
}

bool LightData::operator==(const LightData &light_data) const {
    return data == light_data.data;
}

//--------------------------------------------------------
//                  VoxelData
//--------------------------------------------------------

void VoxelData::setVoxel(const int id, const int variant) {
    voxAndVariant = static_cast<uint16_t>( // 0b 0000 0011 1111 1111 = 0x03FF
        (id & 0x03FF) << 4 |
        (variant & 0x0F)
    );
}

void VoxelData::setLightData(const LightData light_data) {
    lightData = light_data;
}

void VoxelData::setID(const int voxelId) {
    voxAndVariant = static_cast<uint16_t>(
        (voxelId & 0x03FF) << 4 |
        (getVariant() & 0x0F)
    );
}

void VoxelData::setVariant(const int variant) {
    voxAndVariant = static_cast<uint16_t>(
        (getID() & 0x03FF) << 4 |
        (variant & 0x0F)
    );
}

int VoxelData::getID() const {
    return (voxAndVariant & 0x3FF0) >> 4;
}

int VoxelData::getVariant() const {
    return (voxAndVariant & 0x000F);
}

uint16_t VoxelData::getIDAndVariant() const {
    return voxAndVariant;
}

LightData VoxelData::getLightData() const {
    return lightData;
}

bool VoxelData::operator==(const VoxelData &value) const {
    return voxAndVariant == value.getIDAndVariant() && lightData == value.getLightData();
}


//--------------------------------------------------------
//                  ModelData
//--------------------------------------------------------

void ModelData::set(const int model_id, const int variant, const glm::ivec3 inp_position, const int rotation, const LightData inp_lightData) {

    rotation9ModelId10Variant13 = static_cast<uint32_t>(
        ((rotation & 0x1FF) << 23) |
        ((model_id & 0x3FF) << 13) |
        (variant & 0x1FFF)
    );

    this->position = static_cast<uint16_t>(
        ((inp_position.x & 0x1F) << 10) |
        ((inp_position.y & 0x1F) << 5) |
        (inp_position.z & 0x1F)
    );

    this->lightData = inp_lightData;
}

void ModelData::setPosition(glm::ivec3 inp_position) {
    this->position = static_cast<uint16_t>(
        ((inp_position.x & 0x1F) << 10) |
        ((inp_position.y & 0x1F) << 5) |
        (inp_position.z & 0x1F)
    );
}

void ModelData::setModelId(const int model_id) {
    rotation9ModelId10Variant13 = (rotation9ModelId10Variant13 & 0xFF801FFF) | ((model_id & 0x3FF) << 13);
}

void ModelData::setRotation(const int rotation) {
    rotation9ModelId10Variant13 = (rotation9ModelId10Variant13 & 0x007FFFFF) | ((rotation & 0x1FF) << 23);
}

void ModelData::setVariant(const int variant) {
    rotation9ModelId10Variant13 = (rotation9ModelId10Variant13 & 0xFFFFE000) | (variant & 0x1FFF);
}


void ModelData::setLightData(const LightData light_data) {
    lightData = light_data;
}


uint32_t ModelData::getModelId() const {
    return (rotation9ModelId10Variant13 & 0x007FE000) >> 13;
}

uint32_t ModelData::getVariant() const {
    return rotation9ModelId10Variant13 & 0x1FFF;
}

glm::ivec3 ModelData::getPosition() const {
    return {
        (position >> 10) & 0x1F,
        (position >> 5) & 0x1F,
        position & 0x1F
    };
}

uint32_t ModelData::getRotation() const {
    return (rotation9ModelId10Variant13 & 0xFF800000) >> 23;
}

LightData ModelData::getLightData() const {
    return lightData;
}

//--------------------------------------------------------
//                  FaceData
//--------------------------------------------------------

void FaceData::set(const glm::ivec3 position, const DIRECTION direction, const uint8_t variant,
    const uint16_t textureID, const LightData lighting, const uint16_t flags) {
    // Pack pos15Norm3Var4TexID10
    // Pos (15b): X (5b) | Y (5b) | Z (5b)
    // Norm/Direction (3b), Variant (4b), TextureID (10b)
    pos15Norm3Var4TexID10 = static_cast<uint32_t>(
        ((position.x & 0x1F) << 27) |
        ((position.y & 0x1F) << 22) |
        ((position.z & 0x1F) << 17) |
        ((direction & 0x07) << 14) |
        ((variant & 0x0F) << 10) |
        (textureID & 0x3FF)
    );

    // Pack light16Flags16
    // Light (16b), Flags (16b)
    light16Flags16 = static_cast<uint32_t>(
        ((lighting.getSRGB() & 0xFFFF) << 16) |
        (flags & 0xFFFF)
    );
}

void FaceData::setPosition(const glm::ivec3 position) {
    pos15Norm3Var4TexID10 = (pos15Norm3Var4TexID10 & 0x0001FFFF) |
        ((position.x & 0x1F) << 27) |
        ((position.y & 0x1F) << 22) |
        ((position.z & 0x1F) << 17);
}

void FaceData::setDirection(const DIRECTION direction) {
    pos15Norm3Var4TexID10 = (pos15Norm3Var4TexID10 & 0xFFFE3FFF) | ((direction & 0x07) << 14);
}

void FaceData::setVariant(const uint8_t variant) {
    pos15Norm3Var4TexID10 = (pos15Norm3Var4TexID10 & 0xFFFFC3FF) | ((variant & 0x0F) << 10);
}

void FaceData::setTextureID(const uint16_t textureID) {
    pos15Norm3Var4TexID10 = (pos15Norm3Var4TexID10 & 0xFFFFFC00) | (textureID & 0x3FF);
}

void FaceData::setLighting(const uint16_t lighting) {
    light16Flags16 = (light16Flags16 & 0x0000FFFF) | ((lighting & 0xFFFF) << 16);
}

void FaceData::setFlags(const uint16_t flags) {
    light16Flags16 = (light16Flags16 & 0xFFFF0000) | (flags & 0xFFFF);
}

glm::ivec3 FaceData::getPosition() const {
    return {
        (pos15Norm3Var4TexID10 >> 27) & 0x1F,
        (pos15Norm3Var4TexID10 >> 22) & 0x1F,
        (pos15Norm3Var4TexID10 >> 17) & 0x1F
    };
}

DIRECTION FaceData::getDirection() const {
    return static_cast<DIRECTION>((pos15Norm3Var4TexID10 >> 14) & 0x07);
}

uint8_t FaceData::getVariant() const {
    return (pos15Norm3Var4TexID10 >> 10) & 0x0F;
}

uint16_t FaceData::getTextureID() const {
    return pos15Norm3Var4TexID10 & 0x3FF;
}

uint16_t FaceData::getLighting() const {
    return (light16Flags16 >> 16) & 0xFFFF;
}

uint16_t FaceData::getFlags() const {
    return light16Flags16 & 0xFFFF;
}

//--------------------------------------------------------
//                  ChunkData
//--------------------------------------------------------

ChunkData::ChunkData(const glm::ivec3& chunk_position)
    : chunkPosition(chunk_position), bounding_box(chunk_position * SIZE, (chunk_position+1) * SIZE)
{
}

VoxelData ChunkData::get(const int x, const int y, const int z) const  {
    if (layers[y]) {
        return (*layers[y])[x + z * SIZE];
    }
    return uniformVoxel[y];
}

void ChunkData::set(const int x, const int y, const int z, const VoxelData value) {
    if (!layers[y]) {
        if (uniformVoxel[y] == value) return;
        // Promote to partial layer on first differing voxel
        layers[y] = std::make_unique<std::array<VoxelData, SIZE * SIZE>>();
        layers[y]->fill(uniformVoxel[y]);
    }
    (*layers[y])[x + z * SIZE] = value;
}


void ChunkData::generateNoise(const glm::ivec3 chunk_position) {

    std::mt19937 gen(1);
    std::uniform_int_distribution dist(0, 3);

    for (auto & layer : layers) {
        layer = std::make_unique<std::array<VoxelData, SIZE * SIZE>>();

        for (auto & voxel : *layer) {
            voxel.setID(dist(gen));
        }
    }

}

void ChunkData::updateEdges() {
    constexpr int SizeM1 = SIZE - 1;
    using EdgeMaskArray = std::array<uint32_t, SIZE>;

    auto computeFace = [this](const DIRECTION dir, auto getCoord) {
        if (!solidEdges[dir]) {
            solidEdges[dir] = std::make_unique<EdgeMaskArray>();
        }

        auto& maskArray = *solidEdges[dir];
        for (int i = 0; i < SIZE; ++i) {
            uint32_t rowMask = 0;
            for (int j = 0; j < SIZE; ++j) {
                if (const glm::ivec3 coord = getCoord(i, j); VoxelType::FromID(get(coord.x, coord.y, coord.z).getID())->isSolid) {
                    rowMask |= (1u << j);
                }
            }
            maskArray[i] = rowMask;
        }
    };

    computeFace(posX, [=](const int i, const int j) { return glm::ivec3(SizeM1, i, j); });
    computeFace(negX, [=](const int i, const int j) { return glm::ivec3(0,      i, j); });

    computeFace(posY, [=](const int i, const int j) { return glm::ivec3(i, SizeM1, j); });
    computeFace(negY, [=](const int i, const int j) { return glm::ivec3(i,      0, j); });

    computeFace(posZ, [=](const int i, const int j) { return glm::ivec3(i, j, SizeM1); });
    computeFace(negZ, [=](const int i, const int j) { return glm::ivec3(i, j,      0); });
}
