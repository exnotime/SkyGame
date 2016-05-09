#pragma once
#include <glm/glm.hpp>
#include <vector>

#define g_Input InputManager::GetInstance()
class InputManager{
public:
	~InputManager();
	static InputManager& GetInstance();
	
	void UpdateTouch(glm::vec2 pos, int index);
	void Update();
	glm::vec2 GetTouch(int index);
	glm::vec2 GetTouchDelta(int index);
	void SetTouchCount(int count) { m_TouchCount = count; }
	int GetTouchCount() { return m_TouchCount; }
private:
	InputManager();
	int m_TouchCount = 0;
	std::vector<glm::vec2> m_CurrentTouches;
	std::vector<glm::vec2> m_TouchDeltas;

};