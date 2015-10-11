#include "particle.h"
#include <iostream>

Particle::Particle(vec3 position, vec3 velocity, float mass)
{
	x0 = position;
	v0 = velocity;
	m = mass;
	reset();
}

Particle::Particle(float xx, float xy, float xz, float vx, float vy, float vz, float mass)
{
	x0 = vec3(xx, xy, xz);
	v0 = vec3(vx, vy, vz);
	m = mass;
	reset();
}

void Particle::display()
{
	//TODO: implement display
	std::cout << "display() not implemented in class Particle" << std::endl;
}

void Particle::reset()
{
	x = x0;
	v = v0;
}
