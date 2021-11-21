#pragma once
#include <vector>
#include <cstring>//?
#include "MST.h"

extern unsigned int textTextureId;
extern unsigned int textVertexBufferId;
extern unsigned int textUVBufferId;
extern unsigned int textShaderId;
extern unsigned int textUniformId;

void initText(const char *texturePath);
void printText(const char * text, int x, int y, int size);
void cleanupText();
void timerUp(float &i);
std::string convert(float time);