#include "AABB.h"
#include "Collision.h"

using namespace Physics;
using namespace glm;

AABB::AABB(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) {
	this->xmin = xmin;
	this->xmax = xmax;
	this->ymin = ymin;
	this->ymax = ymax;
	this->zmin = zmin;
	this->zmax = zmax;
}

bool AABB::Collide(const AABB* other) {
	return !(other->xmax < this->xmin || other->xmin > this->xmax)
		&& !(other->ymax < this->ymin || other->ymin > this->ymax)
		&& !(other->zmax < this->zmin || other->zmin > this->zmax);
}

Collision* AABB::Collide(const vec3 &origin, const glm::vec3 &direction) {
	vec3 tMin = (vec3(xmin, ymin, zmin) - origin) / direction;
	vec3 tMax = (vec3(xmax, ymax, zmax) - origin) / direction;
	vec3 collPoint;

	Collision* coll = new Collision();
	if (tMin.x > 0 && tMin.x < coll->t) {
		collPoint = origin + tMin.x * direction;
		if (collPoint.y > ymin && collPoint.y < ymax && collPoint.z > zmin && collPoint.z < zmax) {
			coll->t = tMin.x;
			coll->collisionPoint = collPoint;
		}
	}
	if (tMin.y > 0 && tMin.y < coll->t) {
		collPoint = origin + tMin.y * direction;
		if (collPoint.x > ymin && collPoint.x < ymax && collPoint.z > zmin && collPoint.z < zmax) {
			coll->t = tMin.y;
			coll->collisionPoint = collPoint;
		}
	}
	if (tMin.z > 0 && tMin.z < coll->t) {
		collPoint = origin + tMin.z * direction;
		if (collPoint.y > ymin && collPoint.y < ymax && collPoint.x > xmin && collPoint.x < xmax) {
			coll->t = tMin.z;
			coll->collisionPoint = collPoint;
		}
	}
	if (tMax.x > 0 && tMax.x < coll->t) {
		collPoint = origin + tMax.x * direction;
		if (collPoint.y > ymin && collPoint.y < ymax && collPoint.z > zmin && collPoint.z < zmax) {
			coll->t = tMax.x;
			coll->collisionPoint = collPoint;
		}
	}
	if (tMax.y > 0 && tMax.y < coll->t) {
		collPoint = origin + tMax.y * direction;
		if (collPoint.x > ymin && collPoint.x < ymax && collPoint.z > zmin && collPoint.z < zmax) {
			coll->t = tMax.y;
			coll->collisionPoint = collPoint;
		}
	}
	if (tMax.z > 0 && tMax.z < coll->t) {
		collPoint = origin + tMax.z * direction;
		if (collPoint.y > ymin && collPoint.y < ymax && collPoint.x > xmin && collPoint.x < xmax) {
			coll->t = tMax.z;
			coll->collisionPoint = collPoint;
		}
	}

	//TODO
	return nullptr;
}