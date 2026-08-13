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
#ifndef H_TL_GRAPH_SPLIT_H
#define H_TL_GRAPH_SPLIT_H

#include "..\Include\tlMarkEngine.h"
#include "..\Include\tlGraphDefines.h"
#include "..\Include\tlGraphRender.h"


class IRenderer;
class IMarkEngine;
class IFileReader;
class IFileWriter;
class TL_API IGraphSplit
{
public:
    //--IGraphSplit();
    virtual ~IGraphSplit(){};

    virtual void SplitGraph(DPoint_t stData) = 0;
    virtual void SplitGraph(double x1,double y1,double x2,double y2) = 0;
    virtual void SplitGraph(DPoint_t *ppt,int32_t iCount) = 0;
    virtual void SplitGraph(stEllipse_t stData) = 0;
    virtual void SplitGraph(stCircle_t stData) = 0;
    virtual void SplitGraph(stArc_t stData) = 0;

    virtual void SetMarkPen(IMarkPen *pThePen) = 0;
    virtual void MarkGraph(IMarkEngine *pTheEngine) = 0;

    virtual void Clear() = 0;
};



#endif
