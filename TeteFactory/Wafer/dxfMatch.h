#pragma once

#include "ThirdParty\dxflib\dl_dxf.h"
#include "ThirdParty\dxflib\dl_creationadapter.h"

#include<numeric> 

#include "tlGraphObject.h"     

#define D_EQ(a,b) (fabs((a) - (b)) < 0.000001)
#define PI                      3.1415926
#define D2R(d)              ((PI*(d))/180.0)   /*角度转换为弧度*/
#define R2D(d)              ((180*(d))/PI)     /*弧度转换为角度*/


/**
* @brief 块数据结构
*/
typedef struct DxfBlock
{
	DL_BlockData m_BlockData;
	IGraphObject * m_vpData;
	std::string m_sLayerName;

	DxfBlock() :
		m_BlockData("", 0, 0, 0, 0)
	{
		m_vpData = nullptr;
	}
}stDxfBlock;

/**
* @brief 层数据结构
*/
typedef struct DxfLayer
{
	std::string m_sName;
	int m_nID;    // 不等于-1时，表示该层已被使用     

	DxfLayer()
	{
		m_sName = "";
		m_nID = -1;
	}
}stDxfLayer;

/**
* @brief dxf中层信息与CMarkDoc中的层信息匹配类
*/
class CLayerMatch
{
public:
	CLayerMatch()
	{
		s_CurrentID = 0;
	}
	~CLayerMatch()
	{
		for (auto & item : m_vpLayer)
		{
			delete item;
		}
		m_vpLayer.clear();
	}
	void AddLayer(const DL_LayerData &data)
	{
		stDxfLayer * pData = new stDxfLayer();
		pData->m_sName = data.name;
		m_vpLayer.push_back(pData);
	}
	/**
	* @brief 根据实体的层信息与dxf中所有层信息进行对比，然后结合CMarkDoc中层ID，进行匹配。
	* @return 如果匹配成功，返回CMarkDoc中层的ID；如果匹配失败，返回-1。
	*/
	int MatchLayerID(const std::string sLayerName)
	{
		for (auto & item : m_vpLayer)
		{
			if (item->m_sName == sLayerName)
			{
				if (item->m_nID == -1)
				{
					item->m_nID = s_CurrentID++;
				}
				return item->m_nID;
			}
		}
		return -1;
	}

protected:
	int s_CurrentID;  // 当前ID号
	std::vector<stDxfLayer*> m_vpLayer;  // 层集合

};


/**
* @brief 坐标单位枚举,基于DXF文档说明
* 详细查询网址：http://docs.autodesk.com/ACD/2011/CHS/filesDXF/WS1a9193826455f5ff18cb41610ec0a2e719-7a6f.htm
*/
enum emDxfUnit
{
	UNIT_NONE = 0,       // 无单位
	UNIT_INCH = 1,       // 英寸
	UNIT_FEET = 2,       // 英尺
	UNIT_MM = 4,         // 毫米
	UNIT_CM = 5,         // 厘米
	UNIT_MINCH = 8,      // 微英寸
	UNIT_UM = 13,        // 微米
	UNIT_DECIM = 14,     // 分米
};

