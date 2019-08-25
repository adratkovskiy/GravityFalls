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