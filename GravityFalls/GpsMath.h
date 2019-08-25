#pragma once
#define _USE_MATH_DEFINES
#define EATH_RADIUS 6372795

#include <cmath>
#include <iostream>
#include "structs.h"






class GpsMath
{
public:
	GpsMath();
	double gps_convert_to_rad(unsigned long int GPS_DATA, char c);
	unsigned long int gps_convert_to_grad(unsigned long int GPS_DATA);
	double gps_distance();
	double gps_angle();
	void setGPS(unsigned long latitude, unsigned long longitude);
	void setGPS_Point(unsigned long latitude, unsigned long longitude);
private:
	tpGPG_Point GPS_Point;
	tpGPG GPS;
};

