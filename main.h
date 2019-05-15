#pragma once

struct MousePositionStruct
{
	POINT point1;
	unsigned sleep1;
	POINT point2;
	unsigned sleep2;
	MousePositionStruct() { point1.x = point1.y = sleep1 = point2.x = point2.y = sleep2 = 0; }
};

void startupSequence();
void mouseActions(int x, int y);
int getRandomInt(int min, int max); 
POINT getClickPosition();
void clickerLoop();
void copyClicksToFile(int x, int y);
std::string outputFileNameDateFormatter();
bool readInFile(std::string);
bool clickerLoopFromFile();