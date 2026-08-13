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
#ifndef H_TL_GRAPH_DEFINES_H
#define H_TL_GRAPH_DEFINES_H

#include <math.h>

#include "tlTeteDefines.h"

typedef struct st_Dot {
    double x;
    double y;
}stDot_t;
typedef struct st_Line {
    double x1, y1;
    double x2, y2;
}stLine_t;

typedef struct st_BezierConic {
    double x1, y1;
    double x2, y2;
    double x3, y3;
}stBezierConic_t;

typedef struct st_BezierCubic {
    double x1, y1;
    double x2, y2;
    double x3, y3;
    double x4, y4;
}stBezierCubic_t;

typedef struct st_Rect {
    double x1, y1;
    double x2, y2;
    double height(void){return fabs(y2-y1);}
    double width(void){return fabs(x2-x1);}
}stRectangle_t;

typedef struct st_Ellipse {
    DPoint_t center;  //--椭圆中心点
    DPoint_t major;   //--X轴右顶点
    DPoint_t minor;   //--Y轴上顶点
    double angle0;  //--起始角
    double angle1;  //--结束角
    st_Ellipse(void) {
        center.x = 0.;
        center.y = 0.;
        major.x = 0.;
        major.y = 0.;
        minor.x = 0.;
        minor.y = 0.;
        angle0 = 0;
        angle1 = TETE_PI * 2;
    };
    void InitData(DPoint_t pt1, DPoint_t pt2) {
        center.x = (pt1.x + pt2.x) / 2;
        center.y = (pt1.y + pt2.y) / 2;
        major.x = pt1.x>pt2.x?pt1.x:pt2.x;
        major.y = center.y;
        minor.x = center.x;
        minor.y = pt1.y>pt2.y?pt1.y:pt2.y;
    };
    void InitData(double x1, double y1, double x2, double y2) 
    {
        center.x = (x1 + x2) / 2;
        center.y = (y1 + y2) / 2;
        major.x = x1>x2?x1:x2;
        major.y = center.y;
        minor.x = center.x;
        minor.y = y1>y2?y1:y2;
    };
    double GetMajorAngle(void)
    {
        double px, py; 
        px = major.x - center.x;    
        py = major.y - center.y;
        double angle = atan2(py, px);
        if (angle > TETE_PI)
            angle = angle - TETE_PI * 2;
        else if(angle <-TETE_PI)
            angle = angle + TETE_PI * 2;
        return angle;
    };
    double GetMajorRadius(void)
    {
        double dx = major.x - center.x;
        double dy = major.y - center.y;
        return sqrt((dx*dx) + (dy*dy));
    };
    double GetMinorRadius(void)
    {
        double dx = minor.x - center.x;
        double dy = minor.y - center.y;
        return sqrt((dx*dx) + (dy*dy));
    };
}stEllipse_t;

typedef struct st_Circle {
    double cx, cy;
    double radius;
}stCircle_t;

typedef struct st_Arc {
    double cx, cy;//--圆心
    double radiusx;
    double radiusy;
    double angle1;
    double angle2;
    bool isCloclWise;  //--是否顺时针
}stArc_t;

typedef struct st_Polyline {
    DPoint_t *ppt;
    int32_t isize;
}stPolyline_t;

typedef struct st_Polygon {
    double x1, y1;
    double x2, y2;
    int32_t iSegNum;
}stPolygon_t;
/* 位置偏移类型 */
enum{    
    emPOS_REL=0, /* 相对位置 */
    emPOS_ABS=1  /* 绝对位置 */
};

typedef enum{
    emFileType_WMF=0,
    emFileType_DXF,
    emFileType_PLT,
    emFileType_AI,
    emFileType_PDF
}ExFileType_t;

typedef enum {
	emRowAlign_LT = 0,   // 左上
	emRowAlign_LM,       // 左中
	emRowAlign_LB,       // 左下
	emRowAlign_MT,       // 中上
	emRowAlign_MM,       // 中中
	emRowAlign_MB,       // 中下
	emRowAlign_RT,       // 右上
	emRowAlign_RM,       // 右中
	emRowAlign_RB,       // 右下
}emRowAlign_t;

