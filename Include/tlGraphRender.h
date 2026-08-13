/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述   
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#pragma once
#include <windows.h>
#include "tlGraphDefines.h"

class TL_API ID2Coord
{
public:
    //--ID2Coord(void);
    virtual ~ID2Coord(void) {};

    virtual void SetWindowRgn(long left,long top,long right,long bottom)= 0;
    virtual void SetViewportOrg(double orgx,double orgy)= 0;
    virtual void OffsetViewOrg(double osx,double osy)= 0;
    virtual void SetDrawLimit(double dbLimitX,double dbLimitY)= 0;
    virtual void SetMarkLimit(double dbLimitX,double dbLimitY)= 0;
    virtual void ZoomOut(double dbzoom=1.1)= 0; //--缩小
    virtual void ZoomIn(double dbzoom=0.9)= 0;  //--放大
    virtual void ZoomAll(void)= 0;
    virtual double GetFactor(void)const = 0;
    virtual void mm2pix(double mLength, int &pLength)= 0;
    virtual void mm2pix(double mx, double my, int &px, int &py)= 0;
    virtual void mm2pix(double mx, double my, long &px, long &py)= 0;
    virtual void pix2mm(int pLength,double &mLength)= 0;
    virtual void pix2mm(int px, int py, double &mx, double &my)= 0;
    virtual void pix2mm(long px, long py, double &mx, double &my)= 0;
};

class TL_API IGraphRender
{
public:
    //--IGraphRender(CWnd* pParentWnd);
    virtual ~IGraphRender() { };
    enum {
        emGR_CMD_AXIS=1,
        emGR_CMD_ACROSS,
        emGR_CMD_GRID,
        emGR_CMD_ARROW
    };
public:
    virtual void InitRender(void *pVWnd) = 0;
    virtual void BeginRender(void) = 0;
    virtual void FinishRender(void) = 0;
    virtual ID2Coord *GetCoord(void) = 0;
    virtual void BeginPaint(int nPenStyle, int nWidth, COLORREF crColor) = 0;
	virtual void SetBrush(CBrush * pb) = 0;   /*! 必须在BeginPaint之后， EndPaint之前调用 */
	virtual void SetTextStyle(COLORREF clr, CFont * pf = NULL) = 0;  /*! 必须在BeginPaint之后， EndPaint之前调用 */
	virtual void EndPaint(void) = 0;


    /*virtual void SetAxisVisible(bool IsVisible) = 0;
    virtual void SetAcrossVisible(bool IsVisible) = 0;
    virtual void SetGridVisible(bool IsVisible) = 0;*/
    virtual int32_t GetRenderCtrl(int iCmd, void *pArg) = 0;
    virtual int32_t SetRenderCtrl(int iCmd, void *pArg) = 0;
public:
    virtual void MoveTo(double x, double y) = 0;
    virtual void LineTo(double x, double y) = 0;
    virtual void DrawDot(double x,double y) = 0;
    virtual void DrawLine(double x1, double y1, double x2, double y2) = 0;
    virtual void DrawPolyline(DPoint_t *lppt, uint32_t dwCount) = 0;
    virtual void DrawRectangle(double x1, double y1, double x2, double y2) = 0;
    virtual void DrawPolygon(DPoint_t *pData,int iSize, bool bFill = false) = 0;
    virtual void DrawEllipse(double x1, double y1, double x2, double y2) = 0;
    virtual void DrawCircle(double cx, double cy, double r) = 0;
    virtual void DrawArc(double cx, double cy, double r, double a1, double a2,bool reverse=false) = 0;
	virtual void TextOut(double x, double y, const char * s) = 0;
	virtual void DrawText(const char * s, const FField_t& f, uint32_t nFormat = DT_LEFT) = 0;
};

typedef enum {
    emRender_GDI=0,
    emRender_GDIPlus,
    emRender_D2D,
    emRender_OpenGL
}emRender_t;
class IRenderEngine
{
public:
    virtual IGraphRender *CreateRender(emRender_t emRender,void *pArg) = 0;
    virtual int32_t CloseRender(IGraphRender *pRender) = 0;
};



