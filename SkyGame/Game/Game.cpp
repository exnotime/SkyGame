#include "Game.h"
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

#include <android/asset_manager.h>
Game::Game() {

}

Game::~Game() {

}

void Game::Init() {
	m_GameTime.Tick();
	m_Graphics.Init();
	//TODO: Start looking into loading assets with the asset manager in engine->app->activity->assetmanager in main.cpp
}

void Game::Update() {
	float deltaTime = m_GameTime.Tick();
	static float time = 0;
	time += deltaTime;
	time = fmodf(time, 5.0f);
	glm::vec3 color = glm::mix(glm::mix(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), time / 2.0f), glm::vec3(0,1,0),time / 5.0f);
	color = glm::normalize(color) * 0.5f + 0.5f;
	glClearColor(color.r, color.g, color.b, 1.0f);
	
}

void Game::Draw() {
	m_Graphics.Render();
}