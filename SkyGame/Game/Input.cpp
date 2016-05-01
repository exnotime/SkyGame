#include "Input.h"

InputManager::InputManager() {
	m_CurrentTouches = new std::vector<glm::vec2>();
	m_LastFrameTouches = new std::vector<glm::vec2>();
}

InputManager::~InputManager() {
	if (m_CurrentTouches) delete m_CurrentTouches;
	if (m_LastFrameTouches) delete m_LastFrameTouches;
}

InputManager& InputManager::GetInstance() {
	static InputManager m_Instance;
	return m_Instance;
}

void InputManager::AddTouch(glm::vec2 touch) {
	m_CurrentTouches->push_back(touch);
}

int InputManager::GetTouchCount() {
	return m_CurrentTouches->size();
}

glm::vec2 InputManager::GetTouch(int index) {
	return m_CurrentTouches->at(index);
}

glm::vec2 InputManager::GetTouchDelta(int index) {
	return m_LastFrameTouches->at(index) - m_CurrentTouches->at(index);
}

void InputManager::Update() {
	//swap and clear
	m_LastFrameTouches->clear();
	std::vector<glm::vec2>* temp = m_CurrentTouches;
	m_CurrentTouches = m_LastFrameTouches;
	m_LastFrameTouches = temp;
}