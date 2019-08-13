#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <iostream>

#include "Images.h"
#include "AppState.h"
#include "structs.h"


class Picture
{
public:
	Picture(SDL_Rect coordsFromAtlas, Images* atlas, std::string picname, AppState* aState);
	void drawPic();
	void drawPic(double angle);
	void drawPic(SDL_Rect boxTargetCoord, Images* boxTargetatlas);
	SDL_Rect getCoordsOnWindow();
	pointXY getSelfCenter();
	float Picture::vecLen(pointXY a);
	void setCoordsOnWindow(int coordX, int coordY);
	pointXY getWidthHeight();
	void movePic();
	pointXY normalize(pointXY coords);
private:
	SDL_Rect coordsFromAtlas_;
	SDL_Rect coordsOnWindow_;
	Images* atlas_;
	int scrollScreen_ = 0;
	AppState* aState_ = nullptr;
};

