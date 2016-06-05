#include "Level.h"
#include "AssetManager.h"
#include <sstream>
Level::Level() {

}

Level::~Level() {

}

void Level::Load(const std::string& filename) {
	m_Points.clear();
	AssetFile* file;
	if (g_AssetManager.LoadAssetFile(filename, &file)) {
		std::string content = std::string(file->Data, file->Size);
		g_AssetManager.FreeAssetFile(file);
		glm::vec3 point;
		std::stringstream ss;
		ss << content;
		while (!ss.eof()) {
			ss >> point.x >> point.y >> point.z;
			m_Points.push_back(point);
		}
		
	}
}

std::vector<glm::vec3>& Level::GetPoints() {
	return m_Points;
}