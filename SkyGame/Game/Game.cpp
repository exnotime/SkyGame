#include "Game.h"
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <android/asset_manager.h>
#include "Input.h"
#include "Level.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#define LOOK_SPEED 0.3f
#define LOOK_INERTIA 0.5f;
#define MOVE_SPEED 2.0f
#define G 9.8f
#define PLAYER_MASS 50.0f //50 kg
#define JUMP_FORCE 580.0f //in newtons
Game::Game() {

}

Game::~Game() {
	if (m_Player.PlayerCamera) delete m_Player.PlayerCamera;
}

void Game::Init(const glm::vec2& screenSize, GameState* gameState) {
	m_GameTime.Tick();
	m_Graphics.Init(screenSize);
	m_Player.PlayerCamera = new Camera();
	m_LevelList.push_back("levels/level1.txt");
	m_LevelList.push_back("levels/level2.txt");
	m_LevelList.push_back("levels/level3.txt");
	m_LevelList.push_back("levels/level4.txt");
	m_LevelList.push_back("levels/level5.txt");
	m_LevelList.push_back("levels/level6.txt");
	m_LevelList.push_back("levels/level7.txt");
	m_LevelList.push_back("levels/level8.txt");
	m_LevelList.push_back("levels/level9.txt");
	m_LevelList.push_back("levels/level10.txt");

	if (gameState) {
		m_LevelCounter = gameState->Level;
		m_Player.PlayerCamera->SetPosition(gameState->CameraPosition + glm::vec3(0,0.5f,0));
		m_Player.PlayerCamera->SetOrientation(gameState->CameraRotation);
		LOGI("Loaded game state");
	}
	else {
		m_LevelCounter = 0;
	}
	LoadNextLevel();
	m_Player.Velocity = glm::vec3(0);
	m_LookVector = glm::vec2(0);
	m_Player.Jumping = true;
	m_SpriteEngine.Init(screenSize);
	glViewport(0, 0, screenSize.x, screenSize.y);
	m_NextMenu.Init(screenSize);

	m_Initialized = true;
}

void Game::Update() {
	if (!m_Initialized)
		return;
	const float deltaTime = 1.0f / 60.0f; //m_GameTime.Tick(); //to keep the physics consistent just asume we are always 60 fps
	glm::vec3 Acceleration = glm::vec3(0);
	//construct player AABB
	AABB playerBox;
	const glm::vec3 halfPlayerSize = glm::vec3(0.5f, 1.6f, 0.2f) * 0.5f;
	playerBox.Min = m_Player.PlayerCamera->GetPosition() - halfPlayerSize;
	playerBox.Max = m_Player.PlayerCamera->GetPosition() + halfPlayerSize;
	if (!m_NextMenu.Showing()) {
		if (g_Input.GetTouchCount() == 1) {
			glm::vec2 lookVector = glm::vec2(g_Input.GetTouchDelta(0).x * -1, g_Input.GetTouchDelta(0).y) * deltaTime * LOOK_SPEED;
			lookVector = glm::clamp(lookVector, glm::vec2(-1), glm::vec2(1));
			m_LookVector += lookVector;
		}
		else if (g_Input.GetTouchCount() == 2) {
			if (!m_Player.Jumping) {
				glm::vec3 direction = glm::normalize(glm::vec3(m_Player.PlayerCamera->GetForward().x, 0.6f, m_Player.PlayerCamera->GetForward().z));
				Acceleration += direction * (JUMP_FORCE / PLAYER_MASS); //add a bit of upwards momentum to the jump, a = f/m
				m_Player.Jumping = true;
				playerBox.Min.y += 0.5f; //move out of box we are standing on
			}
		}
	}
	//look acceleration
	m_LookVector *= LOOK_INERTIA;
	if ((m_LookVector.x < 0.001f && m_LookVector.x > 0) || (m_LookVector.x > -0.001f && m_LookVector.x < 0))
		m_LookVector.x = 0;
	if ((m_LookVector.y < 0.001f && m_LookVector.y > 0) || (m_LookVector.y > -0.001f && m_LookVector.y < 0))
		m_LookVector.y = 0;
	m_Player.PlayerCamera->YawWorld(m_LookVector.x);
	m_Player.PlayerCamera->PitchRelative(m_LookVector.y);

	Acceleration += G * glm::vec3(0, -1, 0) * 0.02f;
	//add gravity acceleration
	m_Player.Velocity = (m_Player.Velocity) + (Acceleration * deltaTime);
	m_Player.Velocity = m_CollisionHandler.HandleCollisions(playerBox, m_Player.Velocity, m_Boxes, m_Player.Jumping);
	m_Player.PlayerCamera->MoveWorld(m_Player.Velocity);
	//check of we hit the last box and have won the level
	playerBox.Min.y -= 0.2f;
	if (m_CollisionHandler.AABBvsAABB(playerBox, m_Boxes.at(m_Boxes.size() - 1)) && !m_NextMenu.Showing()) {
		m_NextMenu.ToggleShow();
	}
	if (m_Player.PlayerCamera->GetPosition().y < -20.0f) {
		//return the player to the start
		m_Player.PlayerCamera->SetPosition(m_Level.GetPoints()[0] + glm::vec3(0,2,0));
		m_Player.Velocity = glm::vec3(0);
	}
	m_NextMenu.Update();
	if (m_NextMenu.ShouldLoadNextLevel()) {
		LoadNextLevel();
		m_NextMenu.ToggleShow();
	}
	m_Player.PlayerCamera->CalculateViewProjection();
}

void Game::Draw() {
	if (!m_Initialized)
		return;
	//clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Graphics.Render(*m_Player.PlayerCamera, m_CubeModels);
	if (m_NextMenu.Showing()) {
		m_SpriteEngine.Render(m_NextMenu.GetSprites());
	}
}

void Game::LoadNextLevel() {
	m_CubeModels.clear();
	m_Boxes.clear();
	m_Level.Load(m_LevelList[m_LevelCounter]);

	for (auto& point : m_Level.GetPoints()) {
		m_CubeModels.push_back({ glm::vec3(0.5f,0.5f,0.5f), point });
	}
	m_CubeModels.at(m_CubeModels.size() - 1).Color = glm::vec3(1, 1, 0);//make goal yellow

	AABB box;
	for (auto& cube : m_CubeModels) {
		box.Min = cube.Position - glm::vec3(1);
		box.Max = cube.Position + glm::vec3(1);
		m_Boxes.push_back(box);
	}

	m_Player.PlayerCamera->SetPosition(m_Level.GetPoints()[0] + glm::vec3(0, 2, 0));
	m_Player.Velocity = glm::vec3(0);
	m_Player.PlayerCamera->SetOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

	m_LevelCounter = (m_LevelCounter + 1) % m_LevelList.size();//loop levels so we dont have a crash when we reach the end
}

GameState Game::GetGameState() {
	GameState gs;
	gs.CameraPosition = m_Player.PlayerCamera->GetPosition();
	gs.CameraRotation = m_Player.PlayerCamera->GetOrientation();
	gs.Level = m_LevelCounter;
	return gs;
}