#include "chunkmanager.h"

#include <iostream>
#include <glad/glad.h>


/* floor function from float to int
* ex: +2.4 -> 2, -2.4 ->3, -4.0 -> -4
*/
int ffloor(float input) {
	if (input >= 0.0f) return static_cast<int>(input);
	int truncated = static_cast<int>(input);
	if ((float)truncated == input) return truncated;
	return truncated - 1;
}


/* ceiling function from float to int
* ex: +2.4 -> 3, -2.4 -> -2, -4.0 -> -4
*/
int fceil(float input) {
	if (input <= 0.0f) return static_cast<int>(input);
	int truncated = static_cast<int>(input);
	if ((float)truncated == input) return truncated;
	return truncated + 1;
}


/*
* constructor
* @param playerSpawnPos: the player's initial position, around which chunks will be loaded
* @param loadedRadius: radius by which chunk coordinate component can deviate for loading
*/
ChunkManager::ChunkManager(glm::vec3 playerSpawnPos, int rad) {
	this->loadedRadius = rad;

	int playerChunkX = ffloor(playerSpawnPos.x / (float)CHUNK_SIZE);
	int playerChunkY = ffloor(playerSpawnPos.y / (float)CHUNK_SIZE);
	int playerChunkZ = ffloor(playerSpawnPos.z / (float)CHUNK_SIZE);
	for (int i = playerChunkX - rad; i <= playerChunkX + rad; i++)
		for (int j = playerChunkY - rad; j <= playerChunkY + rad; j++)
			for (int k = playerChunkZ - rad; k <= playerChunkZ + rad; k++)
				loadChunk(i, j, k);
}


std::map<Direction, Chunk*> ChunkManager::getNeighbors(int i, int j, int k) const {
	std::map<Direction, Chunk*> ret;
	ret.insert({ Direction::NEGX, (chunkData.contains(std::make_tuple(i - 1, j, k))) ? chunkData.find(std::make_tuple(i - 1, j, k))->second.chunk : nullptr });
	ret.insert({ Direction::POSX, (chunkData.contains(std::make_tuple(i + 1, j, k))) ? chunkData.find(std::make_tuple(i + 1, j, k))->second.chunk : nullptr });
	ret.insert({ Direction::NEGY, (chunkData.contains(std::make_tuple(i, j - 1, k))) ? chunkData.find(std::make_tuple(i, j - 1, k))->second.chunk : nullptr });
	ret.insert({ Direction::POSY, (chunkData.contains(std::make_tuple(i, j + 1, k))) ? chunkData.find(std::make_tuple(i, j + 1, k))->second.chunk : nullptr });
	ret.insert({ Direction::NEGZ, (chunkData.contains(std::make_tuple(i, j, k - 1))) ? chunkData.find(std::make_tuple(i, j, k - 1))->second.chunk : nullptr });
	ret.insert({ Direction::POSZ, (chunkData.contains(std::make_tuple(i, j, k + 1))) ? chunkData.find(std::make_tuple(i, j, k + 1))->second.chunk : nullptr });
	return ret;
}


/* send chunk data to chunk's VAO?/VBO */
void ChunkManager::depositChunkCache(ChunkData& cd) const {
	glBindVertexArray(cd.chunkVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cd.chunkVBO);
	glBufferData(GL_ARRAY_BUFFER, cd.cache->size() * sizeof(BlockVertex), cd.cache->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*) offsetof(BlockVertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, texCoords));
	glEnableVertexAttribArray(2);
}


