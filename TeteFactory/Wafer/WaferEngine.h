/**
******************************************************************************
* @模块 
* @作者 jchq
* @版本 V1.00.00
* @日期 2018-09-21
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明 
*    
*    
*    
*      
*        
*    
*    
******************************************************************************
*/
#pragma once
#include "tlTeteDefines.h"
#include "WaferGraphDoc.h"
#include "WaferMapDoc.h"

/****************************************************************************/
/*
** 晶圆工程文件
*/
class CWaferEngine
{
public:
    static CWaferEngine *Instance();
    ~CWaferEngine();
protected:
    CWaferEngine(void);    
private:
    static CWaferEngine *_instance;
public:

    CWaferGraphDoc * GetGraphDoc() { return m_pGraphDoc; }
    bool LoadGraphDoc(const char *sName);   //--
	bool SaveGraphDoc(const char *sName);   //--

	/*! 加载印章模板文档，激光参数，只加载参数名称，不加载内容 */
	bool LoadDocWithoutPen(const char *sName, std::vector<string> & vsPenName);

    CWaferMapDoc * GetMapDoc() { return m_pMapDoc; }
	bool LoadMapDoc(const char *sName);   //--
	bool SaveMapDoc(const char *sName);  //--


	// 保存和加载文件
	bool LoadFile(const char *sName);
	bool SaveFile(const char * sName);

	// 文件名称
	const char * GetFileName(void) { return m_sFileName; } // 获取文件保存名称
	void SetFileName(const char * sName = "") { strcpy_s(m_sFileName, sizeof m_sFileName, sName); }  // 设置文件名称

	// 文档操作
	void RemoveAllDoc();

	// 保存标志
	bool GetModified(void);
	void ResetModified(void);

	int32_t GetActiveDoc(void) {	return m_iActiveDoc; }
	void SetActiveDoc(int32_t iTab) { m_iActiveDoc = iTab; }
private:
	int32_t m_iActiveDoc;
	int m_iFileVersion;
	char m_sFileName[512];         //--文件名
    CWaferGraphDoc *m_pGraphDoc;
    CWaferMapDoc *m_pMapDoc;
};

inline double s_PtToSegDistance(double x, double y, double x1, double y1, double x2, double y2)
{
	/**
	* 直线l 点p
	* 向量p1p 和向量 p1p2的内积 为cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1)
	* 如果cross小于0，说明p点在直线p1p2上的投影在直线的延长线（p1端）上，故最短距离为p到p1的距离
	* 如果cross大于p1p2距离的平方（可根据内积公式推导），说明p点在直线p1p2上的投影在直线的延长线（p2端）上，故最短距离为p到p2的距离
	* 其他情况，则最短距离，表示p到p1p2的垂直距离
	*/
	double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
	if (cross <= 0)
	{
		return (x - x1) * (x - x1) + (y - y1) * (y - y1);
	}

	double d = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	if (cross >= d)
	{
		return (x - x2) * (x - x2) + (y - y2) * (y - y2);
	}

	// px, py为投影的交点，根据三角形的比例公式计算
	double r = cross / d;
	double px = x1 + (x2 - x1) * r;
	double py = y1 + (y2 - y1) * r;

	return (x - px) * (x - px) + (py - y) * (py - y);
}

inline double s_MinLenPt2Field(double x, double y, const FField_t & field)
{
	double x1 = field.Left;
	double y1 = field.Top;
	DPoint_t p1 = field.topLeft();
	DPoint_t p2 = field.bottomLeft();
	DPoint_t p3 = field.bottomRight();
	DPoint_t p4 = field.topRight();

	double len, minlen;
	minlen = len = s_PtToSegDistance(x, y, p1.x, p1.y, p2.x, p2.y);
	len = s_PtToSegDistance(x, y, p2.x, p2.y, p3.x, p3.y);
	if (minlen > len)
		minlen = len;
	len = s_PtToSegDistance(x, y, p3.x, p3.y, p4.x, p4.y);
	if (minlen > len)
		minlen = len;
	len = s_PtToSegDistance(x, y, p4.x, p4.y, p1.x, p1.y);
	if (minlen > len)
		minlen = len;

	return minlen;
}
