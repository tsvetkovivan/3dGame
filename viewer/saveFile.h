#pragma once
#include <fstream>
#include <iostream>

//void write(int score);
//void read();
class TextFile
{
private:
	std::ifstream outFile;
	std::ofstream inFile;
	const char *path;
public:
	int bestScore;

	TextFile(const char *path);
	~TextFile();

	int read();
	int write(int currentScore);
};