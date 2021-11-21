#include "Camera.h"

void SetView(player pl)
{
	gluLookAt(pl.x, pl.y + pl.h, pl.z, pl.x - sin(pl.angleX / 180 * PI), pl.y + pl.h + tan(pl.angleY / 180 * PI), pl.z - cos(pl.angleX / 180 * PI), 0, 1, 0);
}

void MoveW(player &pl)
{
	pl.dx = -(float)sin(pl.angleX / 180 * PI) * pl.speed;
	pl.dz = -(float)cos(pl.angleX / 180 * PI) * pl.speed;
}

void MoveS(player &pl)
{
	pl.dx = (float)sin(pl.angleX / 180 * PI) * pl.speed;
	pl.dz = (float)cos(pl.angleX / 180 * PI) * pl.speed;
}

void MoveD(player &pl)
{
	pl.dx = (float)sin((pl.angleX + 90) / 180 * PI) * pl.speed;
	pl.dz = (float)cos((pl.angleX + 90) / 180 * PI) * pl.speed;
}

void MoveA(player &pl)
{
	pl.dx = (float)sin((pl.angleX - 90) / 180 * PI) * pl.speed;
	pl.dz = (float)cos((pl.angleX - 90) / 180 * PI) * pl.speed;
}