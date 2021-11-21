#include "Player.h"

extern float timeToEnd;

int MatrixWorld[SIZE_OF_MATRIX][SIZE_OF_MATRIX][SIZE_OF_MATRIX];
glm::vec3 applePosition;
std::vector<glm::vec3> positionsApple;

player::player(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	speed = 0.3;
	h = 1.2;
	w = 0.3;
	d = 0.3;
	on_ground = false;
	minusDy = 0.7;
}

glm::mat4 player::GetView()
{
	glm::vec3 Position(x, y + h, z);
	glm::vec3 Side(x - sin(angleX / 180 * PI), y + h + tan(angleY / 180 * PI), z - cos(angleX / 180 * PI));
	glm::vec3 Up(0, 1, 0);

	return glm::lookAt(Position, Side, Up);
}

void LoadMatrixWorld()
{
	for (int ix = 0; ix < SIZE_OF_MATRIX; ix++)
	{
		for (int iy = 0; iy < SIZE_OF_MATRIX; iy++)
		{
			for (int iz = 0; iz < SIZE_OF_MATRIX; iz++)
			{
				if (iy == 0) { MatrixWorld[ix][iy][iz] = 1; }//floor
				else { MatrixWorld[ix][iy][iz] = 0; }

				if (ix == 2 || ix == 30 || iz == 4 || iz == 29)MatrixWorld[ix][iy][iz] = 8;
			}
		}
	}
	MatrixWorld[SIZE_OF_MATRIX / 2 + 5][1][SIZE_OF_MATRIX / 5 + 5] = 2;//tree
	MatrixWorld[SIZE_OF_MATRIX - 10][1][SIZE_OF_MATRIX - 10] = 2;

	MatrixWorld[9][1][14] = 4;//bush
	MatrixWorld[19][1][19] = 4;
	
	///////////////////////////////////////////////////////////////crate
	for (int i = PILLAR_LADDER + 1; i < PILLAR_LADDER + 3; i++)
		for (int j = PILLAR_LADDER; j < PILLAR_LADDER + 3; j++)
			MatrixWorld[i][12][j] = 3;
	int p = 12;
	for (int i = PILLAR_LADDER + 3; i < PILLAR_LADDER + 9; i++)
		MatrixWorld[i][p++][PILLAR_LADDER + 2] = 3;
	MatrixWorld[11][2][23] = MatrixWorld[15][2][23] = MatrixWorld[24][2][10] = MatrixWorld[15][2][9] = 3;
	MatrixWorld[10][1][23] = MatrixWorld[15][1][24] = MatrixWorld[25][1][10] = MatrixWorld[15][1][10] = 3;
	MatrixWorld[11][1][23] = MatrixWorld[15][1][23] = MatrixWorld[24][1][10] = MatrixWorld[15][1][9] = 3;

	applePosition.x = 12; applePosition.y = 2; applePosition.z = 12;
	MatrixWorld[(int)applePosition.x][(int)applePosition.y][(int)applePosition.z] = 5;//apple

	MatrixWorld[PILLAR_LADDER][1][PILLAR_LADDER] = 6;//ladder
	MatrixWorld[PILLAR_LADDER][8][PILLAR_LADDER] = 6;
	for (int i = 2; i < 8; i++)
		MatrixWorld[PILLAR_LADDER][i][PILLAR_LADDER] = 7;
	for (int i = 9; i < 15; i++)
		MatrixWorld[PILLAR_LADDER][i][PILLAR_LADDER] = 7;
 

	///////////////////////////////////////////////////////////////apples

	positionsApple.resize(15);

	positionsApple[0].x = 10; positionsApple[0].y = 2; positionsApple[0].z = 24;
	positionsApple[1].x = 15; positionsApple[1].y = 2; positionsApple[1].z = 25;
	positionsApple[2].x = 11; positionsApple[2].y = 4; positionsApple[2].z = 23;
	positionsApple[3].x = 15; positionsApple[3].y = 4; positionsApple[3].z = 24;
	positionsApple[4].x = 25; positionsApple[4].y = 2; positionsApple[4].z = 9;
	positionsApple[5].x = 25; positionsApple[5].y = 4; positionsApple[5].z = 10;
	positionsApple[6].x = 15; positionsApple[6].y = 2; positionsApple[6].z = 8;
	positionsApple[7].x = 15; positionsApple[7].y = 3; positionsApple[7].z = 9;
	positionsApple[8].x = 18; positionsApple[8].y = 4; positionsApple[8].z = 18;
	positionsApple[9].x = 22; positionsApple[9].y = 17; positionsApple[9].z = 18;
	positionsApple[10].x = 20; positionsApple[10].y = 2; positionsApple[10].z = 20;
	positionsApple[11].x = 12; positionsApple[11].y = 2; positionsApple[11].z = 18;
	positionsApple[12].x = 9; positionsApple[12].y = 2; positionsApple[12].z = 12;
	positionsApple[13].x = 22; positionsApple[13].y = 2; positionsApple[13].z = 7;
	positionsApple[14].x = 25; positionsApple[14].y = 2; positionsApple[14].z = 18;
}

void catch_apple()
{
	MatrixWorld[(int)applePosition.x][(int)applePosition.y][(int)applePosition.z] = 0;

	int temp = rand() % 15;

	applePosition.x = positionsApple[temp].x;
	applePosition.y = positionsApple[temp].y;
	applePosition.z = positionsApple[temp].z;

	MatrixWorld[(int)applePosition.x][(int)applePosition.y][(int)applePosition.z] = 5;

	timeToEnd += 8;
	numberOfApples++;
}

bool player::Check(int x, int y, int z){
	if (x > SIZE_OF_MATRIX || x < 0 ||
		z > SIZE_OF_MATRIX || z < 0 ||
		y > SIZE_OF_MATRIX)return false;

	if (this->y < 0) this->y = 1.5;

	if (MatrixWorld[x][y][z] == 5){ catch_apple(); return false; }
	if (MatrixWorld[x][y][z] == 6 || MatrixWorld[x][y][z] == 7){
		on_ground = true;
		this->y += 0.1;
		return false;
	}

	return MatrixWorld[x][y][z];
}

void player::Collision(float a, float b, float c)
{
	for (int X = (x - w); X < (x + w); X++)
		for (int Y = (y - (h / 2)); Y < (y + (h / 2)); Y++)
			for (int Z = (z - d); Z < (z + d); Z++)
				if (Check(X, Y, Z)){
					if (a > 0) { x = X - w; }
					if (a < 0) { x = X + w + 1; }
					if (c > 0) { z = Z - d; }
					if (c < 0) { z = Z + d + 1; }
					if (b > 0) { y = Y - h; }
					if (b < 0) { y = Y + 1 + h; on_ground = true; dy = 0; }
				}
}

void player::Update()
{
	if (!on_ground){ dy -= minusDy; }
	on_ground = false;

	x += dx;
	Collision(dx, 0, 0);
	z += dz;
	Collision(0, 0, dz);
	y += dy;
	Collision(0, dy, 0);

	dx = dz = 0;
}

void player::func_j()
{
	if (on_ground)
	{
		on_ground = false;
		dy = 2;
	}
}