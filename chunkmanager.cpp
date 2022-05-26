#include "chunkmanager.h"

#include <iostream>
#include <glad/glad.h>

/*
* constructor
* @param playerSpawnPos: the player's initial position, around which chunks will be loaded
* @param loadedRadius: radius by which chunk coordinate component can deviate for loading
*/
ChunkManager::ChunkManager(glm::vec3 playerSpawnPos, int rad) {
	this->loadedRadius = rad;

	int playerChunkX = (int)(playerSpawnPos.x / (float)CHUNK_SIZE);
	int playerChunkY = (int)(playerSpawnPos.y / (float)CHUNK_SIZE);
	int playerChunkZ = (int)(playerSpawnPos.z / (float)CHUNK_SIZE);
	for (int i = playerChunkX - rad; i <= playerChunkX + rad; i++)
		for (int j = playerChunkY - rad; j <= playerChunkY + rad; j++)
			for (int k = playerChunkZ - rad; k <= playerChunkZ + rad; k++)
				loadChunk(i, j, k);
}


/* draws all chunks after accumulating the meshes from caches and regenerations */
void ChunkManager::draw() {
	// update chunk caches if necessary
	for (auto& entry : chunkData) {
		ChunkData& cd = entry.second;
		if (cd.shouldBeUpdated) {
			cd.chunk->getBlockMesh(std::map<Direction, Chunk*>(), *cd.cache);
			cd.loaded = true;
			cd.shouldBeUpdated = false;
			glBindVertexArray(cd.chunkVAO);
			glBindBuffer(GL_ARRAY_BUFFER, cd.chunkVBO);
			glBufferData(GL_ARRAY_BUFFER, cd.cache->size() * sizeof(BlockVertex), cd.cache->data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*) 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*) offsetof(BlockVertex, normal));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, texCoords));
			glEnableVertexAttribArray(2);
			break; // limit 1 chunk generation per draw call
		}
	}

	// draw all chunks
	for (auto& entry : chunkData) {
		ChunkData& cd = entry.second;
		if (cd.loaded && cd.cache->size() != 0) { // exclude air chunks
			glBindVertexArray(cd.chunkVAO);
			glBindBuffer(GL_ARRAY_BUFFER, cd.chunkVBO);
			glDrawArrays(GL_TRIANGLES, 0, cd.cache->size());
		}
	}
	glBindVertexArray(0);
}


/* rounds float always downwards; ex: +2.4 => +2, -2.4 => -3 */
int roundFloatDown(float input) {
	if (input < 0.0f)
		return static_cast<int>(input - 1.0f);
	else
		return static_cast<int>(input);
}


/*
* loads/unloads chunks in response to player movement
* @param playerPos: updated player position;
*/
void ChunkManager::updatePlayerPos(glm::vec3 playerPos) {
	// check if player has moved
	int newX = roundFloatDown(playerPos.x / (float)CHUNK_SIZE);
	int newY = roundFloatDown(playerPos.y / (float)CHUNK_SIZE);
	int newZ = roundFloatDown(playerPos.z / (float)CHUNK_SIZE);
	if (std::make_tuple(newX, newY, newZ) == lastPlayerChunkPos) return;
	std::cout << "player has moved chunks" << std::endl;
	lastPlayerChunkPos = std::make_tuple(newX, newY, newZ);

	// add new chunks within radius
	for (int i = newX - loadedRadius; i <= newX + loadedRadius; i++)
		for (int j = newY - loadedRadius; j <= newY + loadedRadius; j++)
			for (int k = newZ - loadedRadius; k <= newZ + loadedRadius; k++) {
				if (!chunkData.contains(std::make_tuple(i, j, k)))
					loadChunk(i, j, k);
			}

	// delete far away chunks
	for (auto it = chunkData.begin(); it != chunkData.end(); /* no increment */) {
		glm::vec3& chunkPos = it->second.chunk->chunkPosition;
		if (std::abs((int)chunkPos.x - newX) >= loadedRadius + 2 ||
				std::abs((int)chunkPos.y - newY) >= loadedRadius + 2 ||
				std::abs((int)chunkPos.z - newZ) >= loadedRadius + 2)
			it = unloadChunk(it);
		else it++;
	}
}


/*
* generates chunk, inserts into chunkData under chunk coordinates,
* generates VAO/VBO, does not generate/cache mesh
*/
void ChunkManager::loadChunk(int i, int j, int k) {
	std::cout << "loading chunk i=" << i << ", j=" << j << ", k=" << k << std::endl;
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
			
	Chunk* newChunk = new Chunk(glm::vec3(i, j, k), nullptr);
	Chunk::ChunkBlockMesh* newMesh = new Chunk::ChunkBlockMesh();
	chunkData.insert({ std::make_tuple(i, j, k), ChunkData {
		newChunk, newMesh, VBO, VAO, false, true
	} });
}


/* finds chunk using arguments as chunk coordinates, then calls overloaded version on iterator */
std::map<std::tuple<int, int, int>, ChunkManager::ChunkData>::iterator
ChunkManager::unloadChunk(int i, int j, int k) {
	auto data = chunkData.find(std::make_tuple(i, j, k));
	if (data == chunkData.end()) return chunkData.end();
	return unloadChunk(data);
}


/*
* unloads a single chunk, deallocates chunk and cache, deletes VAO/VBO
* @param it: map iterator pointing to ChunkData to be deleted; must be valid
* @returns iterator returned by map.erase()
*/
std::map<std::tuple<int, int, int>, ChunkManager::ChunkData>::iterator
ChunkManager::unloadChunk(std::map<std::tuple<int, int, int>, ChunkData>::iterator it) {
	glm::vec3& cp = it->second.chunk->chunkPosition;
	std::cout << "unloading chunk i=" << cp.x << ", j=" << cp.y << ", k=" << cp.z << std::endl;

	delete it->second.chunk;
	delete it->second.cache;
	glDeleteBuffers(1, &it->second.chunkVBO);
	glDeleteVertexArrays(1, &it->second.chunkVBO);
	return chunkData.erase(it);
}


/* loops through chunkData, calls unloadChunk on each */
ChunkManager::~ChunkManager() {
	for (auto it = chunkData.begin(); it != chunkData.end(); /* no increment */)
		it = unloadChunk(it);
}


