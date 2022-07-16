#include "chunk.h"
typedef typename Chunk::ChunkBlockMesh ChunkBlockMesh;

#include <vector>
#include <iostream>
#include "texturemanager.h"
#include "NoiseGenerator.h"

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
void Chunk::getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k, ChunkBlockMesh& output) {
	glm::vec3 offset = glm::vec3(0.5f, 0.5f, 0.5f) + glm::vec3((float) i, (float) j, (float) k) + (float) CHUNK_SIZE * chunkPosition;

	if (i >= CHUNK_SIZE || j >= CHUNK_SIZE || k >= CHUNK_SIZE)
		return;

	TextureManager::TextureResult uvCoords = TextureManager::getInstance().getUVCoords(blocks[i][j][k].getType(), dir);

	switch (dir) {
		case Direction::NEGX:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			break;
		case Direction::POSX:
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			output.push_back(BlockVertex(offset + glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			break;
		case Direction::NEGY:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			break;
		case Direction::POSY:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, 0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, 0.5f,  0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, 0.5f,  0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			break;
		case Direction::NEGZ:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			break;
		case Direction::POSZ:
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f, -0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[3])); // bottom right
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3( 0.5f,  0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[1])); // top right
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[0])); // top left
			output.push_back(BlockVertex(offset + glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0, 0.0, 0.0), uvCoords[2])); // bottom left
			break;
	}
}


/* equivalent to overloaded version, but returns vector instead of extending output parameter */
ChunkBlockMesh Chunk::getBlockFace(Direction dir, unsigned int i, unsigned int j, unsigned int k) {
	ChunkBlockMesh mesh;
	getBlockFace(dir, i, j, k, mesh);
	return mesh;
}


/* recalculates edge faces for the chunk edge in the given direction,
* used for when new chunk on edge of render distance is generated */
void Chunk::regenerateEdge(Direction dir, Chunk* neighbor, ChunkBlockMesh& output) {
	switch (dir) {
		case Direction::NEGX:
			for (unsigned int j = 0; j < CHUNK_SIZE; j++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					if (blocks[0][j][k].type == BlockType::AIR) continue;
					else if (!neighbor->blocks[CHUNK_SIZE - 1][j][k].isSolid())
						getBlockFace(Direction::NEGX, 0, j, k, output);
			break;
		case Direction::POSX:
			for (unsigned int j = 0; j < CHUNK_SIZE; j++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					if (blocks[CHUNK_SIZE - 1][j][k].type == BlockType::AIR) continue;
					else if (!neighbor->blocks[0][j][k].isSolid())
						getBlockFace(Direction::POSX, CHUNK_SIZE - 1, j, k, output);
			break;
		case Direction::NEGY:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					if (blocks[i][0][k].type == BlockType::AIR) continue;
					else if (!neighbor->blocks[i][CHUNK_SIZE - 1][k].isSolid())
						getBlockFace(Direction::NEGY, i, 0, k, output);
			break;
		case Direction::POSY:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int k = 0; k < CHUNK_SIZE; k++)
					if (blocks[i][CHUNK_SIZE - 1][k].type == BlockType::AIR) continue;
					else if (!neighbor->blocks[i][0][k].isSolid())
						getBlockFace(Direction::NEGY, i, CHUNK_SIZE - 1, k, output);
			break;
		case Direction::NEGZ:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int j = 0; j < CHUNK_SIZE; j++)
					if (blocks[i][j][0].type == BlockType::AIR) continue;
					else if (!neighbor->blocks[i][j][CHUNK_SIZE - 1].isSolid())
						getBlockFace(Direction::NEGZ, i, j, 0, output);
			break;
		case Direction::POSZ:
			for (unsigned int i = 0; i < CHUNK_SIZE; i++)
				for (unsigned int j = 0; j < CHUNK_SIZE; j++)
					if (blocks[i][j][CHUNK_SIZE - 1].type == BlockType::AIR) continue;
					else if (!neighbor->blocks[i][j][0].isSolid())
						getBlockFace(Direction::NEGZ, i, j, CHUNK_SIZE - 1, output);
			break;
		default:
			break;
	}
}


void Chunk::getSingleBlockMesh(unsigned int i, unsigned int j, unsigned int k,
		std::map<Direction, Chunk*> neighbors, ChunkBlockMesh& output, bool forceEdge) {
	// check if block should be drawn
	if (blocks[i][j][k].type == BlockType::AIR)
		return;

	/* for each block, only draw if either
	* (1) not on edge, and not next to solid block
	* (2) on edge, have neighbor chunk, and neighbor block not solid */
	if ((i != 0 && !blocks[i - 1][j][k].isSolid()) ||
			(i == 0 && forceEdge) ||
			(i == 0 && neighbors[Direction::NEGX] != nullptr &&
			!neighbors[Direction::NEGX]->blocks[CHUNK_SIZE - 1][j][k].isSolid()))
		getBlockFace(Direction::NEGX, i, j, k, output);
	if ((i != CHUNK_SIZE - 1 && !blocks[i + 1][j][k].isSolid()) ||
			(i == CHUNK_SIZE - 1 && forceEdge) || 
			(i == CHUNK_SIZE - 1 && neighbors[Direction::POSX] != nullptr &&
			!neighbors[Direction::POSX]->blocks[0][j][k].isSolid()))
		getBlockFace(Direction::POSX, i, j, k, output);
	if ((j != 0 && !blocks[i][j - 1][k].isSolid()) ||
			(j == 0 && forceEdge) ||
			(j == 0 && neighbors[Direction::NEGY] != nullptr &&
			!neighbors[Direction::NEGY]->blocks[i][CHUNK_SIZE - 1][k].isSolid()))
		getBlockFace(Direction::NEGY, i, j, k, output);
	if ((j != CHUNK_SIZE - 1 && !blocks[i][j + 1][k].isSolid()) ||
			(j == CHUNK_SIZE - 1 && forceEdge) ||
			(j == CHUNK_SIZE - 1 && neighbors[Direction::POSY] != nullptr &&
			!neighbors[Direction::POSY]->blocks[i][0][k].isSolid()))
		getBlockFace(Direction::POSY, i, j, k, output);
	if ((k != 0 && !blocks[i][j][k - 1].isSolid()) ||
			(k == 0 && forceEdge) ||
			(k == 0 && neighbors[Direction::NEGZ] != nullptr &&
						!neighbors[Direction::NEGZ]->blocks[i][j][CHUNK_SIZE - 1].isSolid()))
		getBlockFace(Direction::NEGZ, i, j, k, output);
	if ((k != CHUNK_SIZE - 1 && !blocks[i][j][k + 1].isSolid()) ||
			(k == CHUNK_SIZE - 1 && forceEdge) ||
			(k == CHUNK_SIZE - 1 && neighbors[Direction::POSZ] != nullptr &&
			!neighbors[Direction::POSZ]->blocks[i][j][0].isSolid()))
		getBlockFace(Direction::POSZ, i, j, k, output);
}


