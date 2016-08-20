#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Particle {
public:
	vec3 x;
	vec3 x_star;
	vec3 v;
	vec3 v_star;
	float color[4];
	float m;

	Particle(vec3 position, vec3 velocity, float mass = 1);
	Particle(float xx, float xy, float xz, float vx, float vy, float vz, float mass = 1);

	void display();
	void reset();

private:
	vec3 x0;
	vec3 v0;
};