#include "Picture.h"

Picture::Picture(SDL_Rect coordsFromAtlas, Images* atlas, std::string picname, AppState* aState)
{
	coordsFromAtlas_ = coordsFromAtlas;
	atlas_ = atlas;
	coordsOnWindow_.w = coordsFromAtlas.w;
	coordsOnWindow_.h = coordsFromAtlas.h;
	std::cout << "created picture: " << picname << " ("
		<< coordsOnWindow_.x << " " << coordsOnWindow_.y << ")"
		<< std::endl;
	aState_ = aState;
}

void Picture::drawPic()
{
	atlas_->renderTexture(coordsFromAtlas_, coordsOnWindow_.x, coordsOnWindow_.y + aState_->getScreenScroll());
}

void Picture::drawPic(double angle)
{
	atlas_->renderTexture(coordsFromAtlas_, coordsOnWindow_.x, coordsOnWindow_.y + aState_->getScreenScroll(), angle);
}

void Picture::drawPic(SDL_Rect boxTargetCoord, Images* boxTargetatlas)
{
	atlas_->renderTexture(coordsFromAtlas_, coordsOnWindow_.x, coordsOnWindow_.y + aState_->getScreenScroll());
	boxTargetatlas->renderTexture(boxTargetCoord, coordsOnWindow_.x + coordsOnWindow_.w / 2 - boxTargetCoord.w / 2, coordsOnWindow_.y + coordsOnWindow_.h - 15 + aState_->getScreenScroll());
}

SDL_Rect Picture::getCoordsOnWindow()
{
	return coordsOnWindow_;
}

pointXY Picture::getSelfCenter()
{
	SDL_Rect coordRect = getCoordsOnWindow();
	return pointXY({coordRect.x + coordRect.w/2, coordRect.y + coordRect.h / 2 });
}

void Picture::setCoordsOnWindow(int coordX, int coordY)
{
	coordsOnWindow_.x = coordX;
	coordsOnWindow_.y = coordY;
}

pointXY Picture::getWidthHeight()
{
	return { coordsFromAtlas_.w ,  coordsFromAtlas_.h };
}

void Picture::movePic()
{
	std::cout << "move" << std::endl;
	pointXY c;
	pointXY b = aState_->getTarget();
	pointXY selfCoord = getSelfCenter();
	c.x = b.x - selfCoord.x;
	c.y = b.y - selfCoord.y;
	pointXY diff = normalize(c);
	setCoordsOnWindow(diff.x * 2, diff.y * 2);
}

pointXY Picture::normalize(pointXY coords)
{
	float len = vecLen(coords);
	pointXY b;
	b.x = coords.x / len;
	b.y = coords.y / len;
	return b;
}

float Picture::vecLen(pointXY a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

