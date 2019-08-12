#pragma once
#include <Windows.h>
#include <string>

class WorkTimer
{
public:
	WorkTimer();
	std::string getWorkTimeText();
	std::string getFps();
private:
	int fps = 0;
	int showFps = 0;
	unsigned int lastSec = 0;
	unsigned int programStarted_;
};

