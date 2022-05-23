#include "chunk.h"

#include <vector>

/*
* Constructor for chunk
* @param chunkPosition: initializes chunkPosition
* @param inputBlocks: initializes blocks; if nullptr, Chunk::generate() will be run
*/
Chunk::Chunk(glm::vec3 chunkPosition, Block inputBlocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]) {
	this->chunkPosition = chunkPosition;
	if (inputBlocks == nullptr)
		generate();
	else {
		for (unsigned int i = 0; i < CHUNK_SIZE; i++)
			for (unsigned int j = 0; j < CHUNK_SIZE; j++)
				for (unsigned int k = 0; j < CHUNK_SIZE; k++)
					blocks[i][j][k] = inputBlocks[i][j][k];
	}
}


/*
* helper function to get the vertices of a block face in a given position and direction
* @param dir: the direction of the block face
* @param i, j, k: the coordinates of the block; if any are >= CHUNK_SIZE, empty vector returned
* @param output: vector to which vertices are added
*/
void Chunk::getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k, std::vector<BlockVertex>& output) {
	glm::vec3 offset = glm::vec3(0.5f, 0.5f, 0.5f) + glm::vec3((float) i, (float) j, (float) k) + (float) CHUNK_SIZE * chunkPosition;

	if (i >= CHUNK_SIZE || j >= CHUNK_SIZE || k >= CHUNK_SIZE)
		return;

	switch (dir) {
		case Direction::NEGX:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			break;
		case Direction::POSX:
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			break;
		case Direction::NEGY:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			break;
		case Direction::POSY:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			break;
		case Direction::NEGZ:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			break;
		case Direction::POSZ:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, 0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, 0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, 0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, 0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			break;
	}
}


/* equivalent to overloaded version, but returns vector instead of extending output parameter */
std::vector<BlockVertex> Chunk::getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k) {
	std::vector<BlockVertex> mesh;
	getBlockFace(dir, i, j, k, mesh);
	return mesh;
}


/*
* generates the mesh on a specific side of the chunk,
* returns an alloc'd pointer
* @param direction: which of 6 directions the mesh should be for
* @param output: vector to which faces are added
*/
void Chunk::getSideMesh(Direction direction, std::vector<BlockVertex>& output) {
	switch (direction) {
		case Direction::NEGX:
			for (unsigned int j = 0; j < CHUNK_SIZE; j++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					getBlockFace(direction, 0, j, k, output);
			break;
		case Direction::POSX:
			for (unsigned int j = 0; j < CHUNK_SIZE; j++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					getBlockFace(direction, CHUNK_SIZE - 1, j, k, output);
			break;
		case Direction::NEGY:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					getBlockFace(direction, i, 0, k, output);
			break;
		case Direction::POSY:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					getBlockFace(direction, i, CHUNK_SIZE - 1, k, output);
			break;
		case Direction::NEGZ:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int j = 0; j < CHUNK_SIZE; j++)
					getBlockFace(direction, i, j, 0, output);
			break;
		case Direction::POSZ:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int j = 0; j < CHUNK_SIZE; j++)
					getBlockFace(direction, i, j, CHUNK_SIZE - 1, output);
			break;
	}
}


/* equivalent to overloaded version, but returns vector instead of extending output parameter */
std::vector<BlockVertex> Chunk::getSideMesh(Direction dir) {
	std::vector<BlockVertex> mesh;
	getSideMesh(dir, mesh);
	return mesh;
}


/*
* generates the mesh for all interio-facing block sides
* @param output: vector into which faces are added
*/
void Chunk::getInteriorMesh(std::vector<BlockVertex>& output) {
	for (unsigned int i = 0; i < CHUNK_SIZE - 1; i++)
		for (unsigned int j = 0; j < CHUNK_SIZE - 1; j++)
			for (unsigned int k = 0; k < CHUNK_SIZE - 1; k++) {
				if (i != 0 && !blocks[i - 1][j][k].isSolid())
					getBlockFace(Direction::NEGX, i, j, k, output);
				if (i != CHUNK_SIZE - 1 && !blocks[i + 1][j][k].isSolid())
					getBlockFace(Direction::POSX, i, j, k, output);
				if (j != 0 && !blocks[i][j - 1][k].isSolid())
					getBlockFace(Direction::NEGY, i, j, k, output);
				if (j != CHUNK_SIZE - 1 && !blocks[i][j + 1][k].isSolid())
					getBlockFace(Direction::POSY, i, j, k, output);
				if (k != 0 && !blocks[i][j][k - 1].isSolid())
					getBlockFace(Direction::NEGZ, i, j, k, output);
				if (k != CHUNK_SIZE - 1 && !blocks[i][j][k + 1].isSolid())
					getBlockFace(Direction::POSZ, i, j, k, output);
			}
}


/* equivalent to overloaded version, but return vectors instead of extending output parameter */
std::vector<BlockVertex> Chunk::getInteriorMesh() {
	std::vector<BlockVertex> mesh;
	getInteriorMesh(mesh);
	return mesh;
}


/* returns the raw mesh of all block faces in the chunk, for TESTING purposes only */
std::vector<BlockVertex> Chunk::getMeshUnoptimized() {
	std::vector<BlockVertex> mesh;
	for (unsigned int i = 0; i < CHUNK_SIZE; i++) {
		for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
			for (unsigned int k = 0; k < CHUNK_SIZE; k++) {
				glm::vec3 offset = glm::vec3(0.5f, 0.5f, 0.5f) + glm::vec3(i, j, k) + (float) CHUNK_SIZE * chunkPosition;

				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
					
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
			
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
			
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
	
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 1.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 0.0f)));
				mesh.push_back(BlockVertex(offset + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0f, 1.0f)));
			}
		}
	}
	return mesh;
}


/* Generates the chunk blocks; eventually will use noise sampling,
   currently sets all blocks to be grass */
void Chunk::generate() {
	for (unsigned int i = 0; i < CHUNK_SIZE; i++)
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
			for (unsigned int k = 0; k < CHUNK_SIZE; k++) {
				//blocks[i][j][k].type = BlockType::GRASS;
				//blocks[i][j][k].variant = 0;
				blocks[i][j][k] = Block(BlockType::GRASS, 0);
			}
}


