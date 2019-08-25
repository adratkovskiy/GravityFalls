#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <winsock2.h>
#include <assert.h>

#include "Images.h"
#include "AppState.h"
#include "Picture.h"
#include "ScreenText.h"
#include "WorkTimer.h"
#include "structs.h"
#include "Socket.h"
#include "Address.h"
#include "GpsMath.h"

//#pragma comment( lib, "ws2_32.lib" )


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const float MOVE_SPEED = 2;
const int TILE_SIZE = 1000;
const long double DEF_LAT_Y = 57.1466547; //68°57.1466547
const long double DEF_LNG_X = 02.0312438; //33°02.0312438
const long double PX_TO_LAT_Y = -0.000013311634;
const long double PX_TO_LNG_X = 0.000037065027;
const std::string DEF_IMG_FOLDER = "img/";
const int DEF_FONT_SIZE = 12;

SDL_Window* win = nullptr;
SDL_Renderer* renderer = nullptr;
GpsMath gps;

using namespace std;

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
	//cout << coordLine << "(" << coordLine[0] << ", " << coordLine[coordLine.length() - 1] << "): ";
	if ((coordLine[0] == ':') && (coordLine[coordLine.length() - 1] == '!'))
	{
		y = atof(coordLine.substr(3, 10).c_str());
		x = atof(coordLine.substr(17, 10).c_str());
	}
	return { x, y };
}

int SDL_main(int argc, char* argv[])
{
	if (!InitializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	int port = 8012;
	printf("creating socket on port %d\n", port);
	Socket socket;
	if (!socket.Open(port))
	{
		printf("failed to create socket!\n");
		return 1;
	}
	
	WorkTimer* timer = new WorkTimer();
	if (init() == 1) {
		return 1;
	}

	ScreenText* boatCoordText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 0);
	ScreenText* boatCoordGPSText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 20);
	ScreenText* cursorCoordText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 40);
	ScreenText* cursorCoordGPSText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 60);
	ScreenText* screenScrollText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 80);
	ScreenText* gpsDistanceText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, 0, 100);
	ScreenText* sysTimeText = new ScreenText(DEF_FONT_SIZE, { 255, 255, 255 }, renderer, SCREEN_WIDTH - 200, 0);
	AppState* aState = new AppState(MOVE_SPEED);
	aState->setScreenScroll({SCREEN_WIDTH / -2, SCREEN_HEIGHT / -2 });
	Images* backgroundAtlas = new Images(DEF_IMG_FOLDER, "back.png", renderer);
	SDL_Rect tmpCoords = { 0, 0, TILE_SIZE, TILE_SIZE };
	Picture* background = new Picture(tmpCoords, backgroundAtlas, "background", aState);
	background->setCoordsOnWindow(0, SCREEN_HEIGHT - tmpCoords.h);
	tmpCoords = { 1001, 0, 27, 86 };
	Picture* boat = new Picture(tmpCoords, backgroundAtlas, "boat", aState);
	boat->setCoordsOnWindow(0 - tmpCoords.w / 2, 0 - tmpCoords.h / 2);
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
		boatCoordText->drawText(std::string("Boat X:") + std::to_string(boat->getSelfCenter().x) + std::string(" Y:") + std::to_string(boat->getSelfCenter().y));
		boatCoordGPSText->drawText(std::string("Boat LNG: 33°") + std::to_string(boat->getSelfCenter().x * PX_TO_LNG_X + DEF_LNG_X) + std::string(" LAT: 68°") + std::to_string(boat->getSelfCenter().y * PX_TO_LAT_Y + DEF_LAT_Y));
		cursorCoordText->drawText(std::string("Cursor X:") + std::to_string(e.motion.x) + std::string(" Y:") + std::to_string(e.motion.y));
		cursorCoordGPSText->drawText(std::string("Cursor LNG: 33°:") + std::to_string((e.motion.x - screenScroll.x) * PX_TO_LNG_X + DEF_LNG_X) + std::string(" LAT: 68°") + std::to_string((e.motion.y - screenScroll.y) * PX_TO_LAT_Y + DEF_LAT_Y));
		screenScrollText->drawText(std::string("Screen Scroll X:") + std::to_string(screenScroll.x) + std::string(" Y:") + std::to_string(screenScroll.y));
		sysTimeText->drawText("time: " + timer->getWorkTimeText() + " sec, FPS: " + timer->getFps());
		//gps.setGPS(03302.0312282, 01.1466542);
		//gps.setGPS_Point(03302.0312282, 89.1466542);
		/*Lat: 6857.1466542 Long: 03302.0312282 Alt: 17.2614
		Lat: 6857.1466550 Long: 03302.0312277 Alt: 17.2673*/
		int degrees = 68;
		double minutes = 57.1466542;
		double mydegrees = degrees * 1.0 + (minutes / 60.0);
		mydegrees = (mydegrees * 1000000.0) / 1000000.0;
		gpsDistanceText->drawText("Distance: " + std::to_string(mydegrees));
		
		SDL_RenderPresent(renderer);
		aState->createApp();
		
		//cout << "dist:" <<  << endl;
		while (true)
		{
			Address sender;
			unsigned char buffer[256];
			int bytes_read = socket.Receive(sender, buffer, sizeof(buffer));
			if (!bytes_read)
				break;
			//printf("received packet from %d.%d.%d.%d:%d (%d bytes)\n", sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(), sender.GetPort(), bytes_read);
			//cout << buffer << endl;
			string sName(reinterpret_cast<char*>(buffer));
			pointXYGeo geoCoord = getCoordFromLine(sName);
			if ((geoCoord.x != 0) && (geoCoord.y != 0)) {

				int x_coord = (geoCoord.x - DEF_LNG_X) / PX_TO_LNG_X;
				int y_coord = (geoCoord.y - DEF_LAT_Y) / PX_TO_LAT_Y;
				//boat->setCoordsOnWindow(x_coord - boat->getWidthHeight().x / 2, y_coord - boat->getWidthHeight().y / 2);
				cout << "x:" << x_coord << " y:" << y_coord << endl;
			}
		}
	}
	//closesocket(socket);
	return 0;
}

/*Lat: 6857.1466542 Long: 03302.0312282 Alt: 17.2614
Lat: 6857.1466550 Long: 03302.0312277 Alt: 17.2673*/