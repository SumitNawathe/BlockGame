#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec2(const char* name, const glm::vec2& value) const;
	void setVec2(const char* name, float x, float y) const;
	void setVec3(const char* name, const glm::vec3& value) const;
	void setVec3(const char* name, float x, float y, float z) const;
	void setVec4(const char* name, const glm::vec4& value) const;
	void setVec4(const char* name, float x, float y, float z, float w) const;
	void setMat2(const char* name, const glm::mat2& value) const;
	void setMat3(const char* name, const glm::mat3& value) const;
	void setMat4(const char* name, const glm::mat4& value) const;
};

#endif


