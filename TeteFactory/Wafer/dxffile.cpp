#include "stdafx.h"    
#include "dxffile.h"



CDxfData::CDxfData(IGraphObject * pObject)
{
	m_pObject = pObject;
	m_bPolyline = false;
	m_bBlock = false;
	m_pBlockData = nullptr;
	m_pLayerMatch = new CLayerMatch();
	m_pCoordinateMatch = new CCoordinateMatch();
	m_pPolylineMatch = nullptr;
	m_pSplineMatch = nullptr;
	m_bSpline = false;
}

CDxfData::~CDxfData()
{
	clear();
}

void CDxfData::clear()
{
	for (auto & item : m_vpBlock)
	{
		m_pBlockData->m_vpData->RemoveAllMeta();
		item->m_vpData = nullptr;
		delete item;
	}
	m_vpBlock.clear();

	if (nullptr != m_pLayerMatch)
		delete m_pLayerMatch;
	if (nullptr != m_pCoordinateMatch)
		delete m_pCoordinateMatch;
	if (nullptr != m_pPolylineMatch)
		delete m_pPolylineMatch;
	if (nullptr != m_pSplineMatch)
		delete m_pSplineMatch;
}

void CDxfData::setVariableVector(const std::string &s, double x, double y, double z, int c)
{
	if ("$EXTMIN" == s)
	{
		m_pCoordinateMatch->SetExtMin(x, y);
	}
	else if ("$EXTMAX" == s)
	{
		m_pCoordinateMatch->SetExtMax(x, y);
	}
}

void CDxfData::setVariableString(const std::string &s, const std::string &data, int c)
{

}

void CDxfData::setVariableInt(const std::string &s, int data, int c)
{
	if ("$INSUNITS" == s)
	{
		m_pCoordinateMatch->SetUnit(data);
	}
	else if ("$SPLINESEGS" == s)
	{
		m_pCoordinateMatch->SetSplineSegs(data);
	}
}

void CDxfData::setVariableDouble(const std::string &s, double data, int c)
{

}

void CDxfData::addLayer(const DL_LayerData &data)
{
	m_pLayerMatch->AddLayer(data);
}

void CDxfData::addBlock(const DL_BlockData &data)
{
	if (nullptr == m_pObject)
		return;

	m_pBlockData = new stDxfBlock;
	m_pBlockData->m_vpData = m_pObject->Clone();
	m_pBlockData->m_vpData->RemoveAllMeta();   // 清除所有的图元
	m_pBlockData->m_BlockData = data;     // 块的基准点总是转成为（0， 0），其余坐标对应转换
	m_pBlockData->m_sLayerName = attributes.getLayer();
	m_vpBlock.push_back(m_pBlockData);
	m_bBlock = true;
}

void CDxfData::endBlock()
{
	m_bBlock = false;
}

void CDxfData::addPoint(const DL_PointData & data)
{
	if (nullptr == m_pObject)
		return;
	double x = data.x;
	double y = data.y;
	// 坐标转换
	m_pCoordinateMatch->PointMap(x, y);
	stDot_t stData;
	stData.x = x;
	stData.y = y;
	if (m_bBlock && m_pBlockData)
	{
		m_pBlockData->m_vpData->AppendMeta(emMETA_TAG_FRAME, stData);
	}
	else
	{
		int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
		if (-1 == nLayerID)
			return;
		m_pObject->AppendMeta(emMETA_TAG_FRAME, stData);
		m_pObject->SetLayer(nLayerID);
	}
}

