#pragma once
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "Camera.h"
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
	VertexBuffer	m_VertexBuffer;
	GLuint			m_IndexBuffer;
	glm::vec2		m_ScreenSize;
	bool m_Initialized = false;
	int m_CubeIndiceCount = 0;
};