/*
* calculates a mesh containing all block faces belonging to this chunk
* @param neighbors: pointers to neighboring chunks, used for determining whether edge faces are shown
* @param output: vector to which faces are added
*/
void Chunk::getBlockMesh(std::map<Direction, Chunk*> neighbors, ChunkBlockMesh& output, bool forceEdge) {
	for (unsigned int i = 0; i < CHUNK_SIZE; i++)
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
			for (unsigned int k = 0; k < CHUNK_SIZE; k++) {
				//getSingleBlockMesh(i, j, k, neighbors, output, forceEdge);

				// check if block should be drawn
				if (blocks[i][j][k].type == BlockType::AIR)
					continue;

				/* for each block, only draw if either
				* (1) not on edge, and not next to solid block
				* (2) on edge, have neighbor chunk, and neighbor block not solid */
				if ((i != 0 && !blocks[i - 1][j][k].isSolid()) ||
						(i == 0 && forceEdge) ||
						(i == 0 && neighbors[Direction::NEGX] != nullptr &&
						!neighbors[Direction::NEGX]->blocks[CHUNK_SIZE - 1][j][k].isSolid()))
					getBlockFace(Direction::NEGX, i, j, k, output);
				if ((i != CHUNK_SIZE - 1 && !blocks[i + 1][j][k].isSolid()) ||
						(i == CHUNK_SIZE - 1 && forceEdge) || 
						(i == CHUNK_SIZE - 1 && neighbors[Direction::POSX] != nullptr &&
						!neighbors[Direction::POSX]->blocks[0][j][k].isSolid()))
					getBlockFace(Direction::POSX, i, j, k, output);
				if ((j != 0 && !blocks[i][j - 1][k].isSolid()) ||
						(j == 0 && forceEdge) ||
						(j == 0 && neighbors[Direction::NEGY] != nullptr &&
						!neighbors[Direction::NEGY]->blocks[i][CHUNK_SIZE - 1][k].isSolid()))
					getBlockFace(Direction::NEGY, i, j, k, output);
				if ((j != CHUNK_SIZE - 1 && !blocks[i][j + 1][k].isSolid()) ||
						(j == CHUNK_SIZE - 1 && forceEdge) ||
						(j == CHUNK_SIZE - 1 && neighbors[Direction::POSY] != nullptr &&
						!neighbors[Direction::POSY]->blocks[i][0][k].isSolid()))
					getBlockFace(Direction::POSY, i, j, k, output);
				if ((k != 0 && !blocks[i][j][k - 1].isSolid()) ||
						(k == 0 && forceEdge) ||
						(k == 0 && neighbors[Direction::NEGZ] != nullptr &&
						!neighbors[Direction::NEGZ]->blocks[i][j][CHUNK_SIZE - 1].isSolid()))
					getBlockFace(Direction::NEGZ, i, j, k, output);
				if ((k != CHUNK_SIZE - 1 && !blocks[i][j][k + 1].isSolid()) ||
						(k == CHUNK_SIZE - 1 && forceEdge) ||
						(k == CHUNK_SIZE - 1 && neighbors[Direction::POSZ] != nullptr &&
						!neighbors[Direction::POSZ]->blocks[i][j][0].isSolid()))
					getBlockFace(Direction::POSZ, i, j, k, output);
			}
}


/* equivalent to overloaded version, but return vectors instead of extending output parameter */
ChunkBlockMesh Chunk::getBlockMesh(std::map<Direction, Chunk*> neighbors, bool forceEdge) {
	ChunkBlockMesh output;
	getBlockMesh(neighbors, output, forceEdge);
	return output;
}


/* returns the raw mesh of all block faces in the chunk, for TESTING purposes only */
ChunkBlockMesh Chunk::getMeshUnoptimized() {
	ChunkBlockMesh mesh;
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
		for (unsigned int k = 0; k < CHUNK_SIZE; k++) {
			int height = (int)(128.0f * NoiseGenerator::getInstance().generate2D(i + CHUNK_SIZE * chunkPosition.x, k + CHUNK_SIZE * chunkPosition.z));
			for (unsigned int j = 0; j < CHUNK_SIZE; j++) {
				if ((int) j + CHUNK_SIZE * chunkPosition.y > height)
					blocks[i][j][k] = Block(BlockType::AIR, 0);
				else
					blocks[i][j][k] = Block(BlockType::GRASS, 0);
			}
		}
}


