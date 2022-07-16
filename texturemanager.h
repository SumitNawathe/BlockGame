#pragma once

#include <vector>
#include <map>
#include <utility>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "block.h"

constexpr const char* TEXTURE_ATLAS_PATH = "C:/Users/Sumit Nawathe/source/repos/BlockGame/images/textureatlas1.png";
constexpr unsigned int TEXTURE_SIZE = 16;
constexpr float TEXTURE_SIZE_F = (float) TEXTURE_SIZE;

class TextureManager {
public:
	typedef typename std::pair<BlockType, Direction> TextureQuery;
	typedef typename std::vector<glm::vec2> TextureResult;

	static TextureManager& getInstance() {
		static TextureManager instance;
		return instance;
	}

	TextureResult getUVCoords(BlockType type, Direction dir);
	unsigned int getTextureID() { return textureID; }
	void useTexture() {
		glActiveTexture(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	
private:
	std::map<TextureQuery, TextureResult> data;
	unsigned int textureID;
	int width;
	int height;
	int numChannels;

	TextureManager();
	void registerUVCoords();
	TextureResult calculateUVCoords(unsigned int x, unsigned int y);

	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;
};

