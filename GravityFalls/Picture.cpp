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
	if (getAngle() == 0.0f) {
		atlas_->renderTexture(coordsFromAtlas_, coordsOnWindow_.x, coordsOnWindow_.y + aState_->getScreenScroll());
	}
	else {
		drawPic(getAngle());
	}
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

double Picture::getAngle()
{
	return angle_;
}

void Picture::setCoordsOnWindow(int coordX, int coordY)
{
	coordsOnWindow_.x = coordX;
	coordsOnWindow_.y = coordY;
}

void Picture::setAngle(double angle)
{
	angle_ = angle;
}

pointXY Picture::getWidthHeight()
{
	return { coordsFromAtlas_.w ,  coordsFromAtlas_.h };
}

void Picture::movePic()
{
	int moveSpeed = aState_->getMoveSpeed();
	pointXY moveVector;
	pointXY targetCoord = aState_->getTarget();
	pointXYFloat selfCoord;
	if ((coordsFloat_.x == 0.0f) && (coordsFloat_.y == 0.0f)) {
		selfCoord.x = getSelfCenter().x;
		selfCoord.y = getSelfCenter().y;
	}
	else
	{
		selfCoord = coordsFloat_;
	}
	moveVector.x = targetCoord.x - selfCoord.x;
	moveVector.y = targetCoord.y - selfCoord.y;
	pointXYFloat diff = normalize(moveVector);
	pointXYFloat defVec = normalize({ 0, 1 });
	float angle = acos(diff.x * defVec.x + diff.y * defVec.y) * 180 / PI + 180;
	setAngle(angle);
	std::cout << angle << std::endl;
	diff.x *= moveSpeed;
	diff.y *= moveSpeed;
	if (abs(targetCoord.x - selfCoord.x) + abs(targetCoord.y - selfCoord.y) <= moveSpeed)
	{
		setCoordsOnWindow((targetCoord.x - coordsOnWindow_.w / 2), (targetCoord.y - coordsOnWindow_.h / 2 ));
		aState_->setShipMove(false);
	}
	else {
		coordsFloat_.x = selfCoord.x + diff.x;
		coordsFloat_.y = selfCoord.y + diff.y;
		setCoordsOnWindow(selfCoord.x - coordsOnWindow_.w / 2 + diff.x, selfCoord.y - coordsOnWindow_.h / 2 + diff.y);
	}
	
}

pointXYFloat Picture::normalize(pointXY coords)
{
	float len = vecLen(coords);
	pointXYFloat b;
	b.x = coords.x / len;
	b.y = coords.y / len;
	return {b.x, b.y};
}

float Picture::vecLen(pointXY a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

