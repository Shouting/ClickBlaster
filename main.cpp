#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <random>
#include <time.h>
#include <WinUser.h>
#include <windowsx.h>
#include <fstream>
#include <ctime>

#include "main.h"



//			Better UI? Possibly using PDCurses? Or WPF (DISGUSTING)

POINT point_topLeft;
POINT point_botRight;
int numberOfClicks;
int pauseBetweenClicks;
std::vector<int> xRecordedClicks;
std::vector<int> yRecordedClicks;

void startupSequence()
{

	std::cout << "\n" << std::endl;
	std::cout << "      _________      __   ____  __           __            " << std::endl;
	std::cout << "     / ____/ (_)____/ /__/ __ )/ /___ ______/ /____  _____ " << std::endl;
	std::cout << "    / /   / / / ___/ //_/ __  / / __ `/ ___/ __/ _ \\/ ___/" << std::endl;
	std::cout << "   / /___/ / / /__/ ,< / /_/ / / /_/ (__  ) /_/  __/ /     " << std::endl;
	std::cout << "   \\____/_/_/\\___/_/|_/_____/_/\\__,_/____/\\__/\\___/_/ " << std::endl;

		
	std::string recordedClickLog;
	std::cout << "\tTo replay a pre-recorded cicking session, drag and drop the file into this window and hit [Enter]" << std::endl;;
	std::cout << "\tElse, hit [Enter] to continue with a new random session." << std::endl;
	std::getline(std::cin, recordedClickLog);
	
	if (!recordedClickLog.empty())
	{
		readInFile(recordedClickLog);
		clickerLoopFromFile();
	}
	else
	{
		std::string clickCount;
		std::string clickPause;

		std::cout << "\tPlease specify the number of clicks" << std::endl;
		std::getline(std::cin, clickCount);

		if (std::stoi(clickCount) < 1 || std::stoi(clickCount) > INT_MAX - 1 || std::stoi(clickCount) == NULL)
		{
			std::cout << "\tInvalid entry; Changing value to 1,000" << std::endl;
			numberOfClicks = 1000;
		}
		else
		{
			numberOfClicks = std::stoi(clickCount);
		}

		std::cout << "\tPlease specify the click pause (in miliseconds)" << std::endl;
		std::getline(std::cin, clickPause);
		if (std::stoi(clickPause) < 10 || std::stoi(clickPause) > 100000 || std::stoi(clickPause) == NULL)
		{
			std::cout << "\tInvalid entry.\n\t10 miliseconds pause; Otherwise the process will choke the cpu" << std::endl;
			pauseBetweenClicks = 10;
		}
		else
		{
			pauseBetweenClicks = std::stoi(clickPause);
		}

		std::cout << "\t1. Click in the top-right corner of where you want to set the test" << std::endl;
		point_topLeft = getClickPosition();

		Sleep(500);
		std::cout << "\t2. Click in the bottom-left corner of where you want to set the test" << std::endl;
		point_botRight = getClickPosition();


		std::cout << "\tClick barrage will begin in\n" << std::endl;
		Sleep(1000);
		std::cout << "3.." << std::endl;
		Sleep(1000);
		std::cout << "2.." << std::endl;
		Sleep(1000);
		std::cout << "1.." << std::endl;
		Sleep(1000);

		std::cout << "\n\tBaraging with clicks, hit [END] to stop..." << std::endl;

		clickerLoop();
	}
}

int main()
{
	startupSequence();
	
	std::cout << "\tEnding operation." << std::endl;
}

bool readInFile(std::string filename)
{
	if (filename.empty())
	{
		return false;
	}

	std::string line;
	std::ifstream file(filename);
	if (file.is_open())
	{
		int index = 0;
		while (std::getline(file, line))
		{
			if (index == 0)
			{
				pauseBetweenClicks = std::stoi(line);;
			}
			else
			{
				std::size_t xfound = line.find("x:");
				std::size_t yfound = line.find("y:");
				if (xfound == std::string::npos || xfound == std::string::npos)
				{
					return false;
				}

				std::string xcoordsString = "";
				std::string ycoordsString = "";

				xcoordsString.push_back(line.at(xfound + 2));

				for (signed int i = xfound + 3; i < line.size(); ++i)
				{
					if (line.at(i) == ' ')
					{
						break;
					}
					else
					{
						xcoordsString.push_back(line.at(i));
					}
				}

				ycoordsString.push_back(line.at(yfound + 2));

				for (signed int i = yfound + 3; i < line.size(); ++i)
				{
					if (line.at(i) == ' ')
					{
						break;
					}
					else
					{
						ycoordsString.push_back(line.at(i));
					}
				}

				int xpos = std::stoi(xcoordsString);
				int ypos = std::stoi(ycoordsString);

				xRecordedClicks.push_back(xpos);
				yRecordedClicks.push_back(ypos);
			}
			++index;
		}
	}

	for (int i=0; i < xRecordedClicks.size(); ++i)
	{
		std::cout << std::to_string(xRecordedClicks[i]) << " " << std::to_string(yRecordedClicks[i]) <<  std::endl;
	}
}

