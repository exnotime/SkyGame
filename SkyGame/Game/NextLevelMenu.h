#pragma once
#include "SpriteEngine.h"
#include <vector>
#include <glm/glm.hpp>
class NextLevelMenu {
public:
	NextLevelMenu();
	~NextLevelMenu();
	void Init(glm::vec2 screenSize);
	void Update();
	bool ShouldLoadNextLevel();
	void ToggleShow() { m_IsShowing = !m_IsShowing; }
	bool Showing() { return m_IsShowing; }
	std::vector<Sprite*> GetSprites() { return m_Sprites; }
private:
	bool m_IsShowing = false;
	bool m_LoadNextLevel = false;
	std::vector<Sprite*> m_Sprites;
};