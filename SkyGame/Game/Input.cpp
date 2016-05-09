#include "Input.h"

InputManager::InputManager() {
	m_CurrentTouches.resize(3);
	m_TouchDeltas.resize(3);
}

InputManager::~InputManager() {
}

InputManager& InputManager::GetInstance() {
	static InputManager m_Instance;
	return m_Instance;
}

void InputManager::UpdateTouch(glm::vec2 pos, int index) {
	m_TouchDeltas[index] = pos - m_CurrentTouches[index];
	m_TouchDeltas[index] = glm::clamp(m_TouchDeltas[index], glm::vec2(-5), glm::vec2(5)); //clamp 
	m_CurrentTouches[index] = pos;
}

void InputManager::Update() {
	//clear deltas so we dont have sticky fingers
	for (auto& delta : m_TouchDeltas) {
		delta = glm::vec2(0);
	}
}

glm::vec2 InputManager::GetTouch(int index) {
	if (index < 0 || index >= m_CurrentTouches.size())
		return glm::vec2(0, 0);
	return m_CurrentTouches.at(index);
}

glm::vec2 InputManager::GetTouchDelta(int index) {
	if(index < 0 || index >= m_TouchDeltas.size())
		return glm::vec2(0, 0);
	return m_TouchDeltas.at(index);
}