/**
* @brief header设置属性
*/
typedef struct Header
{
	DL_PointData extmin;  // 图形范围的左下点，基于世界坐标系。
	DL_PointData extmax;  // 图形范围的右上点，基于世界坐标系。
	emDxfUnit units;  // 图形使用单位
	int nSplineSegs;    // 每个样条曲线曲面的直线段数目
	Header()
	{
		units = UNIT_NONE;
	}
}stHeaderSet;
/**
* @brief dxf中坐标与CMarkDoc中坐标匹配类
* 目前只有检测dxf中的尺寸单位，忽略坐标的方向
*/
class CCoordinateMatch
{
private:
	/**
	* @brief 长度单位转换公式：1 inch = 25.4 mm
	*/
	double unitConvert(emDxfUnit emOld, emDxfUnit emNew)
	{
		double In2MM = 25.4;
		double Ft2In = 12;
		double factor = 1.0;
		if (UNIT_NONE == emNew || UNIT_NONE == emOld
			|| emOld == emNew)
			return 1.0;

		switch (emOld)
		{
		case UNIT_INCH:
			switch (emNew)
			{
			case UNIT_FEET:
				factor = 1 / Ft2In;
				break;
			case UNIT_MM:
				factor = In2MM;
				break;
			case UNIT_CM:
				factor = In2MM / 10;
				break;
			case UNIT_MINCH:
				factor = 1000;
				break;
			case UNIT_UM:
				factor = In2MM * 1000;
				break;
			case UNIT_DECIM:
				factor = In2MM / 100;
				break;
			default:
				break;
			}
			break;
		case UNIT_FEET:
			switch (emNew)
			{
			case UNIT_INCH:
				factor = Ft2In;
				break;
			case UNIT_MM:
				factor = Ft2In * In2MM;
				break;
			case UNIT_CM:
				factor = Ft2In * In2MM / 10;
				break;
			case UNIT_MINCH:
				factor = Ft2In * 1000;
				break;
			case UNIT_UM:
				factor = Ft2In * In2MM * 1000;
				break;
			case UNIT_DECIM:
				factor = Ft2In * In2MM / 100;
				break;
			default:
				break;
			}
			break;
		case UNIT_MM:
			switch (emNew)
			{
			case UNIT_INCH:
				factor = 1 / In2MM;
				break;
			case UNIT_FEET:
				factor = 1 / In2MM / Ft2In;
				break;
			case UNIT_CM:
				factor = 0.1;
				break;
			case UNIT_MINCH:
				factor = 1 / In2MM * 1000;
				break;
			case UNIT_UM:
				factor = 1000;
				break;
			case UNIT_DECIM:
				factor = 0.001;
				break;
			default:
				break;
			}
			break;
		case UNIT_CM:
			switch (emNew)
			{
			case UNIT_INCH:
				factor = 10 / In2MM;
				break;
			case UNIT_FEET:
				factor = 10 / In2MM / Ft2In;
				break;
			case UNIT_MM:
				factor = 10;
				break;
			case UNIT_MINCH:
				factor = 10 / In2MM * 1000;
				break;
			case UNIT_UM:
				factor = 10 * 1000;
				break;
			case UNIT_DECIM:
				factor = 0.1;
				break;
			default:
				break;
			}
			break;
		case UNIT_MINCH:
			switch (emNew)
			{
			case UNIT_INCH:
				factor = 0.001;
				break;
			case UNIT_FEET:
				factor = 0.001 / Ft2In;
				break;
			case UNIT_MM:
				factor = 0.001 / In2MM;
				break;
			case UNIT_CM:
				factor = 0.0001 / In2MM;
				break;
			case UNIT_UM:
				factor = 1 / In2MM;
				break;
			case UNIT_DECIM:
				factor = 0.00001 / In2MM;
				break;
			default:
				break;
			}
			break;
		case UNIT_UM:
			switch (emNew)
			{
			case UNIT_INCH:
				factor = 0.001 / In2MM;
				break;
			case UNIT_FEET:
				factor = 0.001 / In2MM / Ft2In;
				break;
			case UNIT_MM:
				factor = 0.001;
				break;
			case UNIT_CM:
				factor = 0.0001;
				break;
			case UNIT_MINCH:
				factor = 1 / In2MM;
				break;
			case UNIT_DECIM:
				factor = 0.00001;
				break;
			default:
				break;
			}
			break;
		case UNIT_DECIM:
			switch (emNew)
			{
			case UNIT_INCH:
				factor = 100 / In2MM;
				break;
			case UNIT_FEET:
				factor = 100 / In2MM / Ft2In;
				break;
			case UNIT_MM:
				factor = 100;
				break;
			case UNIT_CM:
				factor = 10;
				break;
			case UNIT_MINCH:
				factor = 10000 / In2MM;
				break;
			case UNIT_UM:
				factor = 100000;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		return factor;
	}

	/**
	* @brief 计算dxf图形的左边原点和缩放比例
	*/
	void CalcSizePos()
	{
		// 计算Dxf图形的坐标原点
		//double dAreaW = m_headerset.extmax.x - m_headerset.extmin.x;
		//double dAreaH = m_headerset.extmax.y - m_headerset.extmin.y;
		//m_cx = m_headerset.extmin.x + dAreaW / 2;
		//m_cy = m_headerset.extmin.y + dAreaH / 2;

		// 单位换算
		m_dbFactor = unitConvert(m_headerset.units, UNIT_MM);
	}
public:
	CCoordinateMatch()
	{
		m_cx = m_cy = 0;
		m_mx = m_my = 0;
		m_dbFactor = 1.0;
		m_bFirst = true;
	}
	// 设置CMarkDoc图形坐标点（Dxf图形中心坐标将要放置的点，一般为坐标原点即可）
	void SetMapPoint(double mx, double my)
	{
		m_mx = mx;
		m_my = my;
	}
	// 设置Dxf中HEADER段中的变量信息
	void SetExtMin(double x, double y)
	{
		m_headerset.extmin = DL_PointData(x, y);

		CalcSizePos();
	}
	void SetExtMax(double x, double y)
	{
		m_headerset.extmax = DL_PointData(x, y);

		CalcSizePos();
	}
	void SetUnit(int n)
	{
		m_headerset.units = (emDxfUnit)n;

		m_dbFactor = unitConvert(m_headerset.units, UNIT_MM);

		CalcSizePos();
	}

	void SetSplineSegs(int n)
	{
		m_headerset.nSplineSegs = n;
	}

	int GetSplineSegs()
	{
		return m_headerset.nSplineSegs;
	}

	// Dxf中的坐标转换成CMarkDoc中的坐标, 暂时先不转换
	void PointMap(double & x, double & y)
	{
		// 根据Map点，进行坐标转换
		x = (x - m_cx) * m_dbFactor + m_mx;
		y = (y - m_cy) * m_dbFactor + m_my;
	}

	// Dxf中距离换成CMarkDoc中的距离
	double LenMap(double len, emDxfUnit unit = UNIT_NONE)
	{
		return m_dbFactor * len;
	}

	double GetFactor()
	{
		return m_dbFactor;
	}

protected:
	double m_cx, m_cy;
	double m_mx, m_my;
	stHeaderSet m_headerset;
	double m_dbFactor;
	bool m_bFirst;
};

/**
* @brief dxf中多段线实体，与CMarkDoc中的图形对象匹配
* dxf中多段线对象，可能包含折线、圆弧。
*/
class CPolylineMatch
{
public:
	CPolylineMatch(CCoordinateMatch * p = nullptr) :
		m_pldata(0, 0, 0, 0)
	{
		m_pCoordinateMatch = p;
	}
	/**
	* @功能 根据起点和终点以及其凸度,求圆弧的圆心、半径、起始角和结束角
	* @参数[in] p1, p2p表示圆弧的起点和终点
	* @参数[in] dBulge 凸度值
	* @参数[out] arcdata 圆弧结构。其中圆弧结构中的起始角和结束角单位是角度。按逆时针旋转。
	* @返回值 成功返回true，失败返回false。
	*/
	static void s_GetArcByBulge(const DL_PointData & p1, const DL_PointData & p2, double dBulge, DL_ArcData & arcdata)
	{
		if (D_EQ(dBulge, 0)) return;
		if (p1.x == p2.x && p1.y == p2.y) return;

		double x1 = p1.x;
		double y1 = p1.y;
		double x2 = p2.x;
		double y2 = p2.y;

		//--圆弧角度，单位是弧度  凸度 = 1/4 tan(圆弧角)
		double dDegree = 4 * atan(fabs(dBulge));

		//--圆弧的弦长
		double dChordLen = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

		//--圆弧的半径
		double dR = fabs((0.5 * dChordLen) / sin(0.5 * dDegree));

		//--弦中心到圆心的距离的绝对值
		double dCR = sqrt(dR * dR - dChordLen * dChordLen / 4.0);

		//--弦的中心点坐标
		double dCMidX = (x1 + x2) * 0.5;
		double dCMidY = (y1 + y2) * 0.5;

		//--弦中心到圆心在坐标轴的变化距离
		double dX = 0.0, dY = 0.0;
		if (D_EQ(x1, x2))
		{
			//--弦垂直
			dX = dCR;
			dY = 0;
		}
		else if (D_EQ(y1, y2))
		{
			//--弦水平
			dX = 0;
			dY = dCR;
		}
		else
		{
			//--公式1 dX*dX + dY*dY = dCR * dCR   公式2 dY / dX = fabs(dVerK)
			double dVerK = (x1 - x2) / (y2 - y1);   //--弦垂直线的斜率t = -1 / k ;
			dX = fabs(dCR / sqrt(1 + dVerK * dVerK));
			dY = fabs(dVerK * dX);
		}

		//--根据凸度判断圆弧的角度, 当弧度大于1时，圆弧角度大于180
		bool bMinorArc = true;
		if (fabs(dBulge) > 1)
		{
			bMinorArc = false;
		}

		//--根据凸度判断圆弧的走向,凸度为负数时，表示顺时针
		bool bCw = false;
		if (dBulge < 0)
		{
			bCw = true;
		}

		//--求起点指向终点的向量（弦向量）与X轴正方向单位向量的叉积amass = (x2 - x1, y2 - y1) 叉积 (1, 0)
		double dAmass = 1 * (x2 - x1) + 0 * (y2 - y1);

		//--求弦向量与x轴正方向的角度(单位为度), 向量a 叉积 向量b = |向量a| * |向量b| * cos(角度)
		double dChordAng = R2D(acos(dAmass / dChordLen));
		//--如果结束点在起始点的下面，则角度为负
		if ((y2 - y1) < 0)
		{
			dChordAng *= -1;
		}

		//--计算圆心
		double cx = 0.0, cy = 0.0;
		if ((dChordAng > 0 && dChordAng < 90) || D_EQ(dChordAng, 0) || D_EQ(dChordAng, 90))
		{
			//--第一象限
			if (bMinorArc)  //--小弧
			{
				if (bCw)  //--顺时针
				{
					cx = dCMidX + dX;
					cy = dCMidY - dY;
				}
				else //--逆时针
				{
					cx = dCMidX - dX;
					cy = dCMidY + dY;
				}
			}
			else //--大弧
			{
				if (bCw) //--顺时针
				{
					cx = dCMidX - dX;
					cy = dCMidY + dY;
				}
				else  //--逆时针
				{
					cx = dCMidX + dX;
					cy = dCMidY - dY;
				}
			}
		}
		else if ((dChordAng >90 && dChordAng < 180) || D_EQ(dChordAng, 180))
		{
			//--第二象限
			if (bMinorArc) //--小弧
			{
				if (bCw)  //--顺时针
				{
					cx = dCMidX + dX;
					cy = dCMidY + dY;
				}
				else   //--逆时针
				{
					cx = dCMidX - dX;
					cy = dCMidY - dY;
				}
			}
			else  //--大弧
			{
				if (bCw)  //--顺时针
				{
					cx = dCMidX - dX;
					cy = dCMidY - dY;
				}
				else  //--逆时针
				{
					cx = dCMidX + dX;
					cy = dCMidY + dY;
				}
			}
		}
		else if ((dChordAng < -90 && dChordAng > -180))
		{
			//--第三象限
			if (bMinorArc) //--小弧
			{
				if (bCw)  //--顺时针
				{
					cx = dCMidX - dX;
					cy = dCMidY + dY;
				}
				else  //--逆时针
				{
					cx = dCMidX + dX;
					cy = dCMidY - dY;
				}
			}
			else
			{
				//--大弧
				if (bCw)  //--顺时针
				{
					cx = dCMidX + dX;
					cy = dCMidY - dY;
				}
				else  //--逆时针
				{
					cx = dCMidX - dX;
					cy = dCMidY + dY;
				}
			}
		}
		else
		{
			//--第四象限
			if (bMinorArc)  //--小弧
			{
				if (bCw)  //--顺时针
				{
					cx = dCMidX - dX;
					cy = dCMidY - dY;
				}
				else  //--逆时针
				{
					cx = dCMidX + dX;
					cy = dCMidY + dY;
				}
			}
			else  //--大弧
			{
				if (bCw)   //--顺时针
				{
					cx = dCMidX + dX;
					cy = dCMidY + dY;
				}
				else  //--逆时针
				{
					cx = dCMidX - dX;
					cy = dCMidY - dY;
				}
			}
		}

		//--计算起始角和结束角，通过向量点积
		// a = [a1, a2] b = [b1, b2] a * b = a1*b1 + a2*b2 = |a|*|b|*cos(x)

		double dSAmass = 1 * (x1 - cx) + 0 * (y1 - cy);   //--圆心指向起始点的向量与X轴正方向单位向量的叉积
		double dCosValue = dSAmass / dR;
		if (dCosValue < -1)  dCosValue = -1;
		if (dCosValue > 1) dCosValue = 1;
		double dSAng = R2D(acos(dCosValue));
		if ((y1 - cy) < 0)
		{
			dSAng = 360 - dSAng;
		}

		double dEAmass = 1 * (x2 - cx) + 0 * (y2 - cy);   //--圆心指向结束点的向量与X轴正方向单位向量的叉积
		dCosValue = dEAmass / dR;
		if (dCosValue < -1)  dCosValue = -1;
		if (dCosValue > 1) dCosValue = 1;
		double dEAng = R2D(acos(dCosValue));
		if ((y2 - cy) < 0)
		{
			dEAng = 360 - dEAng;
		}

		if (bCw)
		{
			// 如果为顺时针，交换起始角度和结束角度
			double dTmp = dSAng;
			dSAng = dEAng;
			dEAng = dTmp;
		}

		arcdata.cx = cx;
		arcdata.cy = cy;
		arcdata.angle1 = dSAng;
		arcdata.angle2 = dEAng;
		arcdata.radius = dR;
	}

	void AddPolylineData(const DL_PolylineData &data)
	{
		m_pldata = data;
	}
	
	void AddVertex(const DL_VertexData & data)
	{
		m_vdata.push_back(data);
	}

	void Match(IGraphObject * pObject)
	{
		if (nullptr == m_pCoordinateMatch 
			|| nullptr == pObject)
			return;

		bool bClosedCurve = m_pldata.flags & 0x1;  // 根据dxf文档说明，0x01表示闭合曲线
		if (bClosedCurve && m_vdata.size() > 0 &&
			!(D_EQ(m_vdata.front().x, m_vdata.back().x) && D_EQ(m_vdata.front().y, m_vdata.back().y)))
			m_vdata.push_back(m_vdata.front());

		double dx = 0, dy = 0;  // 临时变量
		std::vector<DPoint_t> vp;
		double dbBulge = 0;
		for (int i = 0, nC = m_vdata.size(); i < nC; i++)
		{
			dbBulge = m_vdata[i].bulge;
			if ((!D_EQ(dbBulge, 0) || nC - 1 == i)
				&& (vp.size() != 0))
			{
				// 两种情况标志着一段独立的折线，vp中存在点，且下一段为圆弧；vp中存在点，且为最后一个点
				dx = m_vdata[i].x;
				dy = m_vdata[i].y;
				m_pCoordinateMatch->PointMap(dx, dy);
				vp.push_back(DPoint_t(dx, dy));
				stPolyline_t stData;
				stData.isize = vp.size();
				stData.ppt = new DPoint_t[stData.isize];
				for (int n = 0; n < stData.isize; n++)
				{
					stData.ppt[n] = vp[n];
				}
				pObject->AppendMeta(emMETA_TAG_FRAME, stData);
				vp.clear();
				delete[] stData.ppt;
			}
			if (!D_EQ(dbBulge, 0) && i != nC -1)   
			{
				// 圆弧
				DL_ArcData stArcData(0, 0, 0, 0, 0, 0);
				s_GetArcByBulge(DL_PointData(m_vdata[i].x, m_vdata[i].y), DL_PointData(m_vdata[i + 1].x, m_vdata[i + 1].y), dbBulge, stArcData);
						
				// 坐标转换
				double cx = stArcData.cx;
				double cy = stArcData.cy;
				double r = stArcData.radius;
				m_pCoordinateMatch->PointMap(cx, cy);
				r = m_pCoordinateMatch->LenMap(r);
				if (stArcData.angle1 > stArcData.angle2)
				{
					stArcData.angle2 += 360;
				}
				stArc_t stArc;
				stArc.cx = cx;
				stArc.cy = cy;
				stArc.radiusx = r;
				stArc.radiusy = r;
				stArc.angle1 = D2R(stArcData.angle1);
				stArc.angle2 = D2R(stArcData.angle2);
				stArc.isCloclWise = false;
				pObject->AppendMeta(emMETA_TAG_FRAME, stArc);
			}
			else
			{
				dx = m_vdata[i].x;
				dy = m_vdata[i].y;
				m_pCoordinateMatch->PointMap(dx, dy);
				vp.push_back(DPoint_t(dx, dy));
			}
		}
	}
protected:
	DL_PolylineData m_pldata;
	std::vector<DL_VertexData> m_vdata;
	CCoordinateMatch * m_pCoordinateMatch;  // 坐标系匹配
};

/**
* @brief dxf中椭圆实体，与CMarkDoc中的图形对象匹配
* dxf中椭圆对象，可能包含不同角度的椭圆，还有椭圆弧（暂不处理）。
*/
class CEllipseMatch
{
public:
	CEllipseMatch(CCoordinateMatch * p = nullptr)
	{
		m_pCoordinateMatch = p;
	}
	void Match(const DL_EllipseData &data, IGraphObject * pObject)
	{
		if (nullptr == pObject)
			return;

		double dbLeft = 0, dbTop = 0, dbRight = 0, dbBottom = 0;
		double dbRad = 0;   // 椭圆旋转弧度
		double cx = data.cx;
		double cy = data.cy;
		double dbMajorR = sqrt(data.my * data.my  + data.mx * data.mx);
		double dbMinorR = dbMajorR * data.ratio;
		// 计算主轴与X轴正方向的夹角
		if (D_EQ(data.mx, 0))
		{
			// 椭圆的主轴和次轴对调
			dbLeft = cx - dbMinorR;
			dbBottom = cy - dbMajorR;
			dbRight = cx + dbMinorR;
			dbTop = cy + dbMajorR;
		}
		else
		{
			double k = data.my / data.mx;
			dbRad = tanh(k);
			dbLeft = cx - dbMajorR;
			dbBottom = cy - dbMinorR;
			dbRight = cx + dbMajorR;
			dbTop = cy + dbMinorR;
		}

		// 生成椭圆对象
		m_pCoordinateMatch->PointMap(dbLeft, dbBottom);
		m_pCoordinateMatch->PointMap(dbRight, dbTop);
		m_pCoordinateMatch->PointMap(cx, cy);
		stEllipse_t stData;
		stData.InitData(dbLeft, dbTop, dbRight, dbBottom);
		pObject->AppendMeta(emMETA_TAG_FRAME, stData);
		if(!D_EQ(dbRad, 0))
			pObject->Rotate(cx, cy, dbRad);
	}

protected:
	CCoordinateMatch * m_pCoordinateMatch;  // 坐标系匹配
};

/**
* @brief dxf中样条曲线，与CMarkDoc中的图形对象匹配
* 
*/
class CSplineMatch
{
// Spline解析函数
protected:
	std::vector<double> knot(size_t num, size_t order) const 
	{
		if (m_vKnotP.size() == num + order) {
			//use custom knot vector
			return m_vKnotP;
		}

		std::vector<double> knotVector(num + order, 0.);
		//use uniform knots
		std::iota(knotVector.begin() + order, knotVector.begin() + num + 1, 1);
		std::fill(knotVector.begin() + num + 1, knotVector.end(), knotVector[num]);
		return knotVector;
	}

	std::vector<double> rbasis(int c, double t, int npts,
		const std::vector<double>& x,
		const std::vector<double>& h) const{

		int const nplusc = npts + c;

		std::vector<double> temp(nplusc, 0.);

		// calculate the first order nonrational basis functions n[i]
		for (int i = 0; i< nplusc - 1; i++)
			if ((t >= x[i]) && (t < x[i + 1])) temp[i] = 1;

		/* calculate the higher order nonrational basis functions */

		for (int k = 2; k <= c; k++) {
			for (int i = 0; i < nplusc - k; i++) {
				// if the lower order basis function is zero skip the calculation
				if (temp[i] != 0)
					temp[i] = ((t - x[i])*temp[i]) / (x[i + k - 1] - x[i]);
				// if the lower order basis function is zero skip the calculation
				if (temp[i + 1] != 0)
					temp[i] += ((x[i + k] - t)*temp[i + 1]) / (x[i + k] - x[i + 1]);
			}
		}

		// pick up last point
		if (t >= x[nplusc - 1]) temp[npts - 1] = 1;

		// calculate sum for denominator of rational basis functions
		double sum = 0.;
		for (int i = 0; i < npts; i++) {
			sum += temp[i] * h[i];
		}

		std::vector<double> r(npts, 0);
		// form rational basis functions and put in r vector
		if (sum != 0) {
			for (int i = 0; i < npts; i++)
				r[i] = (temp[i] * h[i]) / sum;
		}
		return r;
	}

	void rbspline(size_t npts, size_t k, size_t p1,
		const std::vector<DL_ControlPointData>& b,
		const std::vector<double>& h,
		std::vector<DPoint_t>& p) const 
	{

		size_t const nplusc = npts + k;

		// generate the open knot vector
		auto const x = knot(npts, k);

		// calculate the points on the rational B-spline curve
		double t{ x[0] };
		double const step{ (x[nplusc - 1] - t) / (p1 - 1) };

		for (auto & vp : p) {
			if (x[nplusc - 1] - t < 5e-6) t = x[nplusc - 1];

			// generate the basis function for this value of t
			auto const nbasis = rbasis((int)k, t, npts, x, h);
			rbasis((int)k, t, (int)npts, x, h);

			// generate a point on the curve
			for (size_t i = 0; i < npts; i++)
			{
				vp.x += b[i].x * nbasis[i];
				vp.y += b[i].y * nbasis[i];
			}

			t += step;
		}

	}


	std::vector<double> knotu(size_t num, size_t order) const
	{
		if (m_vKnotP.size() == num + order) {
			//use custom knot vector
			return m_vKnotP;
		}
		std::vector<double> knotVector(num + order, 0.);
		std::iota(knotVector.begin(), knotVector.end(), 0);
		return knotVector;
	}

	void rbsplinu(size_t npts, size_t k, size_t p1,
		const std::vector<DL_ControlPointData>& b,
		const std::vector<double>& h,
		std::vector<DPoint_t>& p) const {
		size_t const nplusc = npts + k;

		/* generate the periodic knot vector */
		const std::vector<double> x = knotu(npts, k);

		/*    calculate the points on the rational B-spline curve */
		double t = k - 1;
		double const step = double(npts - k + 1) / (p1 - 1);

		for (auto& vp : p) {
			if (x[nplusc - 1] - t < 5e-6) t = x[nplusc - 1];

			/* generate the basis function for this value of t */
			auto const nbasis = rbasis(k, t, npts, x, h);
			/* generate a point on the curve, for x, y, z */
			for (size_t i = 0; i < npts; i++)
			{
				vp.x += b[i].x * nbasis[i];
				vp.y += b[i].y * nbasis[i];
			}

			t += step;
		}

	}


public:
	CSplineMatch(CCoordinateMatch * p = nullptr) :
		m_spdata(0, 0, 0, 0, 0)
	{
		m_pCoordinateMatch = p;
	}

	void AddSplineData(const DL_SplineData & data)
	{
		m_spdata = data;
	}

	void AddControlData(const DL_ControlPointData &data)
	{
		m_vControlP.push_back(data);
	}

	void AddFitData(const DL_FitPointData & data)
	{
		m_vFitP.push_back(data);
	}

	void AddKnotData(const DL_KnotData & data)
	{
		m_vKnotP.push_back(data.k);
	}

	void Match(IGraphObject * pObject)
	{
		if (nullptr == m_pCoordinateMatch || nullptr == pObject)
			return;

		// 样条曲线变成多边形
		if (1 == m_spdata.flags) 
		{
			// 闭合曲线
			for (size_t i = 0; i<m_spdata.degree; ++i) 
			{
				m_vControlP.push_back(m_vControlP.at(i));
			}
		}

		const size_t npts = m_vControlP.size();
		// order:
		const size_t  k = m_spdata.degree + 1;
		// resolution:
		const size_t  p1 = /*m_pCoordinateMatch->GetSplineSegs() * */npts;

		std::vector<double> h(npts + 1, 1.);
		std::vector<DPoint_t> p(p1, { 0., 0. });
		if (1 == m_spdata.flags) {
			rbsplinu(npts, k, p1, m_vControlP, h, p);
		}
		else {
			rbspline(npts, k, p1, m_vControlP, h, p);
		}

		// 去掉相邻重复点
		DPoint_t dpPre;
		bool bFirst = true;
		for (auto iter = p.begin(); iter != p.end();)
		{
			if (bFirst)
			{
				dpPre = *iter;
				bFirst = false;
				iter++;
			}
			else
			{
				if (dpPre == *iter)
				{
					iter = p.erase(iter);
				}
				else
				{
					dpPre = *iter;
					iter++;
				}
			}
		}

		stPolyline_t stPolyline;
		double dx, dy;
		stPolyline.isize = p.size();
		stPolyline.ppt = new DPoint_t[stPolyline.isize];
		for (size_t i = 0; i < stPolyline.isize; ++i)
		{
			dx = p[i].x;
			dy = p[i].y;
			m_pCoordinateMatch->PointMap(dx, dy);
			//CString s;
			//s.Format(_T("p%d: %.4f, %.4f\r\n"), i, dx, dy);
			//OutputDebugString(s);
			stPolyline.ppt[i].x = dx;
			stPolyline.ppt[i].y = dy;
		}
		pObject->AppendMeta(emMETA_TAG_FRAME, stPolyline);

		delete[] stPolyline.ppt;
	}
protected:
	DL_SplineData m_spdata;
	std::vector<DL_ControlPointData> m_vControlP;   // 控制点
	std::vector<DL_FitPointData> m_vFitP;  // 拟合点
	std::vector<double> m_vKnotP;     // 节点
	CCoordinateMatch * m_pCoordinateMatch;  // 坐标系匹配
};
