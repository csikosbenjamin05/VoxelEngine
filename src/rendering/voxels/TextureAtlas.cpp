//
// Created by beni on 2026. 09. 05..
//

#include "TextureAtlas.h"

#include <string>
#include <iostream>
#include <cassert>



#include "model/voxels/ChunkData.h"
#include "model/voxels/VoxelType.h"


std::string TextureCoordinate::toString() const
{
	return std::to_string(uMin) + " " + std::to_string(vMin) + " - " + std::to_string(uMax) + " " + std::to_string(vMax);
}

TextureAtlas::TextureAtlas(const int voxelTextureSize)
	: voxelTextureSize(static_cast<float>(voxelTextureSize))
{
}

void TextureAtlas::Init(const std::filesystem::path& voxelTexturesPath)
{
	voxelSampler.init(
		Sampler2D::WrapMode::REPEAT, Sampler2D::WrapMode::REPEAT,
		Sampler2D::FilterMode::NEAREST, Sampler2D::FilterMode::NEAREST_MIPMAP_NEAREST
	);

	voxelAtlasTexture.init(voxelTexturesPath, 32, 32, true);
}


void TextureAtlas::Clean()
{
	voxelSampler.clean();
	voxelAtlasTexture.clean();
}

void TextureAtlas::UseVoxelAtlas(const unsigned int textureUnit) const {
	voxelSampler.use(textureUnit);
	voxelAtlasTexture.use(textureUnit);
}

/*
TextureCoordinate TextureAtlas::GetUVRangeForVoxelID(const uint16_t id) const
{
	assert((VoxelIdToTextureCoordsMap.contains(id)) && "Unknown Block ID");
	return VoxelIdToTextureCoordsMap.at(id);
}

TextureCoordinate TextureAtlas::GetUVRangeForVoxelType(const VoxelType* voxelType) const
{
	return GetUVRangeForVoxelID(voxelType->id);;
}


TextureCoordinate TextureAtlas::CalculateUVRangeForVoxelId(const uint16_t id) const
{
	TextureCoordinate tc;

	// 256*32 jelenleg az atlasz -> pixelY konstans
	const float pixelX = static_cast<float>(id) * voxelTextureSize;
	constexpr float pixelY = 0;

	const auto width = static_cast<float>(voxelAtlasTexture.getWidth());
	const auto height = static_cast<float>(voxelAtlasTexture.getHeight());

	tc.uMin = pixelX / width;
	tc.uMax = (pixelX + voxelTextureSize) / width;
	tc.vMin = pixelY / height;
	tc.vMax = (pixelY + voxelTextureSize) / height;

	return tc;
}

void TextureAtlas::CalculateUVsForAllVoxels()
{
	const auto blockTypeMap = VoxelType::GetRegistry();

	VoxelIdToTextureCoordsMap.reserve(blockTypeMap.size());

	for (const auto&[id, ref] : blockTypeMap)
	{
		TextureCoordinate tc = CalculateUVRangeForVoxelId(ref->id);
		VoxelIdToTextureCoordsMap[id] = tc;
		std::cout << "Calculated uv for " << ref->name << "[" << id << "] : (" << tc.toString() << "\n";
	}
}
*/