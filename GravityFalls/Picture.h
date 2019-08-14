#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <iostream>

#include "Images.h"
#include "AppState.h"
#include "structs.h"

#define PI 3.14159265

class Picture
{
public:
	Picture(SDL_Rect coordsFromAtlas, Images* atlas, std::string picname, AppState* aState);
	void drawPic();
	void drawPic(double angle);
	void drawPic(SDL_Rect boxTargetCoord, Images* boxTargetatlas);
	float Picture::vecLen(pointXY a);
	void movePic();
	pointXYFloat normalize(pointXY coords);

	pointXY getWidthHeight();
	SDL_Rect getCoordsOnWindow();
	pointXY getSelfCenter();
	double getAngle();
	
	void setCoordsOnWindow(int coordX, int coordY);
	void setAngle(double angle);
	
private:
	SDL_Rect coordsFromAtlas_;
	SDL_Rect coordsOnWindow_;
	pointXYFloat coordsFloat_ = { 0.0f, 0.0f };
	Images* atlas_;
	int scrollScreen_ = 0;
	AppState* aState_ = nullptr;
	double angle_ = 0.0f;
};

