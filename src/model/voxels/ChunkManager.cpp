//
// Created by beni on 2026. 09. 12..
//

#include "ChunkManager.h"

#include <iostream>

#include "model/noise/WorldGeneration.h"


ChunkManager::ChunkManager(IWorldGenerator* worldGenerator, const glm::ivec3 centerIndex, const int renderDistanceHorizontal, const int renderDistanceVertical)
	: centerChunkCoordinate(centerIndex), renderDistanceHorizontal(renderDistanceHorizontal),
	  renderDistanceVertical(renderDistanceVertical), worldGenerator(worldGenerator) {
	//saveManager = new SaveFileManager("game");

	CalculateTotalAndMinAndMaxCorner();

	chunkMap.reserve(loadedChunkCount);
	dirtyChunkList.reserve(loadedChunkCount);

	boundingBox.IterateOverAllPositions([this](const glm::ivec3 position) {
		LoadOrGenerateChunkAt(position);
	});
}

ChunkManager::~ChunkManager()
{
	/*
	for (auto& pair : chunkMap) {

		if (dirtyChunkList.count(pair.first) || !saveManager->IsChunkSaved(pair.first)) {
			saveManager->SaveChunk(pair.first, pair.second);
		}
		delete pair.second;
	}
	*/
	//delete saveManager;
	chunkMap.clear();
}

ChunkData* ChunkManager::GetChunkAt(const glm::ivec3& position) const
{
	if (!IsChunkAt(position))
		return nullptr;

	return chunkMap.at(position).get();
}

inline bool ChunkManager::IsChunkAt(const glm::ivec3& position) const
{
	return chunkMap.contains(position);
}


void ChunkManager::CalculateTotalAndMinAndMaxCorner() {
	boundingBox.setMinCorner(glm::ivec3(
		centerChunkCoordinate.x - renderDistanceHorizontal,
		centerChunkCoordinate.y - renderDistanceVertical,
		centerChunkCoordinate.z - renderDistanceHorizontal
	));

	boundingBox.setMaxCorner(glm::ivec3(
		centerChunkCoordinate.x + renderDistanceHorizontal,
		centerChunkCoordinate.y + renderDistanceVertical,
		centerChunkCoordinate.z + renderDistanceHorizontal
	));

	loadedChunkCount = (renderDistanceHorizontal * 2 + 1) * (renderDistanceVertical * 2 + 1);
}

void ChunkManager::LoadOrGenerateChunkAt(glm::ivec3 position)
{
	std::cout << "LoadOrGenerateChunkAt(" << position.x << "," << position.y << ", " << position.z << ")" << std::endl;

	assert(!IsChunkAt(position) && "Chunk already in chunkMap");

	auto chunk = std::make_unique<ChunkData>(position);

	/*
	if (saveManager->IsChunkSaved(position))
	{
		saveManager->LoadChunkInto(position, chunk);
	}
	else
	{*/
	worldGenerator->GenerateChunk(chunk.get());
	dirtyChunkList.insert(position);
	//}

	chunkMap.emplace(position, std::move(chunk));
}

void ChunkManager::UnloadChunkAt(const glm::ivec3& position)
{
	std::cout << "UnloadChunkAt(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

	const auto it = chunkMap.find(position);

	assert(it != chunkMap.end() && "Tried to unload nonexistent chunk");

	if (it != chunkMap.end())
	{
		// save it
		/*
		if (dirtyChunkList.count(position) || !saveManager->IsChunkSaved(position)) {
			saveManager->SaveChunk(position, it->second);
			dirtyChunkList.erase(position);
		}
		*/

		// delete it
		chunkMap.erase(it);
	}
}


CenterChangedEvent ChunkManager::SetCenterPosition(const glm::ivec3& newPosition)
{
	// No movement
	if (newPosition == centerChunkCoordinate) return;

	const BoundingBox oldBoundingBox = boundingBox;

	CenterChangedEvent centerChangedEvent;

	centerChunkCoordinate = newPosition;
	CalculateTotalAndMinAndMaxCorner();

	///////////////////////
	// UNLOADING
	///////////////////////

	 oldBoundingBox.IterateOverAllPositions([this, &centerChangedEvent](const glm::ivec3 position) {
	 	if (!boundingBox.isInside(position)) {
	 		UnloadChunkAt(position);
			centerChangedEvent.unloadedPositions.insert(position);
	 	}
	 });

	///////////////////////
	// LOADING
	///////////////////////

	boundingBox.IterateOverAllPositions([this, &centerChangedEvent](const glm::ivec3 position) {
		if (!chunkMap.contains(position)) {
			LoadOrGenerateChunkAt(position);
			centerChangedEvent.loadedPositions.insert(position);
		}
	});

	return centerChangedEvent;
}