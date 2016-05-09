#pragma once
#include "Graphics.h"
#include "Timer.h"
#include "Camera.h"
#include "CollisionHandler.h"
#include <vector>
class Game {
public:
	Game();
	~Game();
	void Init(const glm::vec2& screenSize);
	void Update();
	void Draw();
private:
	CollisionHandler m_CollisionHandler;

	std::vector<AABB> m_Boxes;
	std::vector<CubeModel> m_CubeModels;

	Timer m_GameTime;
	GraphicsEngine m_Graphics;
	Camera* m_Camera;
	bool m_Initialized = false;
};