#pragma once
#include "Orbit.h"
#define MAXFILE		256
#define pi			3.14159265358979323846
#define lj_year		0
#define lj_month	1
#define lj_day		2
#define lj_hour		3
#define lj_second	4


class LuojiaOrbit
{
public:
	LuojiaOrbit();
	~LuojiaOrbit();

	int OribitQueryPerTime(char *fn, double *twodpos, double *begintime, int unit, int length); //����txt·��������˵���ʱ�ݴ�ĳ����飬��ʼʱ��, ��λ(������ʱ����), ����(ֻ����ʹ��double,��������תΪint)
	
	int LuojiaOrbitQuery(char *fn, double *time, double *pos);
};

