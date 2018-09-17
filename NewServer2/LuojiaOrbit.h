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

	int OribitQueryPerTime(char *fn, double *twodpos, double *begintime, int unit, int length); //参数txt路径，服务端调用时暂存的长数组，起始时间, 单位(年月日时分秒), 长度(只有秒使用double,其他都会转为int)
	
	int LuojiaOrbitQuery(char *fn, double *time, double *pos);
};

