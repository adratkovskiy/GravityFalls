﻿#include <iostream>
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
const float MOVE_SPEED = 2;
const int TILE_SIZE = 1000;
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

	ScreenText* boatCoordText = new ScreenText(12, { 255, 255, 255 }, renderer, 0, 0);
	ScreenText* cursorCoordText = new ScreenText(12, { 255, 255, 255 }, renderer, 0, 20);
	ScreenText* screenScrollText = new ScreenText(12, { 255, 255, 255 }, renderer, 0, 40);
	ScreenText* sysTimeText = new ScreenText(12, { 255, 255, 255 }, renderer, SCREEN_WIDTH - 200, 0);
	AppState* aState = new AppState(MOVE_SPEED);
	Images* backgroundAtlas = new Images(DEF_IMG_FOLDER, "back.png", renderer);
	SDL_Rect tmpCoords = { 0, 0, TILE_SIZE, TILE_SIZE };
	Picture* background = new Picture(tmpCoords, backgroundAtlas, "background", aState);
	background->setCoordsOnWindow(0, SCREEN_HEIGHT - tmpCoords.h);
	tmpCoords = { 1001, 0, 27, 86 };
	Picture* boat = new Picture(tmpCoords, backgroundAtlas, "ship", aState);
	boat->setCoordsOnWindow(SCREEN_WIDTH / 2 - tmpCoords.w / 2, SCREEN_HEIGHT / 2 - tmpCoords.h / 2);
	tmpCoords = { 1029, 0, 62, 62 };
	Picture* targetToGo = new Picture(tmpCoords, backgroundAtlas, "targetToGo", aState);
	SDL_Event e;
	SDL_Rect r;
	int x = 0;
	int y = 0;
	r.x = x;
	r.y = y;

	bool run = true;
	pointXY screenScrollOld;

	while (run) {
		pointXY screenScroll = aState->getScreenScroll();
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
					aState->setScreenScrollStart({ e.motion.x - screenScroll.x, e.motion.y - screenScroll.y });
					screenScrollOld = { screenScroll.x , screenScroll.y };
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if ((screenScrollOld.x == screenScroll.x) && (screenScrollOld.y == screenScroll.y)) {
						aState->setTarget(e.motion.x - screenScroll.x, e.motion.y - screenScroll.y);
					}
					aState->setScreenScrollActive(false);
				}
				break;
			}
		}
		if (aState->getScreenScrollActive()) {
			aState->setScreenScroll({ e.motion.x - aState->getScreenScrollStart().x, e.motion.y - aState->getScreenScrollStart().y });
		}
		r.x = x;
		r.y = y;
		int x_max, x_min, y_max, y_min;
		SDL_RenderClear(renderer);
		if (screenScroll.x * (-1) <= 0) {
			x_min = (screenScroll.x * (-1) / TILE_SIZE) - 1;
			x_max = 0;
		}
		else {
			x_min = 0;
			x_max = (screenScroll.x * (-1) / TILE_SIZE) + 1;
		}
		if (screenScroll.y * (-1) <= 0) {
			y_min = (screenScroll.y * (-1) / TILE_SIZE) - 1;
			y_max = 0;
		}
		else {
			y_min = 0;
			y_max = (screenScroll.y * (-1) / TILE_SIZE) + 1;
		}
		int sum = 0;
		for (int x = x_min; x <= x_max; x++) {
			for (int y = y_min; y <= y_max; y++) {
				//std::cout << x * TILE_SIZE << std::endl;
				background->drawPic({ x * TILE_SIZE, y * TILE_SIZE });
				sum++;
			}
		}
		std::cout << sum << std::endl;
		if (aState->getShipMove()) {
			pointXY pointTarget = aState->getTarget();
			pointXY pointWH = targetToGo->getWidthHeight();
			targetToGo->setCoordsOnWindow(pointTarget.x - pointWH.x / 2, pointTarget.y - pointWH.y / 2);
			targetToGo->drawPic();

			pointXY boatCoord = boat->getSelfCenter();
			if ((pointTarget.x == boatCoord.x) && (pointTarget.y == boatCoord.y)) {
				aState->setShipMove(false);
			}
			else {
				boat->movePic();
			}
		}
		boat->drawPic();
		boatCoordText->drawText(std::string("Boat X:") + std::to_string(boat->getSelfCenter().x) + std::string(" Y:") + std::to_string(boat->getSelfCenter().y));
		cursorCoordText->drawText(std::string("Cursor X:") + std::to_string(aState->getScreenScrollStart().x) + std::string(" Y:") + std::to_string(e.motion.y));
		screenScrollText->drawText(std::string("Screen Scroll X:") + std::to_string(screenScroll.x) + std::string(" Y:") + std::to_string(screenScroll.y));
		sysTimeText->drawText("time: " + timer->getWorkTimeText() + " sec, FPS: " + timer->getFps());
		SDL_RenderPresent(renderer);
		aState->createApp();
	}

	return 0;
}

