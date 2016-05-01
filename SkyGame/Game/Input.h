#pragma once
#include <glm/glm.hpp>
#include <vector>
class InputManager{
public:
	~InputManager();
	static InputManager& GetInstance();
	void AddTouch(glm::vec2 touch);
	int GetTouchCount();
	glm::vec2 GetTouch(int index);
	glm::vec2 GetTouchDelta(int index);
	void Update();
private:
	InputManager();
	std::vector<glm::vec2>* m_CurrentTouches;
	std::vector<glm::vec2>* m_LastFrameTouches;

};