#include "ShaderProgram.h"
#include "AssetManager.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

ShaderProgram::ShaderProgram ( void ) {
	m_LoadedShaders = false;
}

ShaderProgram::ShaderProgram ( std::vector<Shader*> Shaders ) {
	this->m_Shaders = Shaders;
	Init ( m_Shaders, true );
}

ShaderProgram::~ShaderProgram ( void ) {
	if ( m_LoadedShaders ) {
		for ( std::vector<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it ) {
			delete *it ;
		}
		m_Shaders.clear();
	}
	glDeleteProgram ( m_Handle );
}

bool ShaderProgram::Init ( std::vector<Shader*> Shaders, bool print ) {
	m_LoadedShaders = false;
	if ( m_Shaders.size() > 0 ) {
		m_Shaders.clear();
	}
	//save shaders
	for ( auto& it : Shaders ) {
		m_Shaders.push_back ( it );
	}
	m_Handle = glCreateProgram();
	if ( m_Handle == 0 ) {
		return false;
	}
	//attach shaders to program
	for ( int i = 0; i < static_cast<int> ( Shaders.size() ); i++ ) {
		glAttachShader ( m_Handle, Shaders[i]->GetHandle() );
	}
	//link shaders
	glLinkProgram ( m_Handle );
	//error checking
	GLint status;
	glGetProgramiv ( m_Handle, GL_LINK_STATUS, &status );
	if ( status == GL_FALSE ) {
		char log[1024];
		int len = 0;
		glGetProgramInfoLog ( m_Handle, sizeof ( log ), &len, log );
		LOGI( "Error linking shaders\n Error log:\n %s \n",log );
		return false;
	}
	if ( print ) {
		LOGI("linked shaderprogram\n");
	}
	Validate();
	return glIsProgram(m_Handle);
}

GLuint ShaderProgram::GetHandle() {
	return m_Handle;
}

void ShaderProgram::Apply() {
	// Lazy validation because of AMD driver behaviour.
	if ( !m_Validated ) {
		if ( !Validate() ) {
			GLint status;
			glGetProgramiv ( m_Handle, GL_VALIDATE_STATUS, &status );
			if ( status == GL_FALSE ) {
				char log[1024];
				int len = 0;
				glGetProgramInfoLog ( m_Handle, sizeof ( log ), &len, log );

				if (len > 0) {

				}
			}
		}
	}
	glUseProgram ( m_Handle );
}

bool ShaderProgram::Validate() {
	glValidateProgram ( m_Handle );
	GLint status;
	glGetProgramiv ( m_Handle, GL_VALIDATE_STATUS, &status );
	m_Validated = ( status == GL_TRUE );
	return m_Validated;
}

GLuint ShaderProgram::GetUniformLocationByName ( const std::string& name ) {
	return glGetUniformLocation ( m_Handle, name.c_str() );
}

void ShaderProgram::SetUniformFloat ( const std::string& name, const float value ) {
	glUniform1f (GetUniformLocationByName( name ), value );
}

void ShaderProgram::SetUniformInt( const std::string& name, const int value ) {
	glUniform1i (GetUniformLocationByName( name ), value );
}

void ShaderProgram::SetUniformUInt ( const std::string& name, const unsigned int value ) {
	glUniform1ui (GetUniformLocationByName( name ), value );
}

void ShaderProgram::SetUniformBool ( const std::string& name, const bool value ) {
	glUniform1i (GetUniformLocationByName( name ), value );
}

void ShaderProgram::SetUniformVec3 ( const std::string& name, const glm::vec3& value ) {
	glUniform3f (GetUniformLocationByName( name ), value.x, value.y, value.z );
}

void ShaderProgram::SetUniformVec2 ( const std::string& name, const glm::vec2& value ) {
	glUniform2f (GetUniformLocationByName( name ), value.x, value.y );
}

void ShaderProgram::SetUniformVec4 ( const std::string& name, const glm::vec4& value ) {
	glUniform4f (GetUniformLocationByName( name ), value.x, value.y, value.z, value.w );
}

void ShaderProgram::SetUniformMat4 ( const std::string& name, const glm::mat4x4& value ) {
	glUniformMatrix4fv (GetUniformLocationByName( name ), 1, GL_FALSE, &value[0][0] );
}

void ShaderProgram::SetUniformTextureHandle(const std::string& name, GLuint tex, int index) {
	glUniform1i(FetchUniform(name), index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, tex);
}

GLint ShaderProgram::FetchUniform ( const std::string& name ) {
	auto it = m_UniformMap.find ( name );
	// Uniform wasn't found in cache
	if ( it == m_UniformMap.end() ) {
		GLint loc = GetUniformLocationByName ( name );
		// Don't emplace in cache if uniform was invalid
		if ( loc != -1 ) {
			m_UniformMap[name] = loc;
		} else {
			//printf("Failed to fetch unifrom : %s\n",name.c_str());
			//assert(false); //makes it annoying to debug shaders
		}
		return loc;// Uniform was found in cache
	} else {
		return it->second;
	}
}