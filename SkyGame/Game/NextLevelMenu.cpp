#include "NextLevelMenu.h"
#include "Input.h"
#include "CollisionHandler.h"
NextLevelMenu::NextLevelMenu() {

}

NextLevelMenu::~NextLevelMenu() {
	for (auto& sprite : m_Sprites)
		if (sprite) delete sprite;
	m_Sprites.clear();
}

void NextLevelMenu::Init(glm::vec2 screenSize) {
	//set up sprites
	Sprite* parent = new Sprite();
	parent->Image.Load("textures/parent.png");
	parent->SourceRect = glm::vec4(-1);//use whole texture
	glm::vec2 parentSize = glm::vec2(screenSize.x * 0.4f, screenSize.y * 0.4f);
	parent->ScreenRect = glm::vec4(screenSize.x * 0.5f - parentSize.x * 0.5f, screenSize.y * 0.5f - parentSize.y * 0.5f, parentSize.x, parentSize.y);
	m_Sprites.push_back(parent);

	//calc next buttons from the parent
	Sprite* nextLevelButton = new Sprite();
	nextLevelButton->Image.Load("textures/next.png");
	nextLevelButton->SourceRect = glm::vec4(-1);
	int padding = 10;
	nextLevelButton->ScreenRect = glm::vec4(parent->ScreenRect.x + parentSize.x * 0.5f, parent->ScreenRect.y + padding, parentSize.x * 0.5f - padding, parentSize.y - padding * 2);
	m_Sprites.push_back(nextLevelButton);

	Sprite* shareButton = new Sprite();
	shareButton->Image.Load("textures/share.png");
	shareButton->SourceRect = glm::vec4(-1);
	shareButton->ScreenRect = glm::vec4(parent->ScreenRect.x + padding, parent->ScreenRect.y + padding, parentSize.x * 0.5f - padding, parentSize.y - padding * 2);
	m_Sprites.push_back(shareButton);
}

void NextLevelMenu::Update() {
	//HACK: hardcoded index for next level button
	if (m_IsShowing) {
		glm::vec2 tapPos;
		if (g_Input.GetTap(tapPos)) {
			if (CollisionHandler::RectvsPoint(m_Sprites[1]->ScreenRect, tapPos)) {
				m_LoadNextLevel = true;
			}
		}
	}
}

bool NextLevelMenu::ShouldLoadNextLevel() {
	if (m_LoadNextLevel) {
		m_LoadNextLevel = !m_LoadNextLevel;
		return true;
	}
	else {
		return false;
	}
}