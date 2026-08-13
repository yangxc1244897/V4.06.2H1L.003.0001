
// stdafx.cpp : 只包括标准包含文件的源文件
// TeteFactory.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

static char g_sCurrPath[MAX_PATH];
const char *GetSystemDir(void)
{
	char sPath[MAX_PATH];
	GetModuleFileNameA(NULL, sPath, MAX_PATH);
	char sdrive[_MAX_DRIVE];
	char sdir[_MAX_DIR];
	char sname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(sPath, sdrive, sdir, sname, ext);
	strcpy(g_sCurrPath, sdrive);
	strcat(g_sCurrPath, sdir);
	return g_sCurrPath;
}
const char *GetSystemDirFile(const char *sFile)
{
	char sPath[MAX_PATH];
	GetModuleFileNameA(NULL, sPath, MAX_PATH);
	char sdrive[_MAX_DRIVE];
	char sdir[_MAX_DIR];
	char sname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(sPath, sdrive, sdir, sname, ext);
	strcpy(g_sCurrPath, sdrive);
	strcat(g_sCurrPath, sdir);
	strcat(g_sCurrPath, sFile);
	return g_sCurrPath;
}
double CalAngle(double da, double db)
{
	double Angle;

	if (fabs(da)<0.01)
	{
		if (db>0)
		{
			Angle = TETE_PI / 2;
		}
		else
		{
			Angle = 3 * TETE_PI / 2;
		}
	}
	else
	{
		Angle = atan(db / da);
		if (da<0)
		{ 	//  To ensure the angle is between 0 and 2*M_PI
			Angle += TETE_PI;
		}
		if (Angle<0)
			Angle += 2 * TETE_PI;
	}

	return Angle;
}
void GetArcInfo(DPoint_t dp1, DPoint_t dp2, DPoint_t dp3, stArc_t &stArc)
{
	double a, b, c, d, e, f;
	a = dp1.x - dp2.x;
	b = dp1.y - dp2.y;
	c = dp1.x - dp3.x;
	d = dp1.y - dp3.y;
	e = (dp1.x*dp1.x - dp2.x*dp2.x + dp1.y*dp1.y - dp2.y*dp2.y) / 2.0;
	f = (dp1.x*dp1.x - dp3.x*dp3.x + dp1.y*dp1.y - dp3.y*dp3.y) / 2.0;
	stArc.cx = (b*f - d*e) / (b*c - a*d);
	stArc.cy = (c*e - a*f) / (b*c - a*d);
	stArc.radiusx = sqrt((dp1.x - stArc.cx)*(dp1.x - stArc.cx) + (dp1.y - stArc.cy)*(dp1.y - stArc.cy));
	stArc.radiusy = stArc.radiusx;

	double a1 = CalAngle(dp1.x - stArc.cx, dp1.y - stArc.cy);
	double a2 = CalAngle(dp2.x - stArc.cx, dp2.y - stArc.cy);
	double a3 = CalAngle(dp3.x - stArc.cx, dp3.y - stArc.cy);
	if (a2<a1)
		a2 += 2 * TETE_PI;
	if (a3<a1) {
		a3 += 2 * TETE_PI;
	}
	if (a2>a3)
	{
		stArc.isCloclWise = false;
		stArc.angle1 = a1;
		stArc.angle2 = a2;
	}
	else
	{
		stArc.isCloclWise = false;
		stArc.angle1 = a2;
		stArc.angle2 = a1 + TETE_PI*2.0;
	}
}

void DoEvent(void)
{
	static MSG msg;
	if (0 != PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