void CDxfData::addLine(const DL_LineData &data)
{
	if (nullptr == m_pObject)
		return;
	double x1 = data.x1;
	double y1 = data.y1;
	double x2 = data.x2;
	double y2 = data.y2;
	// 坐标转换
	m_pCoordinateMatch->PointMap(x1, y1);
	m_pCoordinateMatch->PointMap(x2, y2);
	stLine_t stData;
	stData.x1 = x1;
	stData.y1 = y1;
	stData.x2 = x2;
	stData.y2 = y2;
	if (m_bBlock && m_pBlockData)
	{
		m_pBlockData->m_vpData->AppendMeta(emMETA_TAG_FRAME, stData);
	}
	else
	{
		int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
		if (-1 == nLayerID)
			return;

		m_pObject->AppendMeta(emMETA_TAG_FRAME, stData);
		m_pObject->SetLayer(nLayerID);
	}
}

void CDxfData::addArc(const DL_ArcData &data)
{
	if (nullptr == m_pObject)
		return;

	double cx = data.cx;
	double cy = data.cy;
	double r = data.radius;
	double angle1 = data.angle1;
	double angle2 = data.angle2;
	if (angle1 > angle2)
	{
		angle2 += 360;
	}
	// 坐标转换
	m_pCoordinateMatch->PointMap(cx, cy);
	r = m_pCoordinateMatch->LenMap(r);
	stArc_t stArc;
	stArc.cx = cx;
	stArc.cy = cy;
	stArc.radiusx = r;
	stArc.radiusy = r;
	stArc.angle1 = D2R(angle1);
	stArc.angle2 = D2R(angle2);
	stArc.isCloclWise = false;
	if (m_bBlock && m_pBlockData)
	{
		m_pBlockData->m_vpData->AppendMeta(emMETA_TAG_FRAME, stArc);
	}
	else
	{
		int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
		if (-1 == nLayerID)
			return;

		m_pObject->AppendMeta(emMETA_TAG_FRAME, stArc);
		m_pObject->SetLayer(nLayerID);
	}
}

void CDxfData::addCircle(const DL_CircleData &data)
{
	if (nullptr == m_pObject)
		return;
	double cx = data.cx;
	double cy = data.cy;
	double r = data.radius;
	// 坐标转换
	m_pCoordinateMatch->PointMap(cx, cy);
	r = m_pCoordinateMatch->LenMap(r);
	stCircle_t stCircle;
	stCircle.cx = cx;
	stCircle.cy = cy;
	stCircle.radius = r;
	if (m_bBlock && m_pBlockData)
	{
		m_pBlockData->m_vpData->AppendMeta(emMETA_TAG_FRAME, stCircle);
	}
	else
	{
		int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
		if (-1 == nLayerID)
			return;

		m_pObject->AppendMeta(emMETA_TAG_FRAME, stCircle);
		m_pObject->SetLayer(nLayerID);
	}
}

void CDxfData::addEllipse(const DL_EllipseData &data)
{
	if (nullptr == m_pObject)
		return;
	
	CEllipseMatch EllipseMatch(m_pCoordinateMatch);
	
	if (m_bBlock && m_pBlockData)
	{
		EllipseMatch.Match(data, m_pBlockData->m_vpData);
	}
	else
	{
		int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
		if (-1 == nLayerID)
			return;

		EllipseMatch.Match(data, m_pObject);
		m_pObject->SetLayer(nLayerID);
	}
}

void CDxfData::addPolyline(const DL_PolylineData &data)
{
	if (nullptr != m_pPolylineMatch)
	{
		delete m_pPolylineMatch;
	}
	m_pPolylineMatch = new CPolylineMatch(m_pCoordinateMatch);
	m_pPolylineMatch->AddPolylineData(data);
	m_bPolyline = true;
}

void CDxfData::addVertex(const DL_VertexData &data)
{
	if (m_bPolyline)
	{
		m_pPolylineMatch->AddVertex(data);
	}
}

void CDxfData::addSpline(const DL_SplineData & data )
{
	if (nullptr != m_pSplineMatch)
	{
		delete m_pSplineMatch;
	}
	m_pSplineMatch = new CSplineMatch(m_pCoordinateMatch);
	m_pSplineMatch->AddSplineData(data);
	m_bSpline = true;
}