std::string outputFileNameDateFormatter()
{
	std::string format = "";

	struct tm timeNow;
	time_t now = time(0);
	localtime_s(&timeNow, &now);

	// yeah you can do it all on one line but it'd be a mess
	format.append(std::to_string((timeNow.tm_year)+1900) += "-" );
	format.append(std::to_string((timeNow.tm_mon)+1) += "-");
	format.append(std::to_string(timeNow.tm_mday) += "_");
	format.append(std::to_string(timeNow.tm_hour) += "-");
	format.append(std::to_string(timeNow.tm_min));
	
	return format;
}

bool clickerLoopFromFile()
{
	if (xRecordedClicks.size() != yRecordedClicks.size())
	{
		std::cout << "\tSomething's gone wrong. Sorry, time to die." << std::endl;
		Sleep(1000);
		return false;
	}

	std::cout << "\tNumber of clicks: " << xRecordedClicks.size() 
		<< "\n\tPause between clicks: " << pauseBetweenClicks << std::endl;

	std::cout << "\tClick barrage will begin in\n" << std::endl;
	Sleep(1000);
	std::cout << "3.." << std::endl;
	Sleep(1000);
	std::cout << "2.." << std::endl;
	Sleep(1000);
	std::cout << "1.." << std::endl;
	Sleep(1000);

	for (int i = 0; i < xRecordedClicks.size(); ++i)
	{
		if (GetAsyncKeyState(VK_END))
		{
			std::cout << "\n\tEND PRESSED" << std::endl;
			break;
		}

		Sleep(pauseBetweenClicks);
		mouseActions(xRecordedClicks.at(i), yRecordedClicks.at(i));
	}

	return true;
}

void clickerLoop()
{
	std::ofstream outputFile;
	std::string outputFileName = outputFileNameDateFormatter() += ".txt";
	outputFile.open(outputFileName);

	outputFile << pauseBetweenClicks << std::endl;

	for (int i = 0; i < numberOfClicks; ++i) {

		// the GetAsyncKeyState function is great!
		// can detect from whatever selected window!
		if (GetAsyncKeyState(VK_END))
		{
			std::cout << "\n\tEND PRESSED" << std::endl;
			break;
		}

		int new_x = getRandomInt(point_topLeft.x, point_botRight.x);
		int new_y = getRandomInt(point_topLeft.y, point_botRight.y);

		Sleep(pauseBetweenClicks);
		mouseActions(new_x, new_y);

		outputFile << "x:";
		outputFile << (std::to_string(new_x));
		outputFile << " y:";
		outputFile << (std::to_string(new_y));
		outputFile << " \n";
	}
	outputFile.close();
	std::cout << "\tKILLING . . ." << std::endl;
	Sleep(1000);
}

void mouseActions(int x, int y)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	SetCursorPos(x, y);
}

int getRandomInt(int min, int max)
{
	std::random_device randomDevice;
	std::default_random_engine engine(randomDevice());
	std::uniform_int_distribution<int> uniformDist(min, max);
	return uniformDist(engine);
}

void copyClicksToFile(int x, int y)
{
	std::ofstream outputFile;
	std::string outputFileName = "output.txt";
	outputFile.open(outputFileName);

	outputFile << "x:";
	outputFile << (std::to_string(x));
	outputFile << " y:";
	outputFile << (std::to_string(y));
	outputFile << "\n";
	
	outputFile.close();
}

POINT getClickPosition()
{
	bool clicked = false;
	POINT position;
	
	while (!clicked)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			GetCursorPos(&position);
			clicked = true;
		}
	}
	return position;
}