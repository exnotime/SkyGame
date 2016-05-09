#pragma once
#include <glm/glm.hpp>
struct VertexPosNormalTex {
	VertexPosNormalTex() {

	}
	VertexPosNormalTex( glm::vec3 p, glm::vec3 n, glm::vec2 uv ) {
		Position = p;
		Normal = n;
		TexCoord = uv;
	}
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};