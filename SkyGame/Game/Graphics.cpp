#include "Graphics.h"
#include "AssetManager.h"
#include <glm/gtx/transform.hpp>
#define PAR_SHAPES_IMPLEMENTATION
#include <par_shapes.h>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
GraphicsEngine::GraphicsEngine() {

}

GraphicsEngine::~GraphicsEngine() {

}

void GraphicsEngine::Init(const glm::vec2& screenSize) {
	m_ScreenSize = screenSize;
	GenerateCube();
	//create Shaders
	Shader vertShader;
	Shader fragShader;
	Shader skyVertShader;
	Shader skyFragShader;
	std::vector<Shader*> shaders;
	AssetFile* file;
	if (g_AssetManager.LoadAssetFile("shaders/geometry.vert", &file)) {
		vertShader.CreateFromString(std::string(file->Data, file->Size), GL_VERTEX_SHADER, "shaders/geometry.vert", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&vertShader);
	}
	if (g_AssetManager.LoadAssetFile("shaders/geometry.frag", &file)) {
		fragShader.CreateFromString(std::string(file->Data, file->Size), GL_FRAGMENT_SHADER, "shaders/geometry.frag", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&fragShader);
	}
	m_GeometryProgram.Init(shaders, true);
	vertShader.Clear();
	fragShader.Clear();
	shaders.clear();
	//sky shader
	if (g_AssetManager.LoadAssetFile("shaders/sky.vert", &file)) {
		skyVertShader.CreateFromString(std::string(file->Data, file->Size), GL_VERTEX_SHADER, "shaders/sky.vert", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&skyVertShader);
	}
	if (g_AssetManager.LoadAssetFile("shaders/sky.frag", &file)) {
		skyFragShader.CreateFromString(std::string(file->Data, file->Size), GL_FRAGMENT_SHADER, "shaders/sky.frag", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&skyFragShader);
	}
	m_SkyProgram.Init(shaders, true);
	skyVertShader.Clear();
	skyFragShader.Clear();

	m_Texture.Load("textures/box_texture.png");
	m_NormalTexture.Load("textures/bump.png");
	m_Initialized = true;
}

void GraphicsEngine::Render(const Camera& cam, const std::vector<CubeModel>& cubeList) {
	if (!m_Initialized)
		return;
	//set up buffers
	m_VertexBuffer.Apply();
	//render sky
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	m_SkyProgram.Apply();
	m_SkyProgram.SetUniformMat4("g_ProjView", cam.GetViewProjection());
	m_SkyProgram.SetUniformMat4("g_World", glm::translate(cam.GetPosition()));
	glDrawArrays(GL_TRIANGLES, 0, m_CubeVerticeCount);
	glCullFace(GL_BACK);
	//apply shaders
	m_GeometryProgram.Apply();
	m_GeometryProgram.SetUniformMat4("g_ViewProj", cam.GetViewProjection());

	for (auto& cube : cubeList) {
		glm::mat4 world = glm::translate(cube.Position);
		m_GeometryProgram.SetUniformMat4("g_World", world);
		m_GeometryProgram.SetUniformVec3("g_Color", cube.Color);
		m_GeometryProgram.SetUniformVec3("g_CamPos", cam.GetPosition());
		m_GeometryProgram.SetUniformTextureHandle("g_Texture", m_Texture.GetHandle(), 0);
		m_GeometryProgram.SetUniformTextureHandle("g_NormalMap", m_NormalTexture.GetHandle(), 1);
		glDrawArrays(GL_TRIANGLES, 0, m_CubeVerticeCount);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void GraphicsEngine::GenerateCube() {

	std::vector<VertexPosNormalTex> vertices;
	VertexPosNormalTex vertex;
	GLuint texIndex = 0;

	par_shapes_mesh_s* mesh;
	mesh = par_shapes_create_cube();
	//unweld to create per vertex normals
	par_shapes_unweld(mesh, true);
	par_shapes_compute_normals(mesh);
	//scale and translate into unit cube
	par_shapes_scale(mesh, 2, 2, 2);
	par_shapes_translate(mesh, -1, -1, -1);

	for (int i = 0; i < mesh->npoints * 3; i += 3) {
		vertex.Position = glm::vec3(mesh->points[i], mesh->points[i + 1], mesh->points[i + 2]);
		glm::vec3 normal = glm::vec3(mesh->normals[i], mesh->normals[i + 1], mesh->normals[i + 2]);
		vertex.Normal = glm::normalize(normal);
		if (vertex.Normal == glm::vec3(0, 0, -1) || vertex.Normal == glm::vec3(0,0,1)) {
			vertex.TexCoord.x = vertex.Position.x * 0.5f + 0.5f;
			vertex.TexCoord.y = vertex.Position.y * 0.5f + 0.5f;
		}
		else if (vertex.Normal == glm::vec3(0, -1, 0) || vertex.Normal == glm::vec3(0, 1, 0)) {
			vertex.TexCoord.x = vertex.Position.x * 0.5f + 0.5f;
			vertex.TexCoord.y = vertex.Position.z * 0.5f + 0.5f;
		}
		else if (vertex.Normal == glm::vec3(-1, 0, 0) || vertex.Normal == glm::vec3(1, 0, 0)) {
			vertex.TexCoord.x = vertex.Position.z * 0.5f + 0.5f;
			vertex.TexCoord.y = vertex.Position.y * 0.5f + 0.5f;
		}

		vertices.push_back(vertex);
	}
	//HACK: this vertexbuffer is reused for sprites thats why we add 40 extra vertices
	m_VertexBuffer.Init(VertexType::POS_NORMAL_TEX, vertices.data(), (vertices.size() + 40) * sizeof(VertexPosNormalTex) );
	m_CubeVerticeCount = vertices.size();
}