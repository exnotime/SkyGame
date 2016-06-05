#pragma once
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Vertex.h"
#include <vector>
enum VertexType {
	POS_NORMAL_TEX
};
class VertexBuffer {
  private:
	GLuint m_Handle;
	GLuint m_VertexArrayId;
  public:
	VertexBuffer ( void );
	~VertexBuffer ( void );
	bool Init ( VertexType VertexType, void* Data, unsigned int Size);
	void Apply();
	void Release();
	void Update(void* data, unsigned int size, unsigned int offset);
};
