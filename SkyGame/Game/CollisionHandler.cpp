#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {

}

CollisionHandler::~CollisionHandler() {

}

bool CollisionHandler::AABBvsAABB(const AABB& a, const AABB& b) {
	return (a.Max.x > b.Min.x && a.Min.x < b.Max.x &&
		a.Max.y > b.Min.y && a.Min.y < b.Max.y &&
		a.Max.z > b.Min.z && a.Min.z < b.Max.z);
}

bool CollisionHandler::RectvsPoint(const glm::vec4& rect, const glm::vec2& point) {
	return	point.x > rect.x && point.x < rect.x + rect.z &&
			point.y > rect.y && point.y < rect.y + rect.w;
}

glm::vec3 CollisionHandler::HandleCollisions(const AABB& player, glm::vec3 velocity, const std::vector<AABB>& boxes, bool& outInAir) {
	//if we are resting dont really need collisions
	if (!outInAir) {
		return glm::vec3(0);
	}
	//construct broadphase AABB
	AABB broadphase;
	broadphase.Min = glm::min(player.Min, player.Min + velocity);
	broadphase.Max = glm::max(player.Max, player.Max + velocity);
	
	float minTime = 1.0f;
	glm::vec3 finalNormal;
	for (auto& box : boxes) {
		if (AABBvsAABB(broadphase, box)) {
			glm::vec3 normal;
			float time = SweptAABB(player, box, velocity, normal);
			if (time < minTime) {
				finalNormal = normal;
				minTime = time;
			}
		}
	}
	glm::vec3 playerMovement = velocity * minTime;
	//if minTime = 0 the collision normal may be flipped 
	if (finalNormal == glm::vec3(0, 1, 0) || finalNormal == glm::vec3(0, -1, 0)) {
		outInAir = false;
		return playerMovement;
	}
	if (minTime < 1.0f) {
		if (finalNormal.x != 0) {
			velocity.x = 0;
		}
		else if (finalNormal.y != 0) {
			velocity.y = 0;
		}
		else {
			velocity.z = 0;
		}
		playerMovement += finalNormal * 0.1f; //move out of near plane
		playerMovement += velocity * (1.0f - minTime);
	}
	outInAir = true;
	return playerMovement;
}

float CollisionHandler::SweptAABB(const AABB& a, const AABB& b, const glm::vec3& velocity, glm::vec3& outNormal) {
	glm::vec3 invEntry, invExit;
	if (velocity.x > 0) {
		invEntry.x = b.Min.x - a.Max.x;
		invExit.x = b.Max.x - a.Min.x;
	} else {
		invEntry.x = b.Max.x - a.Min.x;
		invExit.x = b.Min.x - a.Max.x;
	}

	if (velocity.y > 0) {
		invEntry.y = b.Min.y - a.Max.y;
		invExit.y = b.Max.y - a.Min.y;
	}
	else {
		invEntry.y = b.Max.y - a.Min.y;
		invExit.y = b.Min.y - a.Max.y;
	}

	if (velocity.z > 0) {
		invEntry.z = b.Min.z - a.Max.z;
		invExit.z = b.Max.z - a.Min.z;
	}
	else {
		invEntry.z = b.Max.z - a.Min.z;
		invExit.z = b.Min.z - a.Max.z;
	}

	glm::vec3 entry, exit;
	if (velocity.x == 0.0f) {
		entry.x = -std::numeric_limits<float>::infinity();
		exit.x = std::numeric_limits<float>::infinity();
	} else {
		entry.x = invEntry.x / velocity.x;
		exit.x = invExit.x / velocity.x;
	}

	if (velocity.y == 0.0f) {
		entry.y = -std::numeric_limits<float>::infinity();
		exit.y = std::numeric_limits<float>::infinity();
	}
	else {
		entry.y = invEntry.y / velocity.y;
		exit.y = invExit.y / velocity.y;
	}

	if (velocity.z == 0.0f) {
		entry.z = -std::numeric_limits<float>::infinity();
		exit.z = std::numeric_limits<float>::infinity();
	}
	else {
		entry.z = invEntry.z / velocity.z;
		exit.z = invExit.z / velocity.z;
	}

	float entryTime = glm::max(entry.x, glm::max(entry.y, entry.z));
	float exitTime = glm::min(exit.x, glm::min(exit.y, exit.z));
	if (entryTime > exitTime || glm::all(glm::lessThan(entry, glm::vec3(0.0f))) || glm::any(glm::greaterThan(entry, glm::vec3(1.0f)))) {
		//no collision
		outNormal = glm::vec3(0);
		return 1.0f;
	} else {
		//calc normal
		if (entry.x > entry.y && entry.x > entry.z) {
			if (invEntry.x < 0.0f) {
				outNormal = glm::vec3(1, 0, 0);
			} else {
				outNormal = glm::vec3(-1, 0, 0);
			}
		} else if (entry.y > entry.x && entry.y > entry.z) {
			if (invEntry.y < 0.0f) {
				outNormal = glm::vec3(0, 1, 0);
			} else {
				outNormal = glm::vec3(0, -1, 0);
			}
		} else {
			if (invEntry.z < 0.0f) {
				outNormal = glm::vec3(0, 0, 1);
			} else {
				outNormal = glm::vec3(0, 0, -1);
			}
		}
		return entryTime;
	}
}