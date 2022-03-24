/*
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2015 John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
 
#define FREESTEAM_BUILDING_LIB
#include "steam_uv.h"
 
#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "b23.h"
#include "backwards.h"
#include "solver2.h"
#include "zeroin.h"
#include "steam_pT.h"
 
#include "common.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STEAM_UV_DEBUG
#ifdef STEAM_UV_DEBUG
# define MSG(FMT,...) fprintf(stderr,"%s:%d: " FMT "\n",__FILE__,__LINE__,__VA_ARGS__)
#else
# define MSG(...)
#endif

/**
	Iterate on pressure and temperature to solve for u given constant V
*/
SteamState freesteam_region1_set_uv(double ut, double vt){
	MSG("region 1, setting u = %f, v = %f",ut,vt);
	double T0 = 300;;
	double p0 = 1e5;

	double u0 = freesteam_region1_u_pT(T0, p0);
	double v0 = freesteam_region1_v_pT(T0, p0);

	double v = v0;
	double T = T0;
	double p = p0;
	double u = u0;

	double error = fabs((ut - u) / u0) + fabs((vt - v) / v0);
	//int attempts = 0;
	while (error > 1e-15){
		double T1 = T * (1.000001);
		double p1 = p;

		double T2 = T;
		double p2 = p * (1.000001);

		double u1 = freesteam_region1_u_pT(p1, T1);
		double v1 = freesteam_region1_v_pT(p1, T1);

		double u2 = freesteam_region1_u_pT(p2, T2);
		double v2 = freesteam_region1_v_pT(p2, T2);

		double dudt = (u1 - u) / (T1 - T);
		double dvdt = (v1 - v) / (T1 - T);

		double dudp = (u2 - u) / (p2 - p);
		double dvdp = (v2 - v) / (p2 - p);

		double dt = (ut - u - (vt - v)*dudp / dvdp) / (dudt - dudp*dvdt / dvdp);
		double dp = -dt *dvdt / dvdp + (vt - v) / dvdp;
		//double f = fabs((v2 - v) / v) / fabs((v1 - v) / v);

		double T3 = T + dt;
		double p3 = p + dp;
		double u3 = freesteam_region1_u_pT(p3, T3);
		double v3 = freesteam_region1_v_pT(p3, T3);

		double error1 = fabs((ut - u3) / u0) + fabs((vt - v3) / v0);
		v = v3;
		T = T3;
		p = p3;
		u = u3;
		error = error1;
		MSG("p = %f, T = %f, u = %f, v = %f -> err = %f",p,T,u,v,error1);
	}

	return freesteam_region1_set_pT(p, T);
}

SteamState freesteam_region3_set_uv(double ut, double vt) {
	MSG("region 3, setting u = %f, v = %f",ut,vt);
	double rho = 1 / vt;
	/* one-way iteration on T to solve v(rho,T). */
	//double freesteam_region3_h_rhoT(double rho, double T);
	double T0 = 700;

	double u0 = freesteam_region3_u_rhoT(rho, T0);
	double u1 = u0;
	double T1 = T0*1.001;
	double error = 1;
	while(error > 1e-10){
		u1 = freesteam_region3_u_rhoT(rho, T1);
		error = fabs((u1 - ut) / ut);
		double dudt = (u1 - u0) / (T1 - T0);
		T0 = T1;
		u0 = u1;
		T1 = T0 + (ut - u1) / dudt;
	}

	SteamState S = freesteam_region3_set_rhoT(rho, T1);
	return S;
}

