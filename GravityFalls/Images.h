#pragma once
#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Images
{
public:
	Images(std::string defImgFolder,
		std::string fileName,
		SDL_Renderer* renderer);
	void renderTexture(SDL_Rect coords, int targetX, int targetY);
	void renderTexture(SDL_Rect coords, int targetX, int targetY, double angle);
	void updScrollScreen(int scrollScreen);

private:
	void loadTexture();
	std::string fileName_;
	SDL_Renderer* renderer_ = nullptr;
	SDL_Texture* texture_ = nullptr;
	SDL_Rect imgSource_;
	SDL_Rect imgLocation_;
	int scrollScreen_;
};

