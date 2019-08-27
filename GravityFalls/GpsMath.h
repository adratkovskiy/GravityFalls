#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <string>
#include "structs.h"


const float MAJOR_AXIS = 6378137.0;
const float MINOR_AXIS = 6356752.3142;
const long double LNG_DEF_X = 33.033848;
const long double LAT_DEF_Y = 68.952440;
const long double GPS_TO_M_LNG_X_COOF = 40096.65798;
const long double GPS_TO_M_LAT_Y_COOF = -111533.579171;
const long double M_TO_PX_COOF = 50;
const float MAJOR_AXIS_POW_2 = pow(MAJOR_AXIS, 2);
const float MINOR_AXIS_POW_2 = pow(MINOR_AXIS, 2);

using namespace std;

class GpsMath
{
public:
	GpsMath();
	long double toDegMin(long double gpsCoord);
	long double deg2Rad(long double gpsCoord);
	long double getDistanceBetween2Points();
	long double getTrueAngle(Gps_Point gpsPoint);
	long double getPointRadius(Gps_Point gpsPoint, long double trueAngle);
	gpsXY pxToGps(pointXY point);
	void setGpsMain(long double latitude, long double longitude);
	void setGpsTarget(long double latitude, long double longitude);

private:
	Gps_Point gpsMain_;
	Gps_Point gpsTarget_;
	gpsXY gpsToM_;
	gpsXY mToGps_;
	gpsXY pxToGps_;
	gpsXY gpsToPx_;
};

