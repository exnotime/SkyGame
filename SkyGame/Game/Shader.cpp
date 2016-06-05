#include "Shader.h"
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
Shader::Shader() {
}

Shader::~Shader() {
	glDeleteShader ( m_Handle );
}

bool Shader::CreateFromString(const std::string& ShaderCode, GLenum ShaderType, const std::string& filename, bool print ) {
	//create shader object
	m_Handle = glCreateShader ( ShaderType );
	if ( m_Handle == 0 ) {
		if ( print ) {
		}
	}
	const char* text = ShaderCode.c_str();
	glShaderSource ( m_Handle, 1, &text, nullptr );

	glCompileShader ( m_Handle );
	int result = 0;
	glGetShaderiv ( m_Handle, GL_COMPILE_STATUS, &result );
	if ( result == GL_FALSE ) {
		if ( print ) {
			LOGI("Error compiling shader %s\n", filename.c_str());
		}
		int length = 0;
		glGetShaderiv ( m_Handle, GL_INFO_LOG_LENGTH, &length );
		if ( length > 0 ) {
			// create a log of error messages
			char* errorLog = new char[length];
			int written = 0;
			glGetShaderInfoLog ( m_Handle, length, &written, errorLog );
			if ( print ) {
				LOGI("Error log:\n%s\n", errorLog );

			}
			delete[] errorLog;
		}
		return false;
	}
	LOGI("Successfully compiled shader %s\n", filename.c_str());
	return true;
}

GLuint Shader::GetHandle() {
	return m_Handle;
}

void Shader::Clear() {
	//when a shader has been linked to a program  there is no longer any need to save the shader(unless you want to reuse the shader)
	//so we clear shaders after we are done with them to free up memory.
	glDeleteShader ( m_Handle );
}
