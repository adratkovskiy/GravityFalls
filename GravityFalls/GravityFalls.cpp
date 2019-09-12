#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <winsock2.h>
#include <assert.h>
#include <vector>
#include <SFML/Window.hpp>

#include "Images.h"
#include "AppState.h"
#include "Picture.h"
#include "ScreenText.h"
#include "WorkTimer.h"
#include "structs.h"
#include "Socket.h"
#include "Address.h"
#include "GpsMath.h"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const float MOVE_SPEED = 2;
const int TILE_SIZE = 1000;
const int MAX_COORD_POINTS = 100;
const string DEF_IMG_FOLDER = "img/";
const int DEF_FONT_SIZE = 12;
const int DEF_PORT = 8012;

SDL_Window* win = nullptr;
SDL_Renderer* renderer = nullptr;
GpsMath gps;
Socket sock;

void wait_seconds(float seconds)
{
	Sleep((int)(seconds * 1000.0f));
}

inline bool InitializeSockets()
{
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
}

inline void ShutdownSockets()
{
	WSACleanup();
}

pointXYGeo getCoordFromLine(string coordLine) {
	long double x = 0;
	long double y = 0;
	if ((coordLine[0] == ':') && (coordLine[coordLine.length() - 1] == '!'))
	{
		y = atof(coordLine.substr(1, 12).c_str());
		x = atof(coordLine.substr(14, 13).c_str());
	}
	return { x, y };
}

int init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Can't init: " << SDL_GetError() << endl;
		system("pause");
		return 1;
	}

	win = SDL_CreateWindow("Gi TestTask", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		cout << "Can't create window: " << SDL_GetError() << endl;
		system("pause");
		return 1;
	}

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		return 1;
	}

	if (TTF_Init() == -1)
	{
		cout << " Failed to initialize TTF : " << SDL_GetError() << endl;
		return 1;
	}

	if (!InitializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	printf("creating socket on port %d\n", DEF_PORT);
	if (!sock.Open(DEF_PORT))
	{
		printf("failed to create socket!\n");
		return 1;
	}

	return 0;
}

