/**
  ******************************************************************************
  * @模块 
  * @作者 jchq
  * @版本 V1.00.00
  * @日期 2019-03-21
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @功能描述
  * 
  *   
  *
  *
  *
  ******************************************************************************
  */

#include "stdafx.h"
#include "CoordTransform.h"

CCoordTransform* CCoordTransform::_instance = NULL;
//---------------------------------------------------------------------------
CCoordTransform *CCoordTransform::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CCoordTransform();
    }
    return _instance;
}

CCoordTransform::CCoordTransform()
{
}
CCoordTransform::~CCoordTransform()
{
}
void CCoordTransform::GetHeadCoord(int32_t iIndex, xCoord_t &xCoord)const
{
    if ((iIndex < 0) || (iIndex >= MAX_HEAD_NUM))
        return;
    xCoord = m_xHeadCoord[iIndex];
}
void CCoordTransform::SetHeadCoord(int32_t iIndex, const xCoord_t &xCoord)
{
    if ((iIndex < 0) || (iIndex >= MAX_HEAD_NUM))
        return;
    m_xHeadCoord[iIndex] = xCoord;
}

void CCoordTransform::GetHeadTurn(int32_t iIndex, xHeadTurn_t &xTurn)const
{
	if ((iIndex < 0) || (iIndex >= MAX_HEAD_NUM))
		return;
	xTurn = m_xHeadTurn[iIndex];
}

void CCoordTransform::SetHeadTurn(int32_t iIndex, const xHeadTurn_t &xTurn)
{
	if ((iIndex < 0) || (iIndex >= MAX_HEAD_NUM))
		return;
	m_xHeadTurn[iIndex] = xTurn;
}

