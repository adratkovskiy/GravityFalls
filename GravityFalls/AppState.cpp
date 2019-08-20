#include "AppState.h"

#include "AppState.h"

AppState::AppState(float moveSpeed)
{
	moveSpeed_ = moveSpeed;
}


float AppState::getMoveSpeed()
{
	return moveSpeed_;
}

pointXY AppState::getScreenScroll()
{
	return screenScroll_;
}

bool AppState::getScreenScrollActive()
{
	return screenScrollActive_;
}

pointXY AppState::getScreenScrollStart()
{
	return screenScrollStart_;
}

void AppState::nextScreenScrollTik()
{
	screenAngleTik_ += 0.01f;
	toScreenScroll();
}

bool AppState::getEnableScreenMove()
{
	return enableScreenMove_;
}

void AppState::toScreenScroll()
{
	/*screenScroll_ = screenScrollStart_ + (screenScrollTarget_ - screenScrollStart_) * sin(screenAngleTik_);
	if (screenScroll_ >= screenScrollTarget_ - 1) {
		enableScreenMove_ = false;
		screenAngleTik_ = 0;
	}*/
}

void AppState::createApp()
{
	if (appNotStarted_) {
		appNotStarted_ = false;
		std::cout << "App started" << std::endl;
	}
}

void AppState::setTarget(int x, int y)
{
	shipMove_ = true;
	targetX_ = x;
	targetY_ = y;
}

pointXY AppState::getTarget()
{
	return { targetX_, targetY_ };
}

bool AppState::getShipMove()
{
	return shipMove_;
}

void AppState::setShipMove(bool stat)
{
	shipMove_ = stat;
}

void AppState::setScreenScroll(pointXY screenScroll)
{
	screenScroll_ = screenScroll;
}

void AppState::setScreenScrollActive(bool screenScrollActive)
{
	screenScrollActive_ = screenScrollActive;
}

void AppState::setScreenScrollStart(pointXY screenScrollStart)
{
	screenScrollStart_ = screenScrollStart;
	screenScrollActive_ = true;
}

/*void AppState::setTarget(Picture* target)
{
	target_ = target;
}*/
