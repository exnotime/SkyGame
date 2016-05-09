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
	glCreateProgram();
	Shader geometryVertShader;
	Shader geometryFragShader;
	std::vector<Shader*> shaders;
	AssetFile* file;
	if (g_AssetManager.LoadAssetFile("shaders/geometry.vert", &file)) {
		geometryVertShader.CreateFromString(std::string(file->Data, file->Size), GL_VERTEX_SHADER, "shader/geometry.vert", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&geometryVertShader);
	}
	if (g_AssetManager.LoadAssetFile("shaders/geometry.frag", &file)) {
		geometryFragShader.CreateFromString(std::string(file->Data, file->Size), GL_FRAGMENT_SHADER, "shader/geometry.frag", true);
		g_AssetManager.FreeAssetFile(file);
		shaders.push_back(&geometryFragShader);
	}
	m_GeometryProgram.Init(shaders, true);
	//once program is linked there is no need for the shader code to reside in memory anymore
	geometryVertShader.Clear();
	geometryFragShader.Clear();
	int i = 0;
	m_Initialized = true;
}

void GraphicsEngine::Render(const Camera& cam, const std::vector<CubeModel>& cubeList) {
	if (!m_Initialized)
		return;
	//clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_ScreenSize.x, m_ScreenSize.y);
	//set up buffers
	m_VertexBuffer.Apply();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	//apply shaders
	m_GeometryProgram.Apply();
	m_GeometryProgram.SetUniformMat4("g_ViewProj", cam.GetViewProjection());

	for (auto& cube : cubeList) {
		glm::mat4 world = glm::translate(cube.Position);
		m_GeometryProgram.SetUniformMat4("g_World", world);
		m_GeometryProgram.SetUniformVec3("g_Color", cube.Color);
		m_GeometryProgram.SetUniformVec3("g_CamPos", cam.GetPosition());
		glDrawElements(GL_TRIANGLES, m_CubeIndiceCount, GL_UNSIGNED_SHORT, nullptr);
	}

	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		LOGI("openGL error %i\n", error);
		error = glGetError();
	}
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
		glm::vec3 c1 = glm::cross(normal, glm::vec3(0.0, 0.0, 1.0));
		glm::vec3 c2 = glm::cross(normal, glm::vec3(0.0, 1.0, 0.0));
		glm::vec3 tangent;
		if (glm::length(c1) > glm::length(c2)) {
			tangent = c1;
		}
		else {
			tangent = c2;
		}
		vertex.Normal = glm::normalize(normal);
		//most shapes dont have texture coordinates so just create tex coords as if it was a sphere and hope for the best :)
		if (!mesh->tcoords) {
			glm::vec2 uv;
			uv.x = glm::dot(glm::vec3(vertex.Normal), glm::vec3(1, 0, 0)) * 0.5 + 0.5;
			uv.y = glm::dot(glm::vec3(vertex.Normal), glm::vec3(0, 1, 0)) * 0.5 + 0.5;
			vertex.TexCoord = glm::vec2(uv);
		}
		else {
			vertex.TexCoord = glm::vec2(mesh->tcoords[texIndex], mesh->tcoords[texIndex + 1]);
			texIndex += 2;
		}
		vertices.push_back(vertex);
	}

	std::vector<unsigned short> indices;
	for (int i = 0; i < mesh->ntriangles * 3; i += 3) {
		indices.push_back(mesh->triangles[i]);
		indices.push_back(mesh->triangles[i + 1]);
		indices.push_back(mesh->triangles[i + 2]);
	}
	m_VertexBuffer.Init(VertexType::POS_NORMAL_TEX, vertices.data(), vertices.size() * sizeof(VertexPosNormalTex));
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
	m_CubeIndiceCount = indices.size();
}