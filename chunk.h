#ifndef CHUNK_H
#define CHUNK

#include <vector>
#include <glm/glm.hpp>
#include "block.h"

constexpr unsigned int CHUNK_SIZE = 16;

class Chunk {
public:
	glm::vec3 chunkPosition;
	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	Chunk(glm::vec3 chunkPosition, Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);

	void getSideMesh(Direction, std::vector<BlockVertex>& output);
	std::vector<BlockVertex> getSideMesh(Direction direction);
	void getInteriorMesh(std::vector<BlockVertex>& output);
	std::vector<BlockVertex> getInteriorMesh();
	std::vector<BlockVertex> getMeshUnoptimized();

private:
	void generate();
	void getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k, std::vector<BlockVertex>& output);
	std::vector<BlockVertex> getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k);
};

#endif