/**
	Iterate on pressure and temperature to solve for u given constant V
*/
SteamState freesteam_region2_set_uv(double ut, double vt){
	MSG("region 2, setting u = %f, v = %f",ut,vt);
	double T0 = 600;
	double p0 = 1e5;

	double v0 = freesteam_region2_v_pT(p0, T0);

	double v = v0;
	double T = T0;
	double p = p0;

	double u0 = freesteam_region2_u_pT(p0, T);
	double u = u0;
	double error = fabs((ut - u) / u0) + fabs((vt - v) / v0);
	//int attempts = 0;
	while (error > 1e-15){
		double T1 = T * (1.000001);
		double p1 = p;

		double T2 = T;
		double p2 = p * (1.000001);

		double u1 = freesteam_region2_u_pT(p1, T1);
		double v1 = freesteam_region2_v_pT(p1, T1);

		double u2 = freesteam_region2_u_pT(p2, T2);
		double v2 = freesteam_region2_v_pT(p2, T2);

		double dudt = (u1 - u) / (T1 - T);
		double dvdt = (v1 - v) / (T1 - T);

		double dudp = (u2 - u) / (p2 - p);
		double dvdp = (v2 - v) / (p2 - p);

		double dt = (ut - u - (vt - v)*dudp / dvdp) / (dudt - dudp*dvdt / dvdp);
		double dp = -dt *dvdt / dvdp + (vt - v) / dvdp;
		//double f = fabs((v2 - v) / v) / fabs((v1 - v) / v);

		double T3 = T + dt;
		double p3 = p + dp;
		double u3 = freesteam_region2_u_pT(p3, T3);
		double v3 = freesteam_region2_v_pT(p3, T3);
		double error1 = fabs((ut - u3) / u0) + fabs((vt - v3) / v0);

		v = v3;
		T = T3;
		p = p3;
		u = u3;
		error = error1;
	}
	return freesteam_region2_set_pT(p, T);
}


double u_23(double vt){
	double T0 = 800;
	double p0 = freesteam_b23_p_T(T0);
	double v0 = freesteam_region2_v_pT(p0, T0);

	double T = T0;
	double T1 = T*1.01;
	//double p = p0;
	double v = v0;
	double u;
	double error = 1;
	while(error > 1e-10){
		double p1 = freesteam_b23_p_T(T1);
		double v1 = freesteam_region2_v_pT(p1, T1);
		u = freesteam_region2_u_pT(p1, T1);
		double dvdt = (v1 - v) / (T1 - T);
		error = fabs((vt - v) / vt);
		T = T1;
		v = v1;
		T1 = T + (vt - v1) / dvdt;
	}
	return u;
}

int freesteam_bounds_uv(double u, double v, int verbose){

#define BOUND_WARN(MSG) \
	if(verbose){\
		fprintf(stderr,"%s (%s:%d): WARNING " MSG " (u = %g kJ/kg, v = %g m3/kg)\n"\
		,__func__,__FILE__,__LINE__,u/1e3,v);\
	}

	BOUND_WARN("not yet implemented");
	return 0;
}


int freesteam_region_uv(double u, double v) {
	const double u_crit0 = 1874291.9078304442;

	/*
	FIXME some more code is needed to implement all the required boundaries correctly
	in accordance with IAPWS-IF97.
	for b13, see http://sourceforge.net/p/freesteam/code/HEAD/tree/tags/freesteam-0.8.1/b13curve.h
	for b23, see http://sourceforge.net/p/freesteam/code/HEAD/tree/tags/freesteam-0.8.1/b23curve.h
	for sat curves see http://sourceforge.net/p/freesteam/code/HEAD/tree/tags/freesteam-0.8.1/satcurve.h
	*/

	const double vf = 0.0022461503950500787; /* FIXME this is not a constant, should be vf(uf(T3) < u < u_crit) */
	const double u13 = 1599556.2219496202; /* FIXME this is not a constant */
	const double ug = 2205006.2923036958; /* FIXME this is not a constant! */
	const double v2 = 0.0043355076532487760; /* this is vmax_b23 */

	// TODO check that u > some minimum value (need ptriple(v)...)
	if(u < u_crit0){
		// FIXME XXX vf is not a constant value (need vf(u)...)
		if(v > vf) return 4;
		// FIXME XXX u13 is not a constant value (need u13(v)...)
		else if(u > u13) return 3;
		else return 1;
	}
	if(u < ug){
		// FIXME first we need to check if v > vcrit, this is missing
		return 4;
	// FIXME should also check v > vmin_b23 before evaluating u_23?
	}else if (v > v2 || u > u_23(v)){
		// FIXME this should be if(v
		return 2;
	}else{
		return 3;
	}
	/* TODO what does it mean to return 0?? */
	return 0;
}


SteamState freesteam_set_uv(double u, double v){
	MSG("u = %f, v = %f",u,v);
	int region = freesteam_region_uv(u, v);
	MSG("region = %d",region);
	switch(region){
	case 1:
		return freesteam_region1_set_uv(u, v);
		break;
	case 2:
		return freesteam_region2_set_uv(u, v);
		break;
	case 3:
		return freesteam_region3_set_uv(u, v);
	default:
		MSG("Region %d not implemented!",region);
		// no 4 or 5
		// XXX FIXME why not???
		// FIXME what if we got a 0 from region_uv?
		return freesteam_region3_set_uv(u, v);
	}
}

