#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
class Level {
public:
	Level();
	~Level();
	void Load(const std::string& filename);
	std::vector<glm::vec3>& GetPoints();
private:
	std::vector<glm::vec3> m_Points;
};