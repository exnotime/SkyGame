#pragma once
#include <GLES3/gl3.h>
#include <string>
class Shader {
  private:
	GLuint m_Handle;
	std::string m_Path;
	GLenum m_Type;
  public:
	Shader();
	~Shader();
	bool CreateFromString(const std::string& ShaderCode, GLenum ShaderType, const std::string& filename, bool print );
	GLuint  GetHandle();
	void Clear();
};