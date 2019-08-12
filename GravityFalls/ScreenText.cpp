#include "ScreenText.h"

ScreenText::ScreenText(int fontSize, SDL_Color fontColor, SDL_Renderer* renderer, int posX, int posY)
{
	posX_ = posX;
	posY_ = posY;
	color_ = fontColor;
	renderer_ = renderer;
	font_ = TTF_OpenFont("sources/droid-sans-mono.ttf", fontSize);
	if (font_ == nullptr)
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
	}
	
}

void ScreenText::drawText(std::string text)
{
	const char* cstr = text.c_str();
	SDL_Surface* surface = TTF_RenderText_Solid(font_, cstr, color_);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { posX_, posY_, texW, texH };
	SDL_RenderCopy(renderer_, texture, NULL, &dstrect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
