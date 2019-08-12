#include "WorkTimer.h"

WorkTimer::WorkTimer()
{
	programStarted_ = (unsigned int)GetTickCount64();
}

std::string WorkTimer::getWorkTimeText()
{
	return std::to_string(((unsigned int)GetTickCount64() - programStarted_) / 1000);
}

std::string WorkTimer::getFps()
{
	if ((((unsigned int)GetTickCount64() - programStarted_) / 1000) > lastSec) {
		showFps = fps + 1;
		fps = 0;
		lastSec = (((unsigned int)GetTickCount64() - programStarted_) / 1000);
	}
	else {
		fps++;
	}
	return std::to_string(showFps);
}