//---------------------------------------------------------------------------
typedef enum {
	emGraph_Arrow = 0,     /*箭头*/
	emGraph_Hand,      /*手型工具*/
	emGraph_Whole,     /*显示全图*/
	emGraph_Part,      /*局部放大*/
	emGraph_ZoomIn,    /*放大*/
	emGraph_ZoomOut,   /*缩小*/
	emDraw_Dot,        /*画点*/
	emDraw_Line,       /*画线*/
	emDraw_PolyLine,   /*画折线*/
	emDraw_Arc,        /*画弧*/
	emDraw_Rectangle,  /*画矩形*/
	emDraw_Ellipse,    /*画椭圆*/
	emDraw_Circle,     /*画圆*/
	emDraw_Polygon,    /*画多边形*/
	emDraw_TextFix,    /*画固定文本*/
	emDraw_TextTime,   /*画时间码*/
	emDraw_TextAuto,   /*画流水号*/
	emDraw_D1Code,     /*画条形码*/
	emDraw_D2Code,     /*画二维码*/
	emDraw_Image,      /*导入图片*/
}emDrawType_t;

/*
*对象填充属性
*/
typedef enum {
    emHatchStyle_NULL = 0,
    emHatchStyle_SPOT,
    emHatchStyle_LINE,
    emHatchStyle_RING,
    emHatchStyle_CIRCLE,
    emHatchStyle_SPIRAL
}emHatchStyle_t;

typedef enum {
	emHatchDirection_SAME = 0,
	emHatchDirection_GOBACK,
	emHatchDirection_SEGMENT,
	emHatchDirection_ARCHED
}emHatchDirection_t;

typedef struct _ST_Hatch {
    bool     IsEnable;
    int32_t  iColor;                 //--颜色
    double   dbBorderPitch;       //--填充与边框的距离
    double   dbFillPitch;         //--填充线间的距离
    int32_t  iFillAvgDis;            //--是否平均分配 (0: 设定值分配模式, 1: 平均分配
    int32_t  iFillTimes;             //--設定填滿的次數(若小於1則不填滿)-->
    double   dbFillAngleStart;    //--設定填滿的起始角度 (填滿的起始角度 [度]，範圍0~360)-->
    double   dbFillAngleStep;     //--設定填滿的累進角度 (填滿的累進角度 [度]，範圍0~360)-->
    int32_t  iFillStyle;             //--填滿雕刻模式 參考 001_FillStyle.PNG-->
    int32_t  iFillLoops;             //--填滿雕刻模式若是環狀,則在外框向內畫幾環後再做其他的填滿-->
    int32_t  iInsideFirst;           //--設定島型填滿是否由內向外填滿 (0: 由外向內填滿 1: 由內向外填滿)-->
    int32_t  iMarkInversely;         //-- 設定反相雕刻 0:無反相雕刻 1:有反相雕刻 -->
    int32_t  iFillDirection;         //-- 填充方向 0：同相填充  1：来回填充
}stHatch_t;

class FField_t {
public:
    FField_t() : Left(0.0), Top(-0.01), Right(-0.01), Bottom(0.0) {};
    inline FField_t(double left, double top, double width, double height) : Left(left), Top(top), Right(left + width), Bottom(top - height) {}
    inline FField_t(const DPoint_t &atopLeft, const DPoint_t &abottomRight) : Left(atopLeft.x), Top(atopLeft.y), Right(abottomRight.x), Bottom(abottomRight.y) {}

    inline bool isEmpty() const { return Left > Right || Top < Bottom; }

    inline void setLeft(double pos) { Left = pos; }
    inline void setTop(double pos) { Top = pos; }
    inline void setRight(double pos) { Right = pos; }
    inline void setBottom(double pos) { Bottom = pos; }
    inline void setTopLeft(const DPoint_t &p) { Left = p.x; Top = p.y; }
    inline void setBottomRight(const DPoint_t &p) { Right = p.x; Bottom = p.y; }
    inline void setTopRight(const DPoint_t &p) { Right = p.x; Top = p.y; }
    inline void setBottomLeft(const DPoint_t &p) { Left = p.x; Bottom = p.y; }

