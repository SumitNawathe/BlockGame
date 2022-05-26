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
			for (int k = playerChunkZ - rad; k <= playerChunkZ + rad; k++) {
				std::cout << "generating i=" << i << ", j=" << j << ", k=" << k << std::endl;
				unsigned int VBO, VAO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
			
				chunkData.insert({ std::make_tuple(i, j, k), ChunkData {
					Chunk(glm::vec3(i, j, k), nullptr),
					Chunk::ChunkBlockMesh(),
					VBO, VAO, false, true
				} });
			}
}


/* draws all chunks after accumulating the meshes from caches and regenerations */
void ChunkManager::draw() {
	// update chunk caches if necessary
	for (auto& entry : chunkData) {
		ChunkData& cd = entry.second;
		if (cd.shouldBeUpdated) {
			cd.chunk.getBlockMesh(std::map<Direction, Chunk*>(), cd.cache);
			cd.loaded = true;
			cd.shouldBeUpdated = false;
			glBindVertexArray(cd.chunkVAO);
			glBindBuffer(GL_ARRAY_BUFFER, cd.chunkVBO);
			glBufferData(GL_ARRAY_BUFFER, cd.cache.size() * sizeof(BlockVertex), cd.cache.data(), GL_STATIC_DRAW);

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
		if (cd.loaded && cd.cache.size() != 0) { // exclude air chunks
			glBindVertexArray(cd.chunkVAO);
			glBindBuffer(GL_ARRAY_BUFFER, cd.chunkVBO);
			glDrawArrays(GL_TRIANGLES, 0, cd.cache.size());
		}
	}
	glBindVertexArray(0);
}


/* TODO */
void ChunkManager::updatePlayerPos(glm::vec3 playerPos) {

}



