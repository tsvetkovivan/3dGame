#include "shaders.h"
#include "Player.h"
#include "Camera.h"
#include "MST.h"
#include "Objects.h"
#include "skybox.h"
#include "text.h"
#include "saveFile.h"

float timeToEnd = 17;
int numberOfApples = 0;
TextFile bestScore("bestScore.txt");

bool isGame = false;

player p1(15, 1.5, 15);
shader shad;

std::string directory = "model/";

Mesh Tree(glm::vec3(4, 4, 4)), Bush(glm::vec3(0.5, 0.5, 0.5)), Crate(glm::vec3(1.5, 1.5, 1.5)), Apple(glm::vec3(15, 15, 15)), Ladder(glm::vec3(1, 3.2, 1)), Rock(glm::vec3(30, 30, 30));// 3d models
std::string nameTree = "Tree.obj";
std::string nameBush = "Palm_01.obj";
std::string nameCrate = "Crate1.obj";
std::string nameApple = "apple.obj";
std::string nameLadder = "objStairs.obj";
std::string nameRock = "Rock1.obj";

Box grass("gr.jpg", 100.0f);//100
skybox sbox("top.jpg", "bottom.jpg", "left.jpg", "right.jpg", "front.jpg", "back.jpg");

//menu
Texture menuImg("menu.jpg");

void resetVariables()
{
	p1.x = 15; 
	p1.y = 2;
	p1.z = 15;
	catch_apple();
	timeToEnd = 17;
	numberOfApples = 0;
}

void init()
{
	glfwInit();
	glewInit();
	shad.Setup();
	sbox.LoadData();
	LoadMatrixWorld();
	grass.LoadData();
	menuImg.LoadTexture();
	
	Ladder.LoadMesh(directory + nameLadder);
	Tree.LoadMesh(directory + nameTree);
	Bush.LoadMesh(directory + nameBush);
	Crate.LoadMesh(directory + nameCrate);
	Apple.LoadMesh(directory + nameApple);
	Rock.LoadMesh(directory + nameRock);

	initText("Holstein.DDS");
}

