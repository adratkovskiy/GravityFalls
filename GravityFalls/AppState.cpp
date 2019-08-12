#include "AppState.h"

#include "AppState.h"

AppState::AppState()
{

}

void AppState::setScreenScroll(int screenScroll)
{
	screenScrollTarget_ = screenScroll + 1;
	screenScrollStart_ = screenScroll_;
	std::cout << "scroll: " << screenScrollTarget_ << std::endl;
	enableScreenMove_ = true;
}

int AppState::getScreenScroll()
{
	return screenScroll_;
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
	screenScroll_ = screenScrollStart_ + (screenScrollTarget_ - screenScrollStart_) * sin(screenAngleTik_);
	if (screenScroll_ >= screenScrollTarget_ - 1) {
		enableScreenMove_ = false;
		screenAngleTik_ = 0;
	}
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
	shipMove = true;
	targetX_ = x;
	targetY_ = y;
}

pointXY AppState::getTarget()
{
	return { targetX_, targetY_ };
}

bool AppState::toShipMove()
{
	return shipMove;
}
