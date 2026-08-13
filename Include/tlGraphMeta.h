/**
  ******************************************************************************
  * @文件     
  * @作者 jchq
  * @版本 V1.00.00
  * @日期 2017-07-11
  * @简述   
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#ifndef H_TL_GRAPH_META_H
#define H_TL_GRAPH_META_H

#include "..\Include\tlMarkEngine.h"
#include "..\Include\tlGraphDefines.h"
#include "..\Include\tlGraphRender.h"
#include "..\Include\tlGraphSplit.h"
enum MetaTag{
    emMETA_TAG_FRAME=0,
    emMETA_TAG_FILL,
    emMETA_TAG_JUMP,
    emMETA_TAG_PATH,
	emMETA_TAG_USELESS    // 无用的图元，主要用来起指示作用
};
/* 图元类型 */
typedef enum {
    emTETE_META_DOT = 0,       /* 点*/
    emTETE_META_LINE,		  /* 线段*/
    emTETE_META_RECTANGLE,    /* 矩形,包括正方形*/
    emTETE_META_POLYGON,      /* 多边形*/
    emTETE_META_ARC,          /* 弧*/
    emTETE_META_ELLIPSE,      /* 椭圆,包括圆*/
    emTETE_META_CIRCLE,       /* 圆*/
    emTETE_META_CHORD,        /* 弦*/
    emTETE_META_POLYLINE,     /* 折线*/
    emTETE_META_MPOLYLINE,    /* 多折线,主要用于字符*/
    emTETE_META_PIE,          /* 扇形*/
    emTETE_META_BEZIER_CONIC, /* 二阶贝塞尔曲线*/
    emTETE_META_BEZIER_CUBIC  /* 三阶贝塞尔曲线*/
}emGMeta_t;

class IGraphRender;
class IMarkEngine;
class IFileReader;
class IFileWriter;
/*---- 图元 ----*/
class TL_API IGraphMeta
{
public:
    //--IGraphMeta();
    virtual ~IGraphMeta() {}
    virtual void SetVisible(bool visible=true) { m_IsVisible = visible; };
    virtual bool IsVisible(void) const { return m_IsVisible; };
    virtual void SetMarkable(bool mark=true) { m_IsMarkable = mark; };
    virtual bool IsMarkable(void) const { return m_IsMarkable; };	
    virtual int32_t MetaType(void)const  = 0;
    int32_t MetaTag(void) const { return m_iMetaTag; }
	void SetMetaTag(int nTag) { m_iMetaTag = nTag; }

    virtual IGraphMeta *Clone(void) = 0;
    virtual void Resize(double dbZoomX, double dbZoomY) = 0;
    virtual void Repos(double px, double py) = 0;
    virtual void Rotate(double cx, double cy, double Angle) = 0;
    virtual void Affine(double a,double b) = 0;
    virtual FField_t GetField() = 0;
    virtual void Draw(IGraphRender *pTheRender) = 0;
    virtual int32_t Mark(IMarkEngine *pTheEngine) = 0;
    virtual int32_t Split(IGraphSplit *pTheGraph) = 0;
    virtual bool LoadFile(IFileReader *pTheReader) = 0;
    virtual bool SaveFile(IFileWriter *pTheWriter) = 0;
protected:
    bool m_IsVisible;
    bool m_IsMarkable;
    int32_t m_iMetaTag;
};



#endif
