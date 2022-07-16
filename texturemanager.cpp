#include "texturemanager.h"
typedef typename TextureManager::TextureResult TextureResult;
typedef typename TextureManager::TextureQuery TextureQuery;

#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

TextureResult TextureManager::getUVCoords(BlockType type, Direction dir) {
	return data[TextureQuery(type, dir)];
}


/* private constructor for TextureManager; loads the texture atlas texture, calls registerUVCoords() */
TextureManager::TextureManager() {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(TEXTURE_ATLAS_PATH, &width, &height, &numChannels, 0);
	if (!data) {
		std::cout << "Failed to load texture atlas: " << TEXTURE_ATLAS_PATH << std::endl;
		exit(-1);
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_image_free(data);

	std::cout << "TextureManager: width = " << width <<
		", height = " << height << ", numChannels = " << numChannels << std::endl;

	registerUVCoords();
}


/*
* calculates the UV coordinates of a block texture given the texture's position
* @returns TextureResult in order: top left, top right, bottom left, bottom right
*/
TextureResult TextureManager::calculateUVCoords(unsigned int x, unsigned int y) {
	return TextureResult{
		TEXTURE_SIZE_F * glm::vec2(x / (float) width, 1.0f - y / (float) height),
		TEXTURE_SIZE_F * glm::vec2((x + 1) / (float) width, 1.0f - y / (float) height),
		TEXTURE_SIZE_F * glm::vec2(x / (float) width, 1.0f - (y + 1) / (float) height),
		TEXTURE_SIZE_F * glm::vec2((x + 1) / (float) width, 1.0f - (y + 1) / (float) height)
	};
}


/* initializes the data map, hardcoded */
void TextureManager::registerUVCoords() {
	// grass
	data[TextureQuery(BlockType::GRASS, Direction::POSY)] = calculateUVCoords(0, 0);
	data[TextureQuery(BlockType::GRASS, Direction::POSX)] = calculateUVCoords(1, 0);
	data[TextureQuery(BlockType::GRASS, Direction::NEGX)] = calculateUVCoords(1, 0);
	data[TextureQuery(BlockType::GRASS, Direction::POSZ)] = calculateUVCoords(1, 0);
	data[TextureQuery(BlockType::GRASS, Direction::NEGZ)] = calculateUVCoords(1, 0);
	data[TextureQuery(BlockType::GRASS, Direction::NEGY)] = calculateUVCoords(2, 0);

	//TextureResult res = calculateUVCoords(0, 0);
}

