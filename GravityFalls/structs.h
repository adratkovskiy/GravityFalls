#pragma once

struct pointXY
{
	int x;
	int y;
};

struct pointXYFloat
{
	double x;
	double y;
};

struct pointXYGeo
{
	long double x;
	long double y;
};

typedef struct {
	long double   latitude;       // �������
	char            latitude_c;     // ������� ������
	long double   longitude;      // ������
	char            longitude_c;    // ������ ������
	long double	altitude;		// ������
} Gps_Point;

struct gpsXY
{
	long double x;
	long double y;
};