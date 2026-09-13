#pragma once
#include <glm/vec3.hpp>
#include <glm/detail/type_vec1.hpp>

class ChunkData;
class VoxelType;
class RandomFloatGenerator;

class ISeedable
{
public:
	virtual ~ISeedable() = default;
	virtual void SetSeed(RandomFloatGenerator * rfg) = 0;
};


// Base interface for ALL layers. Noise or formula layers that get added together to get the final noise.
class ILayer : public ISeedable
{
public:
	~ILayer() override = default;


protected:
	constexpr static float offsetRange = 10000.0f;
};

// Generic interface for any dimension
template <std::size_t Dim>
class ILayerND : public ILayer
{
public:
	using Coordinate = glm::vec<Dim, float>;

	~ILayerND() override = default;
	virtual float GetValueAt(const Coordinate& point) const = 0;
};

using ILayer1D = ILayerND<1>;
using ILayer2D = ILayerND<2>;
using ILayer3D = ILayerND<3>;


// Holds all Layers, computes the final noise value at a given coordinate, then converts it to a BlockType
class ITerrainGenerator : public ISeedable
{
public:
	~ITerrainGenerator() override = default;
	[[nodiscard]] virtual const VoxelType* GetVoxelTypeAt(glm::ivec3& voxelCoordinate, int surfaceHeight) const = 0;
	[[nodiscard]] virtual int GetSurfaceHeight(int x, int z) const = 0;
};

class ITreeGenerator : public ISeedable
{
public:
	~ITreeGenerator() override = default;
	virtual void PopulateChunk(ChunkData* chunk) = 0;
};

// Fully generates single chunk from a given seed and a position
// Can have multiple TerrainGenerators and TreeGenerators
class IWorldGenerator : public ISeedable
{
public:
	~IWorldGenerator() override = default;
	virtual void GenerateChunk(ChunkData* chunk) const = 0;
};

/*
// Inside the World Generator loop:
std::vector<ILayer1D*> layers;

void SetWorldSeed(long seed) {
	for (ILayer1D* layer : layers) {
		// Try to cast the generic 1D layer into a seedable one
		if (auto seedableLayer = dynamic_cast<ISeedable*>(layer)) {
			seedableLayer->SetSeed(seed); // It's a noise layer, configure it!
		}
		// If it returns nullptr, it's a seedless layer (like ConstantLayer1D). It's skipped safely!
	}
}
*/