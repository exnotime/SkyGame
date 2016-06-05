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
	static bool AABBvsAABB(const AABB& a, const AABB& b);
	static bool RectvsPoint(const glm::vec4& rect, const glm::vec2& point);

	glm::vec3 HandleCollisions(const AABB& player,glm::vec3 velocity,const std::vector<AABB>& boxes, bool& outResting);
	float SweptAABB(const AABB& a, const AABB& b, const glm::vec3& velocity, glm::vec3& outNormal);
private:
	
};