#include "GpsMath.h"

GpsMath::GpsMath()
{
	gpsMain_.latitude_c = 'N';
	gpsTarget_.latitude_c = 'N';
	gpsMain_.longitude_c = 'E';
	gpsTarget_.longitude_c = 'E';
	gpsMain_.altitude = 0;
	gpsTarget_.altitude = 0;
	/*gpsMain_.latitude = toDegMin(6860.1466542);
	gpsMain_.longitude = toDegMin(03304.0312282);
	gpsTarget_.latitude = toDegMin(6858.1466542);
	gpsTarget_.longitude = toDegMin(03302.0312282);*/
}

long double GpsMath::toDegMin(long double gpsCoord)
{
	string tmpStr = to_string(gpsCoord);
	int dot = tmpStr.find('.');
	string tmpDegrees = tmpStr.substr(0, dot - 2);
	string tmpMinutes = tmpStr.substr(dot - 2);
	int degrees = atof(tmpDegrees.c_str());
	long double minutes = atof(tmpMinutes.c_str());
	long double mydegrees = degrees * 1.0 + (minutes / 60.0);
	mydegrees = (mydegrees * 1000000.0) / 1000000.0;
	return mydegrees;
}

long double GpsMath::deg2Rad(long double gpsCoord)
{
	return gpsCoord * 4.0 * atan(1.0) / 180.0;
}

long double GpsMath::getDistanceBetween2Points()
{
	long double trueAngle1 = getTrueAngle(gpsMain_);
	long double trueAngle2 = getTrueAngle(gpsTarget_);
	long double pointRadius1 = getPointRadius(gpsMain_, trueAngle1);
	long double pointRadius2 = getPointRadius(gpsTarget_, trueAngle2);
	long double earthPoint1_x = pointRadius1 * cos(deg2Rad(trueAngle1));
	long double earthPoint1_y = pointRadius1 * sin(deg2Rad(trueAngle1));
	long double earthPoint2_x = pointRadius2 * cos(deg2Rad(trueAngle2));
	long double earthPoint2_y = pointRadius2 * sin(deg2Rad(trueAngle2));
	long double x = sqrt(pow((earthPoint1_x - earthPoint2_x), 2) + pow((earthPoint1_y - earthPoint2_y), 2));
	long double y = M_PI * ((earthPoint1_x + earthPoint2_x) / 360) * (gpsMain_.longitude - gpsTarget_.longitude);
	return sqrt(pow(x, 2) + pow(y, 2));
}

long double GpsMath::getTrueAngle(Gps_Point gpsPoint)
{
	return atan(((MINOR_AXIS_POW_2 / MAJOR_AXIS_POW_2) * tan(deg2Rad(gpsPoint.latitude)))) * 180 / M_PI;
}

long double GpsMath::getPointRadius(Gps_Point gpsPoint, long double trueAngle)
{
	return (1 / sqrt((pow(cos(deg2Rad(trueAngle)), 2) / MAJOR_AXIS_POW_2) + (pow(sin(deg2Rad(trueAngle)), 2) / MINOR_AXIS_POW_2))) + gpsPoint.altitude;
}

void GpsMath::setGpsMain(long double latitude, long double longitude)
{
	gpsMain_.latitude = latitude;
	gpsMain_.longitude = longitude;
}

void GpsMath::setGpsTarget(long double latitude, long double longitude)
{
	gpsTarget_.latitude = latitude;
	gpsTarget_.longitude = longitude;
}