    inline double CenterX(void) { return ((Left + Right) / 2.0); }
    inline double CenterY(void) { return ((Top + Bottom) / 2.0); }
    inline DPoint_t topLeft() const { return DPoint_t(Left, Top); }
    inline DPoint_t bottomRight() const { return DPoint_t(Right, Bottom); }
    inline DPoint_t topRight() const { return DPoint_t(Right, Top); }
    inline DPoint_t bottomLeft() const { return DPoint_t(Left, Bottom); }
    inline DPoint_t center() const { return DPoint_t(((Left)+Right) / 2., ((Top)+Bottom) / 2.); }
    inline double posX(void) const { return Left; };
    inline double posY(void) const { return Top; };
    inline double width() const { return  (Right - Left); }
    inline double height() const { return (Top - Bottom); }

    inline bool isAcross(FField_t stField) const {
        return !((Left > stField.Right)
            || (Right < stField.Left)
            || (Top < stField.Bottom)
            || (Bottom > stField.Top));
    }
    inline bool isInside(double px, double py) const {
        return !((px<Left) || (px>Right)
            || (py<Bottom) || (py>Top));
    }
    inline bool isInside(FField_t stField) const {
        return !((stField.Left<Left) || (stField.Right>Right)
              || (stField.Bottom<Bottom) || (stField.Top>Top));
    }

    FField_t& operator =(const FField_t& field)
    {
        Left = field.Left;
        Top = field.Top;
        Right = field.Right;
        Bottom = field.Bottom;

        return *this;
    }
    FField_t operator +(const FField_t& OriR)
    {
        FField_t temR;

        temR.Left = min(Left, OriR.Left);
        temR.Top = max(Top, OriR.Top);
        temR.Right = max(Right, OriR.Right);
        temR.Bottom = min(Bottom, OriR.Bottom);

        return temR;
    }

public:
    double Left;
    double Top;
    double Right;
    double Bottom;
};

typedef struct _ST_CodeRemap {
	/*int iCodeID;*/
	int32_t iCodeType;  /* 0:不支持类型；-1:见底*/
	char sCodeName[55];
}stCodeRemap_t;

/*自动增量结构体*/
typedef enum
{
	BASETYPE_OCT = 0,
	BASETYPE_DEC,
	BASETYPE_HEX_LOWER,
	BASETYPE_HEX_UPPER
}emBaseType_t;
typedef struct _stAutoData
{
	char m_sPreFix[256];		//-- 字首 
	char m_sSufFix[256];		//-- 字尾 
	int m_iAutoValue;       //--当前值
	int m_iStart;			//-- 起始值
	int m_iStep;			//-- 步进量
	int m_iEnd;				//-- 结束值
	int m_iDigit;			//-- 自动数值位数
	int m_iLead;			//-- 填补字符
	int m_iRepeat;			//-- 总需重复次数
	bool m_IsRecycle;		//-- 自动循环
	emBaseType_t m_iBaseType;	    //-- 进制类型
	int m_iRepeated;  	    //-- 已重复次数
	_stAutoData()
	{
		memset(m_sPreFix, 0, 256);
		memset(m_sSufFix, 0, 256);
		m_iStart = 0;
		m_iStep = 1;
		m_iEnd = 65535;
		m_iDigit = 3;
		m_iLead = '0';
		m_iRepeat = 2;
		m_IsRecycle = 0;
		m_iBaseType = BASETYPE_DEC;
		m_iRepeated = 0;
		m_iAutoValue = m_iStart;
	}
}stAutoData_t;

// 文本变量类型
typedef enum
{
	TEXT_OBJECT_NORMAL = 0,  // 普通文本
	TEXT_OBJECT_INDEX,       // 序号
}emTextObjectType_t;

#endif
 