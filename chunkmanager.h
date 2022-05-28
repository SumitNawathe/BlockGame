#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <vector>
#include <map>
#include <utility>
#include <tuple>
#include <optional>
#include <glm/glm.hpp>

#include "chunk.h"


class ChunkManager {
public:
	ChunkManager(glm::vec3 playerSpawnPos = glm::vec3(0.0f, 0.0f, 0.0f), int rad = 2);
	~ChunkManager();

	void draw();
	void updatePlayerPos(glm::vec3 playerPos);
	bool breakBlock(glm::vec3 playerPos, glm::vec3 playerDir, float maxDist = 4.0f);
	bool placeBlock(glm::vec3 playerPos, glm::vec3 playerDir, float maxDist = 4.0f);

private:
	/* internal data structure for caching chunk meshes */
	struct ChunkData {
		Chunk* chunk; // heap-allocated
		Chunk::ChunkBlockMesh* cache; // heap-allocated
		unsigned int chunkVBO;
		unsigned int chunkVAO;
		bool loaded;
		bool shouldBeUpdated;
	};
	typedef typename std::map<std::tuple<int, int, int>, ChunkData>::iterator ChunkDataIterator;

	std::map<std::tuple<int, int, int>, ChunkData> chunkData;
	int loadedRadius;
	std::tuple<int, int, int> lastPlayerChunkPos;

	std::map<Direction, Chunk*> getNeighbors(int i, int j, int k) const;
	void depositChunkCache(ChunkData& cd) const;
	void loadChunk(int i, int j, int k);
	ChunkDataIterator unloadChunk(int i, int j, int k);
	ChunkDataIterator unloadChunk(ChunkDataIterator it);

	bool performPlaceBlock(int cx, int cy, int cz, int bx, int by, int bz);
};

#endif

