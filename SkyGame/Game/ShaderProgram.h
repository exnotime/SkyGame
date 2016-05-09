#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>
#include <map>
#include <sstream>

class ShaderProgram {
  public:
	ShaderProgram ( void );
	ShaderProgram ( std::vector<Shader*> Shaders );
	~ShaderProgram ( void );
	bool Init ( std::vector<Shader*> Shaders, bool print );
	GLuint GetHandle();
	void Apply();
	bool Validate();
	GLuint GetUniformLocationByName ( const std::string& name );

	void SetUniformFloat ( const std::string& name, const float value );
	void SetUniformInt ( const std::string& name, const int value );
	void SetUniformUInt ( const std::string& name, const unsigned int value );
	void SetUniformBool ( const std::string& name, const bool value );
	void SetUniformVec3 ( const std::string& name, const glm::vec3& value );
	void SetUniformMat4 ( const std::string& name, const glm::mat4x4& value );
	void SetUniformVec2 ( const std::string& name, const glm::vec2& value );
	void SetUniformVec4 ( const std::string& name, const glm::vec4& value );
	void SetUniformTextureHandle(const std::string& name, GLuint tex, int index);
	GLint FetchUniform ( const std::string& name );
  private:
	GLuint							m_Handle;
	std::vector<Shader*>			m_Shaders;
	bool							m_LoadedShaders;
	std::map<std::string, GLint>    m_UniformMap;
	bool							m_Validated = false;
};
#endif

