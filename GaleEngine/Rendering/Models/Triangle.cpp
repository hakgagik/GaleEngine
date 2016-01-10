#include "Triangle.h"
using namespace Rendering;
using namespace Models;

Triangle::Triangle() {}

Triangle::~Triangle() {/*will be destroyed in models.cpp*/}

void Triangle::Create() {
	GLuint vao;
	GLuint vbo;
	glGenVertexArrays(1, &vao); // ask GL to make the vao
	glBindVertexArray(vao); // bind the vao

	// create the triangle vertices
	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(glm::vec3( 0.25, -0.25, 0.0), glm::vec4(1, 0, 0, 1)));
	vertices.push_back(VertexFormat(glm::vec3(-0.25, -0.25, 0.0), glm::vec4(0, 1, 0, 1)));
	vertices.push_back(VertexFormat(glm::vec3( 0.25,  0.25, 0.0), glm::vec4(0, 0, 1, 1)));

	// ask gl to generate one vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind that buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * 3, &vertices[0], GL_STATIC_DRAW); // put the triangle data into the vbo
	glEnableVertexAttribArray(0); // enable the first attribute in the vao to be used for position (the first vaa in the vao, if you will)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0); // tell gl how to read just the vertex positions from the vbo
	glEnableVertexAttribArray(1); // enable the second attribute array to be used for color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::color))); // same as above, but with color. note the non-zero offset
	glBindVertexArray(0); //unbind the vao

	this->vao = vao;
	this->vbos.push_back(vbo);
}

void Triangle::Update() {
	// Nothing to update for now
}

void Triangle::Draw() {
	glUseProgram(program); // pick a program
	glBindVertexArray(vao); // bind the vao (since only one vao can be bound at a time and other objects could have bound their own)
	glDrawArrays(GL_TRIANGLES, 0, 3); // draw the vaos. start at the 0th set of attributes and draw three of them (coud also use vertices.size()?)
}