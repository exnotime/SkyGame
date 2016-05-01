#pragma once
#include "Graphics.h"
#include "Timer.h"
class Game {
public:
	Game();
	~Game();
	void Init();
	void Update();
	void Draw();
private:
	Timer m_GameTime;
	GraphicsEngine m_Graphics;
};