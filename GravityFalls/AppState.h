#pragma once
#include <iostream>
#include "structs.h"

class AppState
{
public:
	AppState();
	void setScreenScroll(int screenScroll);
	int getScreenScroll();
	void nextScreenScrollTik();
	bool getEnableScreenMove();
	void toScreenScroll();
	void createApp();
	void setTarget(int x, int y);
	pointXY getTarget();
	bool toShipMove();

private:
	bool shipMove = false;
	int screenScroll_ = 0;
	float screenAngleTik_ = 0.0f;
	bool enableScreenMove_ = false;
	int screenScrollTarget_ = 0;
	int screenScrollStart_ = 0;
	bool appNotStarted_ = true;
	float towerAngle_ = 0.0f;
	int targetX_;
	int targetY_;
};

