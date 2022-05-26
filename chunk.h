#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <map>
#include <tuple>
#include <glm/glm.hpp>
#include "block.h"

constexpr unsigned int CHUNK_SIZE = 16;

class Chunk {
public:
	typedef typename std::vector<BlockVertex> ChunkBlockMesh;

	glm::vec3 chunkPosition;
	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	Chunk(glm::vec3 chunkPosition, Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);

	void getBlockMesh(std::map<Direction, Chunk*> neighbors, ChunkBlockMesh& output, bool forceEdge = true);
	ChunkBlockMesh getBlockMesh(std::map<Direction, Chunk*> neighbors, bool forceEdge = true);
	ChunkBlockMesh getMeshUnoptimized();


private:
	void generate();
	void getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k, ChunkBlockMesh& output);
	ChunkBlockMesh getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k);
};

#endif

