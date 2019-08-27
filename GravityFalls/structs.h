#pragma once

struct pointXY
{
	int x;
	int y;
};

struct pointXYFloat
{
	float x;
	float y;
};

struct pointXYGeo
{
	long double x;
	long double y;
};

typedef struct {
	long double   latitude;       // Долгота
	char            latitude_c;     // Долгота литера
	long double   longitude;      // Широта
	char            longitude_c;    // Широта литера
	long double	altitude;		// Высота
} Gps_Point;

struct gpsXY
{
	long double x;
	long double y;
};