#ifndef _Orbith_
#define _Orbith_


#pragma once

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>


// -------------------------- structure declarations ----------------------------
typedef enum
{
	wgs72old,
	wgs72,
	wgs84
} gravconsttype;

typedef struct elsetrec
{
	long int  satnum;
	int       epochyr, epochtynumrev;
	int       error;
	char      operationmode;
	char      init, method;

	/* Near Earth */
	int    isimp;
	double aycof, con41, cc1, cc4, cc5, d2, d3, d4,
		delmo, eta, argpdot, omgcof, sinmao, t, t2cof, t3cof,
		t4cof, t5cof, x1mth2, x7thm1, mdot, nodedot, xlcof, xmcof,
		nodecf;

	/* Deep Space */
	int    irez;
	double d2201, d2211, d3210, d3222, d4410, d4422, d5220, d5232,
		d5421, d5433, dedt, del1, del2, del3, didt, dmdt,
		dnodt, domdt, e3, ee2, peo, pgho, pho, pinco,
		plo, se2, se3, sgh2, sgh3, sgh4, sh2, sh3,
		si2, si3, sl2, sl3, sl4, gsto, xfact, xgh2,
		xgh3, xgh4, xh2, xh3, xi2, xi3, xl2, xl3,
		xl4, xlamo, zmol, zmos, atime, xli, xni;

	double a, altp, alta, epochdays, jdsatepoch, jdsatepochF, nddot, ndot,
		bstar, rcse, inclo, nodeo, ecco, argpo, mo, no_kozai;
	// Orbitfix add new variables from tle
	char  classification, intldesg[11];
	int   ephtype;
	long  elnum, revnum;
	// Orbitfix add unkozai'd variable
	double no_unkozai;
	// Orbitfix add singly averaged variables
	double am, em, im, Om, om, mm, nm;
	// Orbitfix add constant parameters to eliminate mutliple calls during execution
	double tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2;

	//       Additional elements to capture relevant TLE and object information:       
	long dia_mm; // RSO dia in mm
	double period_sec; // Period in seconds
	unsigned char active; // "Active S/C" flag (0=n, 1=y) 
	unsigned char not_orbital; // "Orbiting S/C" flag (0=n, 1=y)  
	double rcs_m2; // "RCS (m^2)" storage  

} elsetrec;

namespace OrbitFuncs
{

	//	public class OrbitClass
	//	{

	bool Orbitinit
		(
		gravconsttype whichconst, char opsmode, const int satn, const double epoch,
		const double xbstar, const double xndot, const double xnddot, const double xecco, const double xargpo,
		const double xinclo, const double xmo, const double xno,
		const double xnodeo, elsetrec& satrec
		);

	bool Orbit
		(
		// no longer need gravconsttype whichconst, all data contained in satrec
		elsetrec& satrec, double tsince,
		double r[3], double v[3]
		);

	void getgravconst
		(
		gravconsttype whichconst,
		double& tumin,
		double& mu,
		double& radiusearthkm,
		double& xke,
		double& j2,
		double& j3,
		double& j4,
		double& j3oj2
		);

	// older Orbitio methods
	void twoline2rv
		(
		char      longstr1[130], char longstr2[130],
		char      typerun, char typeinput, char opsmode,
		gravconsttype       whichconst,
		double& startmfe, double& stopmfe, double& deltamin,
		elsetrec& satrec
		);

	// older Orbitext methods
	double  gstime
		(
		double jdut1
		);

	double  sgn
		(
		double x
		);

	double  mag
		(
		double x[3]
		);

	void    cross
		(
		double vec1[3], double vec2[3], double outvec[3]
		);

	double  dot
		(
		double x[3], double y[3]
		);

	double  angle
		(
		double vec1[3],
		double vec2[3]
		);

	void    newtonnu
		(
		double ecc, double nu,
		double& e0, double& m
		);

	double  asinh
		(
		double xval
		);

	void    rv2coe
		(
		double r[3], double v[3], const double mu,
		double& p, double& a, double& ecc, double& incl, double& omega, double& argp,
		double& nu, double& m, double& arglat, double& truelon, double& lonper
		);

	void    jday
		(
		int year, int mon, int day, int hr, int minute, double sec,
		double& jd, double& jdFrac
		);

	void    days2mdhms
		(
		int year, double days,
		int& mon, int& day, int& hr, int& minute, double& sec
		);

	void    invjday
		(
		double jd, double jdFrac,
		int& year, int& mon, int& day,
		int& hr, int& minute, double& sec
		);


}  // namespace

void ecef2pos(const double *r, double *pos);
int TEME2ECEF(double jd_utc, double *rs, double *rse);
#endif
