#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Images.h"
#include "AppState.h"
#include "Picture.h"

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 640;
const int TILE_SIZE = 64;
const std::string DEF_IMG_FOLDER = "img/";

SDL_Window* win = nullptr;
SDL_Surface* scr = nullptr;
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

	scr = SDL_GetWindowSurface(win);

	return 0;
}


int main(int argc, char** args) 
{
	if (init() == 1) {
		return 1;
	}

	AppState* aState = new AppState();

	Images* backgroundAtlas = new Images(DEF_IMG_FOLDER, "back.png", renderer);

	SDL_Rect backgroundCoords;
	backgroundCoords.x = 0;
	backgroundCoords.y = 0;
	backgroundCoords.w = 1000;
	backgroundCoords.h = 1000;
	Picture* background = new Picture(backgroundCoords, backgroundAtlas, "backgroundBottom", aState);
	background->setCoordsOnWindow(0, SCREEN_HEIGHT - backgroundCoords.h);

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
				e.motion.x; // Координаты мыши 
				e.motion.y; // Координаты мыши 
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (aState->getEnableDrop()) {
						std::cout << "drop: true" << std::endl;
						//ballonCoords = ballon->getCoordsOnWindow();
						//blockHouse.push_back(houseBoxes(defHouseCoords, elementsAtlas, "defHouse", gState, ballonCoords.x + ballonCoords.w / 2 - defHouseCoords.w / 2, ballonCoords.y + ballonCoords.h - 15, boxId++, 25));
						aState->setEnableDrop(false);
						std::cout << "drop: false" << std::endl;
					}
				}

				break;
			}
		}
		r.x = x;
		r.y = y;
		SDL_RenderClear(renderer);

		background->drawPic();
		/*std::vector<Picture>::iterator backIter = backgroundBlueScreenVec.begin();
		int backgroundBlueScreenNum = 0;
		for (backIter = backgroundBlueScreenVec.begin(); backIter != backgroundBlueScreenVec.end(); ++backIter)
		{
			backIter->setCoordsOnWindow(0, SCREEN_HEIGHT - backgroundBottomCoords.h - backgroundBlueScreenCoords.h - backgroundBlueScreenCoords.h * backgroundBlueScreenNum);
			backIter->drawPic();
			backgroundBlueScreenNum++;
		}
		if (backgroundBlueScreenCoords.h * backgroundBlueScreenNum < gState->getScreenScroll()) {
			backgroundBlueScreenVec.push_back(Picture(backgroundBlueScreenCoords, backgroundAtlas, "backgroundBlueScreen", gState));
			std::cout << "add new blueScreen " << backgroundBlueScreenNum << std::endl;
		}
		int topCoord = SCREEN_HEIGHT - backgroundBottomCoords.h - backgroundBlueScreenCoords.h - backgroundBlueScreenCoords.h;
		if (topCoord + gState->getScreenScroll() > 0) {
			topCoord = 0 - gState->getScreenScroll();
		}
		backgroundTop->setCoordsOnWindow(0, topCoord);
		backgroundTop->drawPic();

		for (picIter = blockHouse.begin(); picIter != blockHouse.end(); ++picIter)
		{
			picIter->drawDroppedPic(blockHouse, SCREEN_HEIGHT);
		}
		ballon->moveBallon();
		if (gState->getEnableScreenMove()) {
			gState->nextScreenScrollTik();
		}*/

		SDL_RenderPresent(renderer);
		aState->createApp();
		//gState->moveTower();
	}

	return 0;
}

