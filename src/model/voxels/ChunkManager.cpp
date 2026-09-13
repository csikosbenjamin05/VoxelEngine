//
// Created by beni on 2026. 09. 12..
//

#include "ChunkManager.h"

#include <iostream>

#include "model/noise/WorldGeneration.h"


ChunkManager::ChunkManager(IWorldGenerator* worldGenerator, const glm::ivec3 centerIndex, const int renderDistanceHorizontal, const int renderDistanceVertical)
	: dirtyChunkList(), centerChunkCoordinate(centerIndex), renderDistanceHorizontal(renderDistanceHorizontal),
	  renderDistanceVertical(renderDistanceVertical), chunkMap(), worldGenerator(worldGenerator)
{
	//saveManager = new SaveFileManager("game");

	CalculateTotalAndMinAndMaxCorner();

	chunkMap.reserve(loadedChunkCount);
	dirtyChunkList.reserve(loadedChunkCount);



	for (int i = GetLeftMostChunkIndex(); i <= GetRightMostChunkIndex(); i++)
		LoadOrGenerateChunkAt(i);
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

	return chunkMap.at(position);
}

inline bool ChunkManager::IsChunkAt(const glm::ivec3& position) const
{
	return chunkMap.contains(position);
}


void ChunkManager::CalculateTotalAndMinAndMaxCorner() {
	minCorner = glm::ivec3(
		centerChunkCoordinate.x - renderDistanceHorizontal,
		centerChunkCoordinate.y - renderDistanceVertical,
		centerChunkCoordinate.z - renderDistanceHorizontal
	);

	maxCorner = glm::ivec3(
		centerChunkCoordinate.x + renderDistanceHorizontal,
		centerChunkCoordinate.y + renderDistanceVertical,
		centerChunkCoordinate.z + renderDistanceHorizontal
	);

	loadedChunkCount = (renderDistanceHorizontal * 2 + 1) * (renderDistanceVertical * 2 + 1);
}

void ChunkManager::LoadOrGenerateChunkAt(glm::ivec3 &position)
{
	std::cout << "LoadOrGenerateChunkAt(" << position.x << "," << position.y << ", " << position.z << ")" << std::endl;

	assert(!IsChunkAt(position) && "Chunk already in chunkMap");

	auto chunk = new ChunkData(position);

	/*
	if (saveManager->IsChunkSaved(position))
	{
		saveManager->LoadChunkInto(position, chunk);
	}
	else
	{*/
	worldGenerator->GenerateChunkAt(position, chunk);

	dirtyChunkList.insert(position);
	//}

	chunk->RecalculateSurfaceHeights();
	chunkMap[position] = chunk;
}

void ChunkManager::UnloadChunkAt(int position)
{
	std::cout << "UnloadChunkAt(" << position << ")" << std::endl;
	auto it = chunkMap.find(position);

	assert(it != chunkMap.end() && "Tried to unload nonexistent chunk");

	if (it != chunkMap.end())
	{
		// save it
		if (dirtyChunkList.count(position) || !saveManager->IsChunkSaved(position)) {
			saveManager->SaveChunk(position, it->second);
			dirtyChunkList.erase(position);
		}

		// delete it
		delete it->second;
		chunkMap.erase(it);
	}
}


void ChunkManager::SetCenterIndex(int index)
{
	int diff = centerChunkCoordinate - index;
	int oldLeft = GetLeftMostChunkIndex();
	int oldRight = GetRightMostChunkIndex();

	centerChunkCoordinate = index;

	int newLeft = GetLeftMostChunkIndex();
	int newRight = GetRightMostChunkIndex();

	bool movedLeft = diff > 0;
	bool movedRight = diff < 0;

	// unload old chunks

	if (movedLeft) // left movement, unload from right
	{

		int unloadBoundary = std::max(newRight + 1, oldLeft);

		for (int i = oldRight; i >= unloadBoundary; i--)
			UnloadChunkAt(i);

	}
	else if (movedRight) // right movement, unload from left
	{

		int unloadBoundary = std::min(newLeft - 1, oldRight);

		for (int i = oldLeft; i <= unloadBoundary; i++)
			UnloadChunkAt(i);

	}

	// load or generate new ones

	if (movedLeft) // left movement, load from left
	{
		int loadBoundry = std::min(newRight, oldLeft - 1);

		for (int i = newLeft; i <= loadBoundry; i++)
			LoadOrGenerateChunkAt(i);

	}
	else if (movedRight) // right movement, load from right
	{
		int loadBoundry = std::max(oldRight + 1, newLeft);

		for (int i = newRight; i >= loadBoundry; i--)
			LoadOrGenerateChunkAt(i);
	}
}