#include "SpriteEngine.h"
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
SpriteEngine::SpriteEngine() {

}
SpriteEngine::~SpriteEngine() {

}

void SpriteEngine::Init(glm::vec2 screenSize) {
	m_InvScreenSize = glm::vec2(1.0f) / screenSize;
	//load shader
	Shader vertShader;
	Shader fragShader;
	std::vector<Shader*> shaders;
	AssetFile* file;
	if (g_AssetManager.LoadAssetFile("shaders/sprite.vert", &file)) {
		vertShader.CreateFromString(std::string(file->Data, file->Size), GL_VERTEX_SHADER, "shaders/sprite.vert", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&vertShader);
	}
	if (g_AssetManager.LoadAssetFile("shaders/sprite.frag", &file)) {
		fragShader.CreateFromString(std::string(file->Data, file->Size), GL_FRAGMENT_SHADER, "shaders/sprite.frag", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&fragShader);
	}
	m_SpriteShader.Init(shaders, true);
	vertShader.Clear();
	fragShader.Clear();
}

void SpriteEngine::Render(std::vector<Sprite*> spriteList) {
	//build vbo
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	std::vector<VertexPosNormalTex> vertices;
	VertexPosNormalTex vertex;
	for (auto& sprite : spriteList) {
		glm::vec2 size = glm::vec2(sprite->ScreenRect.z, sprite->ScreenRect.w) * m_InvScreenSize * 2.0f;
		glm::vec2 pos = glm::vec2(sprite->ScreenRect.x, sprite->ScreenRect.y) * m_InvScreenSize * 2.0f - 1.0f;
		glm::vec4 texCoord;
		texCoord.x = (sprite->SourceRect.x == -1) ? 0 : sprite->SourceRect.x / sprite->Image.GetSize().x;
		texCoord.y = (sprite->SourceRect.y == -1) ? 0 : sprite->SourceRect.y / sprite->Image.GetSize().y;
		texCoord.z = (sprite->SourceRect.z == -1) ? 1 :(sprite->SourceRect.x + sprite->SourceRect.z) / sprite->Image.GetSize().x;
		texCoord.w = (sprite->SourceRect.w == -1) ? 1 :(sprite->SourceRect.y + sprite->SourceRect.w) / sprite->Image.GetSize().y;
		//lower left
		vertex.Position = glm::vec3(pos, 0);
		vertex.TexCoord = glm::vec2(texCoord.x, texCoord.w);
		vertices.push_back(vertex);
		//lower right
		vertex.Position = glm::vec3(pos, 0) + glm::vec3(size.x, 0, 0);
		vertex.TexCoord = glm::vec2(texCoord.z, texCoord.w);
		vertices.push_back(vertex);
		//upper left
		vertex.Position = glm::vec3(pos, 0) + glm::vec3(0, size.y, 0);
		vertex.TexCoord = glm::vec2(texCoord.x, texCoord.y);
		vertices.push_back(vertex);
		//upper right
		vertex.Position = glm::vec3(pos, 0) + glm::vec3(size.x, size.y, 0);
		vertex.TexCoord = glm::vec2(texCoord.z, texCoord.y);
		vertices.push_back(vertex);
	}
	glBufferSubData(GL_ARRAY_BUFFER, 36 * sizeof(VertexPosNormalTex), vertices.size() * sizeof(VertexPosNormalTex), vertices.data());
	int vertexCounter = 36;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_SpriteShader.Apply();
	for (auto& sprite : spriteList) {
		m_SpriteShader.SetUniformTextureHandle("g_Image", sprite->Image.GetHandle(), 0);
		glDrawArrays(GL_TRIANGLE_STRIP, vertexCounter, 4);
		vertexCounter += 4;
	}
	glDisable(GL_BLEND);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}