void CCoordTransform::Base2Head(int32_t iIndex, double &x, double &y)
{
    if ((iIndex < 0) || (iIndex >= MAX_HEAD_NUM))
        return;

	//// 振镜坐标调整，平移+旋转
	//x += m_xHeadTurn[iIndex].dx;
	//y += m_xHeadTurn[iIndex].dy;

	//if (!D_EQUAL(m_xHeadTurn[iIndex].angle, 0))
	//{
	//	double dbRad = DEG2RAD(m_xHeadTurn[iIndex].angle);
	//	double dx = x - m_xHeadCoord[iIndex].cx, dy = y - m_xHeadCoord[iIndex].cy;
	//	x = dx*cos(dbRad) - dy*sin(dbRad) + m_xHeadCoord[iIndex].cx;
	//	y = dx*sin(dbRad) + dy*cos(dbRad) + m_xHeadCoord[iIndex].cy;
	//}


	// 图形坐标转振镜坐标
	x = x - m_xHeadCoord[iIndex].cx;
	y = y - m_xHeadCoord[iIndex].cy;

	// 坐标极性变换
    if (m_xHeadCoord[iIndex].option & 0x0001)
    {
        double t = x;
        x = y;
        y = t;
    }
    if (m_xHeadCoord[iIndex].option & 0x0002)
    {
        x = -x;
    }
    if (m_xHeadCoord[iIndex].option & 0x0004)
    {
        y = -y;
    }
}
void CCoordTransform::Head2Base(int32_t iIndex, double &x, double &y)
{
    if ((iIndex < 0) || (iIndex >= MAX_HEAD_NUM))
        return;
    double tx(x), ty(y);
    if (m_xHeadCoord[iIndex].option & 0x0001)
    {
        tx = y;
        ty = x;
    }
    if (m_xHeadCoord[iIndex].option & 0x0002)
    {
        tx = -tx;
    }
    if (m_xHeadCoord[iIndex].option & 0x0004)
    {
        ty = -ty;
    }
    x = tx + m_xHeadCoord[iIndex].cx;
    y = ty + m_xHeadCoord[iIndex].cy;
}
bool CCoordTransform::IsInHead(int32_t iIndex, double x, double y)
{
    if ((x < m_xHeadCoord[iIndex].minx()) || (x > m_xHeadCoord[iIndex].maxx()))
        return false;
    if ((y < m_xHeadCoord[iIndex].miny()) || (y > m_xHeadCoord[iIndex].maxy()))
        return false;
    return true;
}
bool CCoordTransform::IsInHead(int32_t iIndex, double x1, double y1, double x2, double y2)
{
    if ((x1 < m_xHeadCoord[iIndex].minx()) || (x1 > m_xHeadCoord[iIndex].maxx()))
        return false;
    if ((y1 < m_xHeadCoord[iIndex].miny()) || (y1 > m_xHeadCoord[iIndex].maxy()))
        return false;
    if ((x2 < m_xHeadCoord[iIndex].minx()) || (x2 > m_xHeadCoord[iIndex].maxx()))
        return false;
    if ((y2 < m_xHeadCoord[iIndex].miny()) || (y2 > m_xHeadCoord[iIndex].maxy()))
        return false;
    return true;
}
void CCoordTransform::GetVisionCoord(int32_t iIndex, xCoord_t &xCoord)const
{
    if ((iIndex < 0) || (iIndex >= MAX_VISION_NUM))
        return;
    xCoord = m_xVisionCoord[iIndex];
}
void CCoordTransform::SetVisionCoord(int32_t iIndex, const xCoord_t &xCoord)
{
    if ((iIndex < 0) || (iIndex >= MAX_VISION_NUM))
        return;
    m_xVisionCoord[iIndex] = xCoord;
}
void CCoordTransform::Base2Vision(int32_t iIndex, double &x, double &y)
{
    if ((iIndex < 0) || (iIndex >= MAX_VISION_NUM))
        return;
    x = x - m_xVisionCoord[iIndex].cx;
    y = y - m_xVisionCoord[iIndex].cy;
    if (m_xVisionCoord[iIndex].option & 0x0001)
    {
        double t = x;
        x = y;
        y = t;
    }
    if (m_xVisionCoord[iIndex].option & 0x0002)
    {
        x = -x;
    }
    if (m_xVisionCoord[iIndex].option & 0x0004)
    {
        y = -y;
    }
}
void CCoordTransform::Vision2Base(int32_t iIndex, double &x, double &y)
{
    if ((iIndex < 0) || (iIndex >= MAX_VISION_NUM))
        return;
    double tx(x), ty(y);
    if (m_xVisionCoord[iIndex].option & 0x0001)
    {
        tx = y;
        ty = x;
    }
    if (m_xVisionCoord[iIndex].option & 0x0002)
    {
        tx = -tx;
    }
    if (m_xVisionCoord[iIndex].option & 0x0004)
    {
        ty = -ty;
    }
    x = tx + m_xVisionCoord[iIndex].cx;
    y = ty + m_xVisionCoord[iIndex].cy;
}
bool CCoordTransform::IsInVision(int32_t iIndex, double x, double y)
{
    if ((x < m_xVisionCoord[iIndex].minx()) || (x > m_xVisionCoord[iIndex].maxx()))
        return false;
    if ((y < m_xVisionCoord[iIndex].miny()) || (y > m_xVisionCoord[iIndex].maxy()))
        return false;
    return true;
}
bool CCoordTransform::IsInVision(int32_t iIndex, double x1, double y1, double x2, double y2)
{
    if ((x1 < m_xVisionCoord[iIndex].minx()) || (x1 > m_xVisionCoord[iIndex].maxx()))
        return false;
    if ((y1 < m_xVisionCoord[iIndex].miny()) || (y1 > m_xVisionCoord[iIndex].maxy()))
        return false;
    if ((x2 < m_xVisionCoord[iIndex].minx()) || (x2 > m_xVisionCoord[iIndex].maxx()))
        return false;
    if ((y2 < m_xVisionCoord[iIndex].miny()) || (y2 > m_xVisionCoord[iIndex].maxy()))
        return false;
    return true;
}
void CCoordTransform::GetMotionCoord(int32_t iIndex, xCoord_t &xCoord)const
{
    if ((iIndex < 0) || (iIndex >= MAX_MOTION_NUM))
        return;
    xCoord = m_xMotionCoord[iIndex];
}
void CCoordTransform::SetMotionCoord(int32_t iIndex, const xCoord_t &xCoord)
{
    if ((iIndex < 0) || (iIndex >= MAX_MOTION_NUM))
        return;
    m_xMotionCoord[iIndex] = xCoord;
}
void CCoordTransform::Base2Motion(int32_t iIndex, double &x, double &y)
{
    if ((iIndex < 0) || (iIndex >= MAX_MOTION_NUM))
        return;
    x = x - m_xMotionCoord[iIndex].cx;
    y = y - m_xMotionCoord[iIndex].cy;
    if (m_xMotionCoord[iIndex].option & 0x0001)
    {
        double t = x;
        x = y;
        y = t;
    }
    if (m_xMotionCoord[iIndex].option & 0x0002)
    {
        x = -x;
    }
    if (m_xMotionCoord[iIndex].option & 0x0004)
    {
        y = -y;
    }
}
void CCoordTransform::Motion2Base(int32_t iIndex, double &x, double &y)
{
    if ((iIndex < 0) || (iIndex >= MAX_MOTION_NUM))
        return;
    double tx(x), ty(y);
    if (m_xMotionCoord[iIndex].option & 0x0001)
    {
        tx = y;
        ty = x;
    }
    if (m_xMotionCoord[iIndex].option & 0x0002)
    {
        tx = -tx;
    }
    if (m_xMotionCoord[iIndex].option & 0x0004)
    {
        ty = -ty;
    }
    x = tx + m_xMotionCoord[iIndex].cx;
    y = ty + m_xMotionCoord[iIndex].cy;
}
bool CCoordTransform::IsInMotion(int32_t iIndex, double x, double y)
{
    if ((x < m_xMotionCoord[iIndex].minx()) || (x > m_xMotionCoord[iIndex].maxx()))
        return false;
    if ((y < m_xMotionCoord[iIndex].miny()) || (y > m_xMotionCoord[iIndex].maxy()))
        return false;
    return true;
}
bool CCoordTransform::IsInMotion(int32_t iIndex, double x1, double y1, double x2, double y2)
{
    if ((x1 < m_xMotionCoord[iIndex].minx()) || (x1 > m_xMotionCoord[iIndex].maxx()))
        return false;
    if ((y1 < m_xMotionCoord[iIndex].miny()) || (y1 > m_xMotionCoord[iIndex].maxy()))
        return false;
    if ((x2 < m_xMotionCoord[iIndex].minx()) || (x2 > m_xMotionCoord[iIndex].maxx()))
        return false;
    if ((y2 < m_xMotionCoord[iIndex].miny()) || (y2 > m_xMotionCoord[iIndex].maxy()))
        return false;
    return true;
}