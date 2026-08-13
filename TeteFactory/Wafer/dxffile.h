/*******************************************************************
*  Copyright(c) 2018 泰德激光
*  All rights reserved.
*
*  文件名称: dxffile.h
*  简要描述: 基于开源库dxflib的DXF文档解析类
*
*  创建日期: 2018-12-20
*  作者: hhhuang
*  说明: 目前能够支持解析的DXF图形类型有：点、直线、圆弧、圆、椭圆（不支持椭圆弧）、多段线、正多边形、矩形
         能够解析的DXF对象有：层对象（只做简单的区分，不涉及层的其他特性）、块对象（不支持块的阵列插入）

*  修改日期:
*  作者:
*  说明:

******************************************************************/

#pragma once

#include "dxfMatch.h"


class CDxfData : public DL_CreationAdapter {
public:
	CDxfData(IGraphObject * pObject);
	~CDxfData();
	void clear();

	// HEADER段中的变量
	// [10, 39]
	virtual void setVariableVector(const std::string&s, double x, double y, double z, int c);
	// [0, 9] 或 其余
	virtual void setVariableString(const std::string&, const std::string&, int);
	// [60, 99]
	virtual void setVariableInt(const std::string&, int, int);
	// [40, 59]
	virtual void setVariableDouble(const std::string&, double, int);
	
	virtual void addPoint(const DL_PointData&);
	virtual void addLine(const DL_LineData&);

	virtual void addArc(const DL_ArcData&);
	virtual void addCircle(const DL_CircleData&);
	virtual void addEllipse(const DL_EllipseData&);

	virtual void addPolyline(const DL_PolylineData&);
	virtual void addVertex(const DL_VertexData&);

	virtual void addSpline(const DL_SplineData&);
	virtual void addControlPoint(const DL_ControlPointData&);
	virtual void addFitPoint(const DL_FitPointData&);
	virtual void addKnot(const DL_KnotData&);

	virtual void addLayer(const DL_LayerData&);

	virtual void addBlock(const DL_BlockData&);
	virtual void endBlock();

	virtual void addInsert(const DL_InsertData&);

	virtual void endEntity();

	double GetFactor();

protected:
	IGraphObject * m_pObject;    // 对象
	bool m_bPolyline;   // 是否是折线
	bool m_bSpline;     // 是否是样条曲线

	bool m_bBlock;    // 是否是块结构中的实体对象
	std::vector<stDxfBlock*> m_vpBlock;
	stDxfBlock *m_pBlockData;

	CLayerMatch * m_pLayerMatch;  // 层匹配
	CCoordinateMatch * m_pCoordinateMatch;  // 坐标系匹配
	CPolylineMatch * m_pPolylineMatch;     // 多边形匹配
	CSplineMatch * m_pSplineMatch;         // 样条曲线匹配
};

/**
* @brief 外部调用
*/
class CDxfFile
{
public:
	bool LoadFile(const char sFileName[], IGraphObject * pObject)
	{
		if (sFileName == nullptr || pObject == nullptr)
			return false;
		CDxfData dxfdata(pObject);
		CDxfReader dxf;
		bool bRet = dxf.in(sFileName, &dxfdata);
		if (!bRet)	return false;

		return true;
	}
};

