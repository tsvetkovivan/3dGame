#pragma once
#include "MST.h"
#include "shaders.h"
#include "Player.h"

struct Box{
	Texture *img = NULL;
	static GLfloat vertices[];
	static GLuint indices[];
	const char *path;
	GLuint VBO, VAO, EBO;
	float size;

	Box(const char *path, float size) : path(path), size(size) {};
	void LoadData();
	void Draw();
	void Cube(glm::vec3 &bias, player &p1, shader &shad);
	void set_size();
};