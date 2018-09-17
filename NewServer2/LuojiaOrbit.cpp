#include "stdafx.h"
#include "LuojiaOrbit.h"


LuojiaOrbit::LuojiaOrbit()
{
}


LuojiaOrbit::~LuojiaOrbit()
{
}

int LuojiaOrbit::OribitQueryPerTime(char *fn, double *twodpos, double *begintime, int unit, int length)
{
	if (unit > 6 || unit < 0)
		return -1;
	double temp_pos[3] = { 0.0 };
	for (int i = 0; i < length; i++)
	{
		begintime[unit] = i;
		int temp = LuojiaOrbitQuery(fn, begintime, temp_pos);
		if (temp != 0)
			return temp;
		twodpos[i * 3]		=	temp_pos[0];
		twodpos[i * 3 + 1]  =	temp_pos[1];
		twodpos[i * 3 + 2]  =	temp_pos[2];
	}

	return 0;
}

int LuojiaOrbit::LuojiaOrbitQuery(char *fn, double *time, double *pos)
{
	int fnlen;
	char orbfile[MAXFILE] = { 0 };
	char outfile[MAXFILE] = { 0 };
	FILE *forb = NULL;
	FILE *fout = NULL;
	double ro[3] = { 0.0 }, vo[3] = { 0.0 };
	double re[3] = { 0.0 };
	int i;
	//double pos[3] = { 0.0 };
	double R2D = 180 / pi;
	double  startmfe = 0.0, stopmef = 0.0, deltamin = 1;
	int yr, mon, day, hour, min;
	double jd, jdf;
	double sec;
	double tsince;
	char buf1[MAXFILE] = { 0 }, buf2[MAXFILE] = { 0 }, buf3[MAXFILE] = { 0 };
	gravconsttype  whichconst;
	elsetrec satrec;
	yr = (int)time[0];
	mon = (int)time[1];
	day = (int)time[2];
	hour = (int)time[3];
	min = (int)time[4];
	sec = time[5];
	OrbitFuncs::jday(yr, mon, day, hour, min, sec, jd, jdf);
	jd = jd + jdf;
	fnlen = strlen(fn);
	strncpy(outfile, fn, fnlen);
	strncpy(outfile + fnlen - 3, "out", 4);
	whichconst = wgs84;
	if (!(forb = fopen(fn, "r"))) {
		printf("Error: TLE file %s open error\n", orbfile);
		return 1;
	}
	if (!(fout = fopen(outfile, "w"))) {
		printf("Error: out file %s open error\n", orbfile);
		return 1;
	}
	fgets(buf1, MAXFILE, forb);
	fgets(buf2, MAXFILE, forb);
	fgets(buf3, MAXFILE, forb);
	fclose(forb);
	OrbitFuncs::twoline2rv(buf2, buf3, 'c', 'e', 'i', whichconst, startmfe, stopmef, deltamin, satrec);
	tsince = (jd - satrec.jdsatepoch) * 1440;
	OrbitFuncs::Orbit(satrec, tsince, ro, vo);
	TEME2ECEF(jd, ro, re);
	for (i = 0; i < 3; i++) {
		re[i] = re[i] * 1000;
	}
	ecef2pos(re, pos);
	pos[0] = pos[0] * R2D;
	pos[1] = pos[1] * R2D;
	fprintf(fout, "%4d %02d %02d %02d %02d %6.3f %18.4f %18.4f %18.4f\n", yr, mon, day, hour, min, sec, pos[0], pos[1], pos[2]);

	fclose(fout);
	return 0;
}