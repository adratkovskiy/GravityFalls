#pragma once
#include <iostream>
#include "structs.h"
//#include "Picture.h"

class AppState
{
public:
	AppState(float moveSpeed);
	void nextScreenScrollTik();
	void toScreenScroll();
	void createApp();
	bool getShipMove();
	pointXY getTarget();
	bool getEnableScreenMove();
	float getMoveSpeed();
	pointXY getScreenScroll();
	bool getScreenScrollActive();
	pointXY getScreenScrollStart();

	void setTarget(int x, int y);
	void setShipMove(bool stat);
	void setScreenScroll(pointXY screenScroll);
	void setScreenScrollActive(bool screenScrollActive);
	void setScreenScrollStart(pointXY screenScrollStart);

private:
	bool shipMove_ = false;
	float screenAngleTik_ = 0.0f;
	bool enableScreenMove_ = false;
	bool appNotStarted_ = true;
	float towerAngle_ = 0.0f;
	int targetX_ = 0;
	int targetY_ = 0;
	float moveSpeed_;
	bool screenScrollActive_ = false;
	pointXY screenScroll_ = { 0,0 };
	pointXY screenScrollStart_ = { 0,0 };
};

