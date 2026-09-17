#include "MyTerrainGenerator.h"

#include <model/noise/generators/RandomFloatGenerator.h>
#include <model/noise/PerlinNoise.h>
#include <model/voxels/ChunkData.h>
#include <model/voxels/VoxelType.h>

#include <glm/exponential.hpp>

#include <cmath>


MyTerrainGenerator::MyTerrainGenerator(PerlinNoise* perlinNoise, RandomFloatGenerator* rfg)
	: baseHeightLayer(perlinNoise, rfg, 1.0f / 60.0f),
detailHeightLayer(perlinNoise, rfg, 1.0f / 10.0f),
detailAmplitudeLayer(perlinNoise, rfg, 1.0f / 90.0f)
{
}

const VoxelType* MyTerrainGenerator::GetVoxelTypeAt(glm::ivec3& voxelCoordinate, const int surfaceHeight) const
{
	const int stone = surfaceHeight / 3 * 2; // 40%
	const int y = voxelCoordinate.y;

	if (y > surfaceHeight)
		return &VoxelType::Air;

	if (y == surfaceHeight)
		return &VoxelType::Grass;
	
	if (y > stone)
		return &VoxelType::Dirt;

	return &VoxelType::Stone;
}

void MyTerrainGenerator::SetSeed(RandomFloatGenerator* rfg)
{
	baseHeightLayer.SetSeed(rfg);
}

int MyTerrainGenerator::GetSurfaceHeight(const int x, const int z) const
{
	const float base = baseHeightLayer.GetValueAt(glm::vec2(x, z));

	const float detailAmplitude = detailAmplitudeLayer.GetValueAt(glm::vec2(x, z));
	float detail = detailHeightLayer.GetValueAt(glm::vec2(x, z)) * std::pow(detailAmplitude, 2);


	const auto final = static_cast<float>(std::pow<float>(base + detail, 2));

	return static_cast<int>(final * ChunkData::SIZE * 2.0f);
}
