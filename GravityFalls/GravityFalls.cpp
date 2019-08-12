#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Images.h"
#include "AppState.h"
#include "Picture.h"
#include "ScreenText.h"
#include "WorkTimer.h"
#include "structs.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int TILE_SIZE = 64;
const std::string DEF_IMG_FOLDER = "img/";

SDL_Window* win = nullptr;
SDL_Renderer* renderer = nullptr;

int init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Can't init: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	win = SDL_CreateWindow("Gi TestTask", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		std::cout << "Can't create window: " << SDL_GetError() << std::endl;
		system("pause");
		return 1;
	}

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	if (TTF_Init() == -1)
	{
		std::cout << " Failed to initialize TTF : " << SDL_GetError() << std::endl;
		return 1;
	}

	return 0;
}


int SDL_main(int argc, char* argv[])
{
	WorkTimer* timer = new WorkTimer();
	if (init() == 1) {
		return 1;
	}


	ScreenText* simpleText = new ScreenText(12, { 255, 255, 255 }, renderer, 0, 0);
	ScreenText* sysTime = new ScreenText(12, { 255, 255, 255 }, renderer, SCREEN_WIDTH - 200, 0);
	AppState* aState = new AppState();
	Images* backgroundAtlas = new Images(DEF_IMG_FOLDER, "back.png", renderer);

	SDL_Rect tmpCoords = {0, 0, 1000, 1000};
	Picture* background = new Picture(tmpCoords, backgroundAtlas, "background", aState);
	background->setCoordsOnWindow(0, SCREEN_HEIGHT - tmpCoords.h);
	tmpCoords = {1001, 0, 27, 86};
	Picture * ship = new Picture(tmpCoords, backgroundAtlas, "ship", aState);
	ship->setCoordsOnWindow(SCREEN_WIDTH / 2 - tmpCoords.w / 2, SCREEN_HEIGHT / 2 - tmpCoords.h / 2);
	tmpCoords = { 1029, 0, 62, 62 };
	Picture* targetToGo = new Picture(tmpCoords, backgroundAtlas, "targetToGo", aState);
	SDL_Event e;
	SDL_Rect r;
	int x = 0;
	int y = 0;
	r.x = x;
	r.y = y;

	bool run = true;

	while (run) {
		while (SDL_PollEvent(&e) != NULL) {
			if (e.type == SDL_QUIT) {
				run = false;
			}
			switch (e.type) {
			case SDL_MOUSEMOTION:
				e.motion.x; 
				e.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					pointXY point = targetToGo->getWidthHeight();
					aState->setTarget(e.motion.x - point.x/2, e.motion.y - point.y / 2);

				}
				break;
			}
		}
		r.x = x;
		r.y = y;
		SDL_RenderClear(renderer);

		background->drawPic();
		if (aState->toShipMove()) {
			pointXY point = aState->getTarget();
			targetToGo->setCoordsOnWindow(point.x, point.y);
			targetToGo->drawPic();
		}
		ship->drawPic();

		simpleText->drawText(std::string("X: ") + std::to_string(ship->getCoordsOnWindow().x) + std::string(" Y: ") + std::to_string(ship->getCoordsOnWindow().y));
		sysTime->drawText("time: " + timer->getWorkTimeText() + " sec, FPS: " + timer->getFps());
		SDL_RenderPresent(renderer);
		aState->createApp();
	}

	return 0;
}

