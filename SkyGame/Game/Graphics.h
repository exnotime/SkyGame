#pragma once
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Texture.h"
struct CubeModel {
	glm::vec3 Color;
	glm::vec3 Position;
};

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();
	void Init(const glm::vec2& screenSize);
	void Render(const Camera& cam, const std::vector<CubeModel>& cubeList);
	void GenerateCube();
	glm::vec2 GetScreenSize() { return m_ScreenSize; }
private:
	ShaderProgram	m_GeometryProgram;
	ShaderProgram	m_SkyProgram;
	VertexBuffer	m_VertexBuffer;
	glm::vec2		m_ScreenSize;
	Texture			m_Texture;
	Texture			m_NormalTexture;
	bool m_Initialized = false;
	int m_CubeVerticeCount = 0;
};