#include "saveFile.h"

TextFile::TextFile(const char *path)
{
	this->path = path;
	if (this->read() != 0) std::cout << "cant read file\n";
}

TextFile::~TextFile()
{

}

int TextFile::write(int currentScore)
{
	inFile.open(path);
	if (!inFile.is_open()) return -1;
	inFile << currentScore;
	inFile.close();
	return 0;
}

int TextFile::read()
{
	outFile.open(path);
	if (!outFile.is_open()) return -1;
	outFile >> bestScore;
	outFile.close();
	return 0;
}