#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class ScreenText
{
public:
	ScreenText(int fontSize, SDL_Color fontColor, SDL_Renderer* renderer, int posX, int posY);
	void drawText(std::string text);
private:
	TTF_Font* font_ = nullptr;
	SDL_Renderer* renderer_;
	SDL_Color color_;
	int posX_;
	int posY_;
};

