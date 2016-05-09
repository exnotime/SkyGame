#include "Game.h"
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <android/asset_manager.h>
#include "Input.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOOK_SPEED 0.5f
#define MOVE_SPEED 2.0f
Game::Game() {

}

Game::~Game() {

}

void Game::Init(const glm::vec2& screenSize) {
	m_GameTime.Tick();
	m_Graphics.Init(screenSize);
	m_Camera = new Camera();
	m_Camera->SetPosition(glm::vec3(0, 0, 5));
	m_CubeModels.push_back({ glm::vec3(1,0,0), glm::vec3(0) });
	m_CubeModels.push_back({ glm::vec3(0,1,0), glm::vec3(10,0,0) });
	m_CubeModels.push_back({ glm::vec3(0,0,1), glm::vec3(-10,0,0) });
	m_CubeModels.push_back({ glm::vec3(1,0,1), glm::vec3(0,10,0) });

	AABB box;
	for (auto& cube : m_CubeModels) {
		box.Min = cube.Position - glm::vec3(1);
		box.Max = cube.Position + glm::vec3(1);
		m_Boxes.push_back(box);
	}
	m_Initialized = true;
}

void Game::Update() {
	if (!m_Initialized)
		return;
	float deltaTime = m_GameTime.Tick();
	if (g_Input.GetTouchCount() == 1) {
		glm::vec2 lookVector = glm::vec2(g_Input.GetTouchDelta(0).x *-1, g_Input.GetTouchDelta(0).y) * deltaTime * LOOK_SPEED;
		lookVector = glm::clamp(lookVector, glm::vec2(-1), glm::vec2(1));
		m_Camera->YawWorld(lookVector.x);
		m_Camera->PitchRelative(lookVector.y);
	}
	else if (g_Input.GetTouchCount() == 2) {
		glm::vec3 velocity = m_Camera->GetForward() * MOVE_SPEED;
		//construct player AABB
		AABB playerBox;
		glm::vec3 halfPlayerSize = glm::vec3(0.5f, 1.6f, 0.2f) * 0.5f;
		playerBox.Min = m_Camera->GetPosition() - halfPlayerSize;
		playerBox.Max = m_Camera->GetPosition() + halfPlayerSize;
		velocity = m_CollisionHandler.HandleCollisions(playerBox, velocity, m_Boxes);
		LOGI("Velocity : x = %f y = %f z = %f", velocity.x, velocity.y, velocity.z);
		m_Camera->MoveWorld(velocity * deltaTime);
	}

	m_Camera->CalculateViewProjection();
}

void Game::Draw() {
	if (!m_Initialized)
		return;
	m_Graphics.Render(*m_Camera, m_CubeModels);
}
