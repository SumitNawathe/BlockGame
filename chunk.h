#pragma once

#include <vector>
#include <map>
#include <tuple>
#include <glm/glm.hpp>
#include "globals.h"
#include "block.h"

class Chunk {
public:
	typedef typename std::vector<BlockVertex> ChunkBlockMesh;

	glm::vec3 chunkPosition;
	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	Chunk(glm::vec3 chunkPosition, Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);

	void regenerateEdge(Direction dir, Chunk* neighbor, ChunkBlockMesh& output);
	void getSingleBlockMesh(unsigned int i, unsigned int j, unsigned int k,
		std::map<Direction, Chunk*> neighbors, ChunkBlockMesh& output, bool forceEdge = false);
	void getBlockMesh(std::map<Direction, Chunk*> neighbors, ChunkBlockMesh& output, bool forceEdge = false);
	ChunkBlockMesh getBlockMesh(std::map<Direction, Chunk*> neighbors, bool forceEdge = false);
	ChunkBlockMesh getMeshUnoptimized();


private:
	void generate();
	void getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k, ChunkBlockMesh& output);
	ChunkBlockMesh getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k);
};