int SDL_main(int argc, char* argv[])
{
	if (init() == 1) {
		return 1;
	}

	WorkTimer* timer = new WorkTimer();
	ScreenText* boatCoordText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 0);
	ScreenText* boatCoordGPSText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 20);
	ScreenText* cursorCoordText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 40);
	ScreenText* cursorCoordGPSText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 60);
	ScreenText* screenScrollText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 80);
	ScreenText* gpsDistanceText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 100);
	ScreenText* pointCountText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 120);
	ScreenText* joyLeftText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 140);
	ScreenText* joyRightText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 160);
	ScreenText* sysTimeText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, SCREEN_WIDTH - 200, 0);
	AppState* aState = new AppState(MOVE_SPEED);
	aState->setScreenScroll({SCREEN_WIDTH / -2, SCREEN_HEIGHT / -2 });
	Images* backgroundAtlas = new Images(DEF_IMG_FOLDER, "back.png", renderer);
	SDL_Rect tmpCoords = { 0, 0, TILE_SIZE, TILE_SIZE };
	Picture* background = new Picture(tmpCoords, backgroundAtlas, "background", aState);
	background->setCoordsOnWindow(0, SCREEN_HEIGHT - tmpCoords.h);
	tmpCoords = { 1001, 0, 27, 86 };
	//1091, 0, 5, 5 // 1096, 0, 5, 5
	Picture* boat = new Picture(tmpCoords, backgroundAtlas, "boat", aState);
	boat->setCoordsOnWindow(0 - tmpCoords.w / 2, 0 - tmpCoords.h / 2);
	tmpCoords = { 1029, 0, 62, 62 };
	Picture* targetToGo = new Picture(tmpCoords, backgroundAtlas, "targetToGo", aState);
	tmpCoords = { 1091, 0, 7, 7 };
	Picture* blackCross = new Picture(tmpCoords, backgroundAtlas, "black cross", aState);
	tmpCoords = { 1098, 0, 7, 7 };
	Picture* redCross = new Picture(tmpCoords, backgroundAtlas, "yellow cross", aState);

	vector<pointXY> allPoints;

	SDL_Event e;
	SDL_Rect r;
	int x = 0;
	int y = 0;
	r.x = x;
	r.y = y;

	bool run = true;
	pointXY screenScrollOld;
	pointXY prevXY = { 0, 0 };
	pointXY joyLeft;
	pointXY joyRight;
	while (run) {
		sf::Joystick::update();
		joyLeft = { (int)round(sf::Joystick::getAxisPosition(0, sf::Joystick::X)), (int)round(sf::Joystick::getAxisPosition(0, sf::Joystick::Y)) };
		joyRight = { (int)round(sf::Joystick::getAxisPosition(0, sf::Joystick::R)), (int)round(sf::Joystick::getAxisPosition(0, sf::Joystick::Z)) };
		pointXY screenScroll = aState->getScreenScroll();
		while (SDL_PollEvent(&e) != NULL) { // SDL hook buttons
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
					aState->setScreenScrollStart({ screenScroll.x + e.motion.x , screenScroll.y + e.motion.y });
					screenScrollOld = { screenScroll.x , screenScroll.y };
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if ((screenScrollOld.x == screenScroll.x) && (screenScrollOld.y == screenScroll.y)) {
						aState->setTarget(screenScroll.x + e.motion.x, screenScroll.y + e.motion.y);
					}
					aState->setScreenScrollActive(false);
				}
				break;
			}
		}  
		if (aState->getScreenScrollActive()) {
			aState->setScreenScroll({ aState->getScreenScrollStart().x - e.motion.x, aState->getScreenScrollStart().y - e.motion.y });
		}
		r.x = x;
		r.y = y;
		int x_max, x_min, y_max, y_min;
		SDL_RenderClear(renderer);
		if (screenScroll.x <= 0) {
			x_min = (screenScroll.x / TILE_SIZE) - 1;
			x_max = 0;
		}
		else {
			x_min = 0;
			x_max = (screenScroll.x / TILE_SIZE) + 1;
		}
		if (screenScroll.y <= 0) {
			y_min = (screenScroll.y / TILE_SIZE) - 1;
			y_max = 0;
		}
		else {
			y_min = 0;
			y_max = (screenScroll.y / TILE_SIZE) + 1;
		}
		int sum = 0;
		for (int x = x_min; x <= x_max; x++) {
			for (int y = y_min; y <= y_max; y++) {
				background->drawPic({ x * TILE_SIZE, y * TILE_SIZE });
			}
		}

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
		if ((prevXY.x == boat->getSelfCenter().x) && (prevXY.y == boat->getSelfCenter().y)) {

		}
		else {
			allPoints.push_back({ boat->getSelfCenter().x, boat->getSelfCenter().y });
			prevXY.x = boat->getSelfCenter().x;
			prevXY.y = boat->getSelfCenter().y;
		}
		
		boatCoordText->drawText(string("Boat X:") + to_string(boat->getSelfCenter().x) + string(" Y:") + to_string(boat->getSelfCenter().y));
		gpsXY boatCoordGPS = gps.pxToGps({boat->getSelfCenter().x, boat->getSelfCenter().y });
		gps.setGpsMain(boatCoordGPS.y, boatCoordGPS.x);
		boatCoordGPSText->drawText(string("Boat LNG:") + to_string(boatCoordGPS.x) + string(" LAT:") + to_string(boatCoordGPS.y));
		cursorCoordText->drawText(string("Cursor X:") + to_string(e.motion.x) + string(" Y:") + to_string(e.motion.y));
		gpsXY cursorCoordGPS = gps.pxToGps({ (e.motion.x + screenScroll.x) , (e.motion.y + screenScroll.y) });
		gps.setGpsTarget(cursorCoordGPS.y, cursorCoordGPS.x);
		cursorCoordGPSText->drawText(string("Cursor LNG:") + to_string(cursorCoordGPS.x) + string(" LAT:") + to_string(cursorCoordGPS.y));
		screenScrollText->drawText(string("Screen Scroll X:") + to_string(screenScroll.x) + string(" Y:") + to_string(screenScroll.y));
		sysTimeText->drawText("time: " + timer->getWorkTimeText() + " sec, FPS: " + timer->getFps());
		string distance = to_string(round(gps.getDistanceBetween2Points() * 100) / 100);
		gpsDistanceText->drawText("Distance: " + distance.substr(0, distance.length() - 4));
		joyLeftText->drawText("JoyLeft: " + to_string(joyLeft.x) + " " + to_string(joyLeft.y));
		joyRightText->drawText("JoyRight: " + to_string(joyRight.x) + " " + to_string(joyRight.y));
		int pointCount = 0;
		for (vector<pointXY>::iterator It = allPoints.begin(); It < allPoints.end(); It++)
		{
			if (It + 1 == allPoints.end()) {
				redCross->setCoordsOnWindow(It->x - redCross->getWidthHeight().x / 2, It->y - - redCross->getWidthHeight().y / 2);
				redCross->drawPic();
			}
			else
			{
				blackCross->setCoordsOnWindow(It->x - blackCross->getWidthHeight().x / 2, It->y - blackCross->getWidthHeight().y / 2);
				blackCross->drawPic();
			}
			pointCount++;
				
		}
		pointCountText->drawText("Gps point count:" + to_string(pointCount));

		SDL_RenderPresent(renderer);
		aState->createApp();

		while (true) // working the net
		{
			Address sender;
			unsigned char buffer[256];
			int bytes_read = sock.Receive(sender, buffer, sizeof(buffer));
			if (!bytes_read)
				break;
			string sName(reinterpret_cast<char*>(buffer));
			pointXYGeo geoCoord = getCoordFromLine(sName);
			pointXY toPx = gps.gpsToPx({ gps.toDegMin(geoCoord.x) , gps.toDegMin(geoCoord.y) });
			cout << to_string(gps.toDegMin(geoCoord.x)) << "," << to_string(gps.toDegMin(geoCoord.y)) << "(" << toPx.x << "," << toPx.y << ")"<< endl;
			boat->setCoordsOnWindow(toPx.x - boat->getWidthHeight().x / 2, toPx.y - boat->getWidthHeight().y / 2);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			cout << "left" << endl;
		}
	}
	return 0;
}
