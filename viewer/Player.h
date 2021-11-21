#pragma once
#include "Ad.h"

#define SIZE_OF_MATRIX 33
#define PILLAR_LADDER 16

#include <time.h>
#include <iostream>
#include <vector>

struct  player
{
	float x, y, z, angleX, angleY, speed, h, dx, dy, dz, w, d;
	bool on_ground;
	float minusDy;

	player() {};
	player(float x, float y, float z);
	~player() {};

	glm::mat4 GetView();
	void Collision(float a, float b, float c);
	void Update();
	void func_j();
	bool Check(int x, int y, int z);
};

extern int MatrixWorld[SIZE_OF_MATRIX][SIZE_OF_MATRIX][SIZE_OF_MATRIX];
extern glm::vec3 applePosition;
extern std::vector<glm::vec3> positionsApple;
extern int numberOfApples;

void LoadMatrixWorld();
void catch_apple();