void CDxfData::addControlPoint(const DL_ControlPointData & data)
{
	if (m_bSpline)
	{
		m_pSplineMatch->AddControlData(data);
	}
}

void CDxfData::addFitPoint(const DL_FitPointData & data)
{
	if (m_bSpline)
	{
		m_pSplineMatch->AddFitData(data);
	}
}

void CDxfData::addKnot(const DL_KnotData & data)
{
	if (m_bSpline)
	{
		m_pSplineMatch->AddKnotData(data);
	}
}

void CDxfData::endEntity()
{
	if (nullptr == m_pObject)
		return;
	if (m_bPolyline)
	{
		// 多段线结束
		if (m_bBlock && m_pBlockData)
		{
			m_pPolylineMatch->Match(m_pBlockData->m_vpData);
		}
		else
		{
			int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
			if (-1 == nLayerID)
			{
				delete m_pPolylineMatch;
				m_pPolylineMatch = nullptr;
				m_bPolyline = false;
				return;
			}	
			m_pPolylineMatch->Match(m_pObject);
			m_pObject->SetLayer(nLayerID);
		}
		delete m_pPolylineMatch;
		m_pPolylineMatch = nullptr;
		m_bPolyline = false;
	}

	else if (m_bSpline)
	{
		// 样条曲线结束
		if (m_bBlock && m_pBlockData)
		{
			m_pSplineMatch->Match(m_pBlockData->m_vpData);
		}
		else
		{
			int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
			if (-1 == nLayerID)
			{
				delete m_pSplineMatch;
				m_pSplineMatch = nullptr;
				m_bSpline = false;
				return;
			}
			m_pSplineMatch->Match(m_pObject);
			m_pObject->SetLayer(nLayerID);
		}
		delete m_pSplineMatch;
		m_pSplineMatch = nullptr;
		m_bSpline = false;
	}
}

void CDxfData::addInsert(const DL_InsertData &data)
{
	if (nullptr == m_pObject)
		return;

	// 查找指定的块数据
	for (auto & item : m_vpBlock)
	{
		if (item->m_BlockData.name == data.name)
		{
			int nLayerID = m_pLayerMatch->MatchLayerID(attributes.getLayer());
			if (-1 == nLayerID)
				return;
			IGraphObject *pTheObject = item->m_vpData->Clone();
			pTheObject->SetLayer(nLayerID);
			if (nullptr != pTheObject)
			{
				double bpx = item->m_BlockData.bpx, bpy = item->m_BlockData.bpy, ipx = data.ipx, ipy = data.ipy;
				m_pCoordinateMatch->PointMap(bpx, bpy);
				m_pCoordinateMatch->PointMap(ipx, ipy);
				
				if(!(D_EQ(data.sx, 1) && D_EQ(data.sy, 1)))
					pTheObject->Resize(data.sx, data.sy);
				if(!D_EQ(data.angle, 0))
					pTheObject->Rotate(bpx, bpx, D2R(data.angle));
				
				pTheObject->Repos(ipx - bpx, ipy - bpx, 0);

				// 将pTheObject对象和m_pObject合并
				while (pTheObject->GetMetaCount() > 0)
				{
					IGraphMeta * pMeta = pTheObject->GetMeta(0);
					if (emMETA_TAG_USELESS == pMeta->MetaTag())
					{
						pTheObject->DeleteMeta(0);
						continue;
					}
					pMeta = pTheObject->DetachMeta(0);   // 分离图元
					if (NULL != pMeta)
					{
						pMeta->SetMetaTag(emMETA_TAG_FRAME);
						m_pObject->AppendMeta(pMeta);   // 如果有填充属性的图元，合并之后，改变其属性为外框
					}
				}

				delete pTheObject;
			}
			break;
		}
	}
}

double CDxfData::GetFactor()
{
	return m_pCoordinateMatch->GetFactor();
}


