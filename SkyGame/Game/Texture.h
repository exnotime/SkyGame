#pragma once
#include <string>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include "AssetManager.h"
class Texture {
public:
	Texture();
	~Texture();
	void Load(const std::string& filename);
	GLuint GetHandle() { return m_Handle; }
	glm::vec2 GetSize() { return glm::vec2(m_Width, m_Height); }
private:
	GLuint m_Handle;
	int m_Width;
	int m_Height;
	int m_Channels;
};