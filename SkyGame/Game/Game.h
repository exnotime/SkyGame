#pragma once
#include "Graphics.h"
#include "Timer.h"
#include "Camera.h"
#include "CollisionHandler.h"
#include "SpriteEngine.h"
#include "Level.h"
#include <vector>
#include "NextLevelMenu.h"
struct Player {
	Camera* PlayerCamera;
	glm::vec3 Velocity;
	bool Jumping;
};

struct GameState {
	glm::vec3 CameraPosition;
	glm::quat CameraRotation;
	int Level;
};

class Game {
public:
	Game();
	~Game();
	void Init(const glm::vec2& screenSize, GameState* gameState);
	void Update();
	void Draw();
	void LoadNextLevel();
	GameState GetGameState();
private:
	CollisionHandler m_CollisionHandler;
	std::vector<AABB> m_Boxes;
	std::vector<CubeModel> m_CubeModels;
	int m_LevelCounter;
	std::vector<std::string> m_LevelList;
	Level m_Level;
	Timer m_LevelTimer;
	Timer m_GameTime;
	GraphicsEngine m_Graphics;
	Player m_Player;
	bool m_Initialized = false;
	NextLevelMenu m_NextMenu;
	SpriteEngine m_SpriteEngine;
	glm::vec2 m_LookVector;
};