/* draws all chunks after accumulating the meshes from caches and regenerations */
void ChunkManager::draw() {
	// update chunk caches if necessary
	for (auto& entry : chunkData) {
		ChunkData& cd = entry.second;
		if (cd.shouldBeUpdated) {
			glm::vec3 chunkCoords = cd.chunk->chunkPosition;
			int cx = (int)(chunkCoords.x), cy = (int)(chunkCoords.y), cz = (int)(chunkCoords.z);
			std::cout << "updatating chunk i=" << cx << ", j=" << cy << ", k=" << cz << std::endl;
			cd.cache->clear();
			cd.chunk->getBlockMesh(getNeighbors(cx, cy, cz), *cd.cache, cx == 0 && cy == 0 && cz == 0);
			if (!cd.loaded) {
				cd.loaded = true;
				if (chunkData.contains(std::make_tuple(cx - 1, cy, cz))) chunkData.find(std::make_tuple(cx - 1, cy, cz))->second.shouldBeUpdated = true;
				if (chunkData.contains(std::make_tuple(cx + 1, cy, cz))) chunkData.find(std::make_tuple(cx + 1, cy, cz))->second.shouldBeUpdated = true;
				if (chunkData.contains(std::make_tuple(cx, cy - 1, cz))) chunkData.find(std::make_tuple(cx, cy - 1, cz))->second.shouldBeUpdated = true;
				if (chunkData.contains(std::make_tuple(cx, cy + 1, cz))) chunkData.find(std::make_tuple(cx, cy + 1, cz))->second.shouldBeUpdated = true;
				if (chunkData.contains(std::make_tuple(cx, cy, cz - 1))) chunkData.find(std::make_tuple(cx, cy, cz - 1))->second.shouldBeUpdated = true;
				if (chunkData.contains(std::make_tuple(cx, cy, cz + 1))) chunkData.find(std::make_tuple(cx, cy, cz + 1))->second.shouldBeUpdated = true;
				break; // limit 1 chunk generation per draw call
			}
			cd.shouldBeUpdated = false;
			depositChunkCache(cd);
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


/*
* loads/unloads chunks in response to player movement
* @param playerPos: updated player position;
*/
void ChunkManager::updatePlayerPos(glm::vec3 playerPos) {
	// check if player has moved
	int newX = ffloor(playerPos.x / (float)CHUNK_SIZE);
	int newY = ffloor(playerPos.y / (float)CHUNK_SIZE);
	int newZ = ffloor(playerPos.z / (float)CHUNK_SIZE);
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
ChunkManager::ChunkDataIterator ChunkManager::unloadChunk(ChunkManager::ChunkDataIterator it) {
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


int nextIntUp(float input) {
	int ans = fceil(input);
	if ((float)ans == input)
		return ans + 1;
	return ans;
}

int nextIntDown(float input) {
	int ans = ffloor(input);
	if ((float)ans == input)
		return ans - 1;
	return ans;
}


bool ChunkManager::breakBlock(glm::vec3 p, glm::vec3 v, float maxDist) {
	if (maxDist > 10.0f) { std::cout << "ERROR: maxDist (= " << maxDist << ") > 10.0f" << std::endl; exit(-1); }
	else if (maxDist < 0.0f) { std::cout << "ERROR: maxDist (=" << maxDist << ") < 0.0f" << std::endl; exit(-1); }
	else {
		std::cout << "maxDist = " << maxDist << ", p = (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
	}

	// get chunk
	int cpx = ffloor(p.x / (float)CHUNK_SIZE),
		cpy = ffloor(p.y / (float)CHUNK_SIZE),
		cpz = ffloor(p.z / (float)CHUNK_SIZE);
	auto cdf = chunkData.find(std::make_tuple(cpx, cpy, cpz));
	if (cdf == chunkData.end())
		return false; // breaking block in chunk that doesn't exist
	Chunk* chunk = cdf->second.chunk;
	
	// get coord in chunk
	int cx = ffloor(p.x - (float)CHUNK_SIZE * cpx),
		cy = ffloor(p.y - (float)CHUNK_SIZE * cpy),
		cz = ffloor(p.z - (float)CHUNK_SIZE * cpz);
	if (chunk->blocks[cx][cy][cz].isSolid()) {
		// found block, break it
		std::cout << "breaking block " << std::endl;
		chunk->blocks[cx][cy][cz] = Block(BlockType::AIR, 0);
		cdf->second.shouldBeUpdated = true;
		// if on edge, update neighbor
		if (cx == 0) {
			auto temp = chunkData.find(std::make_tuple(cpx - 1, cpy, cpz));
			if (temp != chunkData.end()) temp->second.shouldBeUpdated = true;
		}
		if (cx == CHUNK_SIZE - 1) {
			auto temp = chunkData.find(std::make_tuple(cpx + 1, cpy, cpz));
			if (temp != chunkData.end()) temp->second.shouldBeUpdated = true;
		}
		if (cy == 0) {
			auto temp = chunkData.find(std::make_tuple(cpx, cpy - 1, cpz));
			if (temp != chunkData.end()) temp->second.shouldBeUpdated = true;
		}
		if (cy == CHUNK_SIZE - 1) {
			auto temp = chunkData.find(std::make_tuple(cpx, cpy + 1, cpz));
			if (temp != chunkData.end()) temp->second.shouldBeUpdated = true;
		}
		if (cz == 0) {
			auto temp = chunkData.find(std::make_tuple(cpx, cpy, cpz - 1));
			if (temp != chunkData.end()) temp->second.shouldBeUpdated = true;
		}
		if (cz == CHUNK_SIZE - 1) {
			auto temp = chunkData.find(std::make_tuple(cpx, cpy, cpz + 1));
			if (temp != chunkData.end()) temp->second.shouldBeUpdated = true;
		}
		return true;
	}

	// block not breakable, calculate next position
	float dx = (v.x >= 0) ? (float)nextIntUp(p.x) - p.x : p.x - (float)nextIntDown(p.x),
		dy = (v.y >= 0) ? (float)nextIntUp(p.y) - p.y : p.y - (float)nextIntDown(p.y),
		dz = (v.z >= 0) ? (float)nextIntUp(p.z) - p.z : p.z - (float)nextIntDown(p.z);
	float min = std::min(std::min(dx / std::abs(v.x), dy / std::abs(v.y)), dz / std::abs(v.z)) + 0.001f;
	if (min > maxDist)
		return false; // next step would be too far
	return breakBlock(p + min * v, v, maxDist - min);
}


