#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

enum class Direction {
	POSX,
	NEGX,
	POSY,
	NEGY,
	POSZ,
	NEGZ
};

class BlockVertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	BlockVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) {
		this->position = position;
		this->normal = normal;
		this->texCoords = texCoords;
	}
	BlockVertex(float posX, float posY, float posZ, float normX, float normY, float normZ, float texU, float texV) {
		this->position = glm::vec3(posX, posY, posZ);
		this->normal = glm::vec3(normX, normY, normZ);
		this->texCoords = glm::vec2(texU, texV);
	}
};

typedef BlockVertex *BlockMesh;

enum class BlockType {
	AIR,
	GRASS
};

class Block {
public:
	BlockType type;
	int variant;

	Block() {
		this->type = BlockType::AIR;
		this->variant = 0;
	}
	Block(BlockType type, int variant) {
		this->type = type;
		this->variant = variant;
	}

	BlockType getType() { return type; }
	int getVariant() { return variant; }

	bool isSolid() {
		switch (type) {
			case BlockType::AIR:
				return false;
			case BlockType::GRASS:
				return true;
			default:
				return false;
		}
	}
};

#endif

