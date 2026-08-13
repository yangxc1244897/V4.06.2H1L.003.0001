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

#ifndef H_COORD_TRANSFORM_H
#define H_COORD_TRANSFORM_H
#include "tlTeteDefines.h"
#include "tlMarkDefine.h"
 
#define MAX_HEAD_NUM     2  /* 扫描头数量 */
#define MAX_VISION_NUM   2  /* CCD数量    */
#define MAX_MOTION_NUM   1  /* 工作台数量 */
typedef struct _xCoord_t {
    double cx,cy,ca;    //--中心坐标(在基准坐标系中的位置)及角度
    double rx,ry;       //--XY轴正极值
    uint32_t option;    //--[0]:XY互换,[1]:X反向，[2]:Y反向
    struct _xCoord_t() { cx = 0.0, cy = 0.0, ca = 0.0, option = 0; }
    double minx() { return (cx - rx); }
    double maxx() { return (cx + rx); }
    double miny() { return (cy - ry); }
    double maxy() { return (cy + ry); }
}xCoord_t;

//typedef struct _HeadTurn_t {
//	double dx, dy;  //-偏移量
//	double cx, cy, angle;//--旋转中心及角度
//	double fx, fy;  //--缩放因子
//}xHeadTurn_t;

class CCoordTransform
{
public:
    static CCoordTransform *Instance();
    ~CCoordTransform();
    
protected:
    CCoordTransform(void);    //--
private:
    static CCoordTransform *_instance;

public:
    void GetHeadCoord(int32_t iIndex,xCoord_t &xCoord)const;
    void SetHeadCoord(int32_t iIndex,const xCoord_t &xCoord);
	void GetHeadTurn(int32_t iIndex, xHeadTurn_t &xTurn)const;
	void SetHeadTurn(int32_t iIndex, const xHeadTurn_t &xTurn);
    void Base2Head(int32_t iIndex,double &x,double &y);  //--基准坐标转换为扫描坐标
    void Head2Base(int32_t iIndex,double &x,double &y); 
    bool IsInHead(int32_t iIndex,double x,double y);     //--判断点是否在扫描区域内
    bool IsInHead(int32_t iIndex,double x1,double y1,double x2,double y2);//--判断矩形区域是否在扫描区域内

    void GetVisionCoord(int32_t iIndex,xCoord_t &xCoord)const;
    void SetVisionCoord(int32_t iIndex,const xCoord_t &xCoord);
    void Base2Vision(int32_t iIndex,double &x,double &y);  //--基准坐标转换为CCD坐标
    void Vision2Base(int32_t iIndex,double &x,double &y);
    bool IsInVision(int32_t iIndex,double x,double y);     //--判断点是否在视觉区域内
    bool IsInVision(int32_t iIndex,double x1,double y1,double x2,double y2);//--判断矩形区域是否在视觉区域内

    void GetMotionCoord(int32_t iIndex,xCoord_t &xCoord)const;
    void SetMotionCoord(int32_t iIndex,const xCoord_t &xCoord);
    void Base2Motion(int32_t iIndex,double &x,double &y);  //--基准坐标转换为运动平台坐标
    void Motion2Base(int32_t iIndex,double &x,double &y);
    bool IsInMotion(int32_t iIndex,double x,double y);     //--判断点是否在运动区域内
    bool IsInMotion(int32_t iIndex,double x1,double y1,double x2,double y2);//--判断矩形区域是否在运动区域内
private:
    xCoord_t m_xHeadCoord[MAX_HEAD_NUM];
    xCoord_t m_xVisionCoord[MAX_VISION_NUM];
    xCoord_t m_xMotionCoord[MAX_MOTION_NUM];
	xHeadTurn_t m_xHeadTurn[MAX_HEAD_NUM];    // 双头偏移数据
};

#endif

