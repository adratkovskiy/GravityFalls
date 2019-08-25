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