void disp()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	

	glm::vec3 bias(0, 1, 0);
	glm::vec3 scale(1, 1, 1);
	glm::vec3 rotate(1, 0, 0);

	//text
	char a[] = { "TIME \0" };
	printText(a, 0, 700, 70);
	const std::string temp = convert(timeToEnd);
	printText(temp.c_str(), 400, 700, 70);
	if ((int)timeToEnd <= 0)
	{
		if (numberOfApples > bestScore.bestScore)
		{
			if (bestScore.write(numberOfApples) != 0) ConsoleLog("cant write file\n");
			bestScore.bestScore = numberOfApples;
		}
		isGame = false;
	}

	shad.Enable();// main shader

	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);//field
	grass.Draw();
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);
	
	Rock.scale.z = 45;
	bias.x = -11; bias.y = 0.5; bias.z = 16;
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, 0, 1);//rock
	Rock.Render();
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, 0, 1);
	Rock.scale.z = 30;

	Rock.scale.x = 45; bias.x = 15; bias.z = -10;
	rotate.x = 0; rotate.y = 1;
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, 175, 1);//rock
	Rock.Render();
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, 175, 1);
	Rock.scale.x = 30;
	rotate.x = 1; rotate.y = 0;

	Rock.scale.z = 45;
	bias.x = 44; bias.y = 0.5; bias.z = 16; rotate.x = 0; rotate.y = 1;
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, 180, 1);//rock
	Rock.Render();
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, 180, 1);
	Rock.scale.z = 30; rotate.x = 1; rotate.y = 0;

	Rock.scale.x = 53; bias.x = 15; bias.z = 46; bias.y = -2;
	rotate.x = 0; rotate.y = 1;
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, -3, 1);//rock
	Rock.Render();
	SetShaderModification(p1, shad, bias, Rock.scale, rotate, -3, 1);
	Rock.scale.x = 30;
	rotate.x = 1; rotate.y = 0;
	
	for (int ix = 0; ix < SIZE_OF_MATRIX; ix++)
		for (int iy = 0; iy < SIZE_OF_MATRIX; iy++)
			for (int iz = 0; iz < SIZE_OF_MATRIX; iz++)
			{
				bias.x = ix + 0.5; bias.y = iy; bias.z = iz + 0.5;

				if (MatrixWorld[ix][iy][iz] == 2)
				{
					SetShaderModification(p1, shad, bias, Tree.scale, rotate, 0, 1);//tree
					Tree.Render();
					SetShaderModification(p1, shad, bias, Tree.scale, rotate, 0, 0);
				}
				else if (MatrixWorld[ix][iy][iz] == 4)
				{
					SetShaderModification(p1, shad, bias, Bush.scale, rotate, 0, 1);//bush
					Bush.Render();
					SetShaderModification(p1, shad, bias, Bush.scale, rotate, 0, 0);
				}
				else if (MatrixWorld[ix][iy][iz] == 3)
				{
					bias.y += 0.5;
					SetShaderModification(p1, shad, bias, Crate.scale, rotate, 0, 1);//crate
					Crate.Render();
					SetShaderModification(p1, shad, bias, Crate.scale, rotate, 0, 0);
					bias.y -= 0.5; 
				}
				else if (MatrixWorld[ix][iy][iz] == 5)
				{
					SetShaderModification(p1, shad, bias, Apple.scale, rotate, 0, 1);//apple
					Apple.Render();
					SetShaderModification(p1, shad, bias, Apple.scale, rotate, 0, 0);
				}
				else if (MatrixWorld[ix][iy][iz] == 6)
				{
					SetShaderModification(p1, shad, bias, Ladder.scale, rotate, 0, 1);//ladder
					Ladder.Render();
					SetShaderModification(p1, shad, bias, Ladder.scale, rotate, 0, 1);
				}

			}
			
	bias.x = -10; bias.y = -10; bias.z = -10;//skybox
	sbox.Cube(bias, p1, shad);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void menu()
{
	glColor3f(0, 128, 128);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	char a[] = { "to continue, press space\0" };
	printText(a, 45, 370, 30);
	char b[] = { "current result\0" };
	printText(b, 30, 700, 30);
	const std::string curSc = convert(numberOfApples);
	printText(curSc.c_str(), 500, 700, 30);
	char c[] = { "best result\0" };
	printText(c, 30, 600, 30);
	const std::string bestSc = convert(bestScore.bestScore);
	printText(bestSc.c_str(), 450, 600, 30);

	glutSwapBuffers();
}

void Mouse()
{
	RECT rect;
	HWND hwnd;
	hwnd = GetForegroundWindow();
	GetWindowRect(hwnd, &rect);

	POINT mousexy;
	GetCursorPos(&mousexy);
	int middleX = rect.left + 400;
	int middleY = rect.top + 300;

	p1.angleX += (middleX - mousexy.x) / 4;
	p1.angleY += (middleY - mousexy.y) / 4;

	SetCursorPos(middleX, middleY);
}

void keyboardNew()
{
	if (isGame)
	{
		if (GetAsyncKeyState(0x57)) MoveW(p1);
		if (GetAsyncKeyState(0x44)) MoveD(p1);
		if (GetAsyncKeyState(0x53)) MoveS(p1);
		if (GetAsyncKeyState(0x41)) MoveA(p1);
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (isGame) p1.func_j();
		else { resetVariables(); isGame = true; }
	}
	if (GetAsyncKeyState(VK_ESCAPE)) exit(0);
}

void Timer(int)
{
	std::cout << (int)p1.x << "  " << (int)p1.y << " " << (int)p1.z << std::endl;
	
	if (isGame)
	{
		timerUp(timeToEnd);
		glLoadIdentity();
		SetView(p1);
		p1.Update();
		disp();
		Mouse();
	}
	else{
		menu();
	}

	keyboardNew();
	//glutPostRedisplay();
	glutTimerFunc(50, Timer, 0);
}

int main(int argc, char **argv)
{
	ShowCursor(false);
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("game");

	glClearColor(0.014, 0.116, 0.199, 0.001);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 1.0, 500);
	glMatrixMode(GL_MODELVIEW);

	init();

	glutTimerFunc(50, Timer, 0);
	glutDisplayFunc(disp);
	//glutKeyboardFunc(Keyboard);

	glutMainLoop();

	cleanupText();
	return 0;
}