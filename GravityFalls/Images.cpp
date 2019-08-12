#include "Images.h"

Images::Images(std::string defImgFolder, std::string fileName, SDL_Renderer* renderer)
{
	std::cout << "created atlas: " << defImgFolder << fileName << std::endl;
	renderer_ = renderer;
	fileName_ = defImgFolder + fileName;
	loadTexture();
}

void Images::loadTexture()
{
	texture_ = IMG_LoadTexture(renderer_, fileName_.c_str());
	if (!texture_)
	{
		std::cout << IMG_GetError();
	}
}

void Images::renderTexture(SDL_Rect coords, int targetX, int targetY)
{
	SDL_Rect dst;
	dst.x = targetX;
	dst.y = targetY + scrollScreen_;
	dst.w = coords.w;
	dst.h = coords.h;
	SDL_RenderCopy(renderer_, texture_, &coords, &dst);
}

void Images::renderTexture(SDL_Rect coords, int targetX, int targetY, double angle)
{
	SDL_Rect dst;
	dst.x = targetX;
	dst.y = targetY + scrollScreen_;
	dst.w = coords.w;
	dst.h = coords.h;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_RenderCopyEx(renderer_, texture_, &coords, &dst, angle, NULL, flip);
}

void Images::updScrollScreen(int scrollScreen)
{
	scrollScreen_ += scrollScreen;
}
