#pragma once
#include <glm/glm.hpp>
#include <vector>
struct AABB {
	glm::vec3 Min;
	glm::vec3 Max;
};
class CollisionHandler {
public:
	CollisionHandler();
	~CollisionHandler();
	bool AABBvsAABB(const AABB& a, const AABB& b);
	glm::vec3 HandleCollisions(const AABB& player,glm::vec3 velocity,const std::vector<AABB>& boxes);
	float SweptAABB(const AABB& a, const AABB& b, const glm::vec3& velocity, glm::vec3& outNormal);
private:
	
};