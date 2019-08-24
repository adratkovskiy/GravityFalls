#pragma once
#define _USE_MATH_DEFINES
#define EATH_RADIUS 6372795

#include <cmath>
#include <iostream>





typedef struct {
	unsigned long   latitude;       // �������
	char            latitude_c;     // ������� ������
	unsigned long   longitude;      // ������
	char            longitude_c;    // ������ ������
} tpGPG_Point;

typedef	struct
{
	unsigned long	latitude; 		// ������
	char			latitude_c; 	// ������ ������
	unsigned long	longitude;		// ������� 
	char			longitude_c;	// ������� ������
	uint8_t			satels;			// ���������� ���������
	double			altitude;		// ������ ��� ������� ����
	double			geodeff;		// ����������� ��������
	double			Time;			// �����
	uint8_t			Day;			// ����
	uint8_t			Month;			// �����
	unsigned int	Year;			// ���
	double			course;			// ����������� ����� � ��������
	double			speed;			// ��������
	uint8_t			actual;			// GPS ������ ����� �������� � ������ ���������� ������
} tpGPG;

tpGPG	GPS;

class GpsMath
{
public:
	double gps_convert_to_rad(unsigned long int GPS_DATA, char c);
	unsigned long int gps_convert_to_grad(unsigned long int GPS_DATA);
	double gps_distance();
	double gps_angle();
private:
	tpGPG_Point GPS_Point;
};

