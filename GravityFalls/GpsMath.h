#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <string>
#include "structs.h"


const float MAJOR_AXIS = 6378137.0;
const float MINOR_AXIS = 6356752.3142;
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

private:
	Gps_Point gpsMain_;
	Gps_Point gpsTarget_;
};

