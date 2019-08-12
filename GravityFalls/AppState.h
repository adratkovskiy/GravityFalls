#pragma once
#include <iostream>

class AppState
{
public:
	AppState();
	bool getEnableDrop();
	void setEnableDrop(bool enableDrop);
	void setScreenScroll(int screenScroll);
	int getScreenScroll();
	void nextScreenScrollTik();
	bool getEnableScreenMove();
	void toScreenScroll();
	void createApp();

private:
	bool enableDrop_ = false;
	int screenScroll_ = 0;
	float screenAngleTik_ = 0.0f;
	bool enableScreenMove_ = false;
	int screenScrollTarget_ = 0;
	int screenScrollStart_ = 0;
	bool appNotStarted_ = true;
	float towerAngle_ = 0.0f;
};

