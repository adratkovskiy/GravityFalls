#pragma once
#define _USE_MATH_DEFINES
#define EATH_RADIUS 6372795

#include <cmath>
#include <iostream>





typedef struct {
	unsigned long   latitude;       // Долгота
	char            latitude_c;     // Долгота литера
	unsigned long   longitude;      // Широта
	char            longitude_c;    // Широта литера
} tpGPG_Point;

typedef	struct
{
	unsigned long	latitude; 		// Широта
	char			latitude_c; 	// Широта литера
	unsigned long	longitude;		// Долгота 
	char			longitude_c;	// Долгота литера
	uint8_t			satels;			// Количество спутников
	double			altitude;		// Высота над уровнем моря
	double			geodeff;		// Геоидальное различие
	double			Time;			// Время
	uint8_t			Day;			// День
	uint8_t			Month;			// Месяц
	unsigned int	Year;			// Год
	double			course;			// Направление курса в градусах
	double			speed;			// Скорость
	uint8_t			actual;			// GPS модуль нашел спутники и выдает актуальные данные
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

