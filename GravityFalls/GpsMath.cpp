#include "GpsMath.h"

GpsMath::GpsMath()
{
	GPS.latitude_c = GPS_Point.latitude_c = 'N';
	GPS.longitude_c = GPS_Point.longitude_c = 'E';
}

double GpsMath::gps_convert_to_rad(unsigned long int GPS_DATA, char c)
{
	double rad;

	rad = (double)gps_convert_to_grad(GPS_DATA) * M_PI / 1000000 / 180;
	if ((c == 'S') | (c == 'W'))
		rad = -1 * rad;
	return rad;
}

unsigned long int GpsMath::gps_convert_to_grad(unsigned long int GPS_DATA)
{
	unsigned long int grad, min;
	grad = (GPS_DATA / 1000000) * 1000000;
	min = ((GPS_DATA - grad) * 100) / 60;
	return grad + min;
}

double GpsMath::gps_distance()
{
	double lat1_cos, lat2_cos, lat1_sin, lat2_sin;
	double lat1, long1, lat2, long2;
	double sin_delta_long, cos_delta_long;
	double y, x;

	lat1 = gps_convert_to_rad(GPS.latitude, GPS.latitude_c);
	long1 = gps_convert_to_rad(GPS.longitude, GPS.longitude_c);

	lat2 = gps_convert_to_rad(GPS_Point.latitude, GPS_Point.latitude_c); // Координаты точки
	long2 = gps_convert_to_rad(GPS_Point.longitude, GPS_Point.longitude_c);

	lat1_cos = cos(lat1);
	lat2_cos = cos(lat2);
	lat1_sin = sin(lat1);
	lat2_sin = sin(lat2);

	sin_delta_long = sin(long2 - long1);
	cos_delta_long = cos(long2 - long1);

	y = sqrt(pow(lat2_cos * sin_delta_long, 2) + pow(lat1_cos * lat2_sin - lat1_sin * lat2_cos * cos_delta_long, 2));
	x = lat1_sin * lat2_sin + lat1_cos * lat2_cos * cos_delta_long;

	return (atan2(y, x) * EATH_RADIUS) / 1000;
}

double GpsMath::gps_angle()
{
	double lat1, long1, lat2, long2;
	double dlon_W, dlon_E, dphi, atn2, dlon, tc;
	int sign;

	lat1 = gps_convert_to_rad(GPS.latitude, GPS.latitude_c);
	long1 = gps_convert_to_rad(GPS.longitude, GPS.longitude_c);

	lat2 = gps_convert_to_rad(GPS_Point.latitude, GPS_Point.latitude_c); // Координаты точки
	long2 = gps_convert_to_rad(GPS_Point.longitude, GPS_Point.longitude_c);

	dlon_W = (long2 - long1) - (2 * M_PI * (floor((long2 - long1) / (2 * M_PI))));
	dlon_E = (long1 - long2) - (2 * M_PI * (floor((long1 - long2) / (2 * M_PI))));
	dphi = log((tan((lat2 / 2) + (M_PI / 4))) / (tan((lat1 / 2) + (M_PI / 4))));

	if (dlon_W < dlon_E) {
		dlon_W = -1 * dlon_W;

		//get sign
		if (dlon_W >= 0)
			sign = 1;
		else
			sign = -1;

		if (abs(dlon_W) >= abs(dphi)) {
			atn2 = (sign * M_PI / 2) - atan(dphi / dlon_W);
		}
		else {
			if (dphi > 0) {
				atn2 = atan(dlon_W / dphi);
			}
			else {
				if ((-1 * dlon_W) >= 0) {
					atn2 = M_PI + atan(dlon_W / dphi);
				}
				else {
					atn2 = (-1 * M_PI) + atan(dlon_W / dphi);
				}
			}
		}
	}
	else {
		//get sign
		if (dlon_W >= 0)
			sign = 1;
		else
			sign = -1;

		if (abs(dlon_E) >= abs(dphi)) {
			if (dlon_E > 0)
				atn2 = sign * M_PI / 2 - atan(dphi / (dlon_E));
			else
				atn2 = 0;
		}
		else {
			if (dphi > 0) {
				atn2 = atan((dlon_E) / dphi);
			}
			else {
				if ((dlon_E) >= 0) {
					atn2 = M_PI + atan((dlon_E) / dphi);
				}
				else {
					atn2 = (-1 * M_PI) + atan((dlon_E) / dphi);
				}
			}
		}
		dlon = dlon_E;
	}

	tc = atn2 - (2 * M_PI * (floor((atn2) / (2 * M_PI))));
	return 360 - ((tc * 180) / M_PI);
}

void GpsMath::setGPS(unsigned long latitude, unsigned long longitude)
{
	GPS.latitude = latitude;
	GPS.longitude = longitude;
}

void GpsMath::setGPS_Point(unsigned long latitude, unsigned long longitude)
{
	GPS_Point.latitude = latitude;
	GPS_Point.longitude = longitude;
}
