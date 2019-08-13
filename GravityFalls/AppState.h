#pragma once
#include <iostream>
#include "structs.h"
//#include "Picture.h"

class AppState
{
public:
	AppState();
	void nextScreenScrollTik();
	void toScreenScroll();
	void createApp();
	
	bool getShipMove();
	pointXY getTarget();
	bool getEnableScreenMove();
	int getScreenScroll();

	void setScreenScroll(int screenScroll);
	void setTarget(int x, int y);
	void setShipMove(bool stat);
	//void setTarget(Picture* target);

private:
	bool shipMove = false;
	int screenScroll_ = 0;
	float screenAngleTik_ = 0.0f;
	bool enableScreenMove_ = false;
	int screenScrollTarget_ = 0;
	int screenScrollStart_ = 0;
	bool appNotStarted_ = true;
	float towerAngle_ = 0.0f;
	int targetX_ = 0;
	int targetY_ = 0;
	//Picture* target_ = nullptr;
};

