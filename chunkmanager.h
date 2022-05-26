#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <vector>
#include <map>
#include <utility>
#include <tuple>
#include <glm/glm.hpp>

#include "chunk.h"


class ChunkManager {
public:
	ChunkManager(glm::vec3 playerSpawnPos = glm::vec3(0.0f, 0.0f, 0.0f), int rad = 1);

	void draw();
	void updatePlayerPos(glm::vec3 playerPos);

private:
	/* internal data structure for caching chunk meshes */
	struct ChunkData {
		Chunk chunk;
		Chunk::ChunkBlockMesh cache;
		unsigned int chunkVBO;
		unsigned int chunkVAO;
		bool loaded;
		bool shouldBeUpdated;
	};

	std::map<std::tuple<int, int, int>, ChunkData> chunkData;
	unsigned int loadedRadius;
};

#endif

