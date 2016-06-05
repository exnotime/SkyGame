#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexBuffer.h"
struct Sprite {
	Texture Image;
	glm::vec4 ScreenRect;
	glm::vec4 SourceRect;
};

class SpriteEngine {
public:
	SpriteEngine();
	~SpriteEngine();
	void Init(glm::vec2 screenSize);
	void Render(std::vector<Sprite*> spriteList);
private:
	ShaderProgram m_SpriteShader;
	glm::vec2 m_InvScreenSize;
};