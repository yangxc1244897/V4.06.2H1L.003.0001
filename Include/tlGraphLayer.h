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
#ifndef H_TL_GRAPH_LAYER_H
#define H_TL_GRAPH_LAYER_H

#include "tlGraphDefines.h"
#include "tlMarkEngine.h"

class IFileReader;
class IFileWriter;
class TL_API IGraphLayer
{
public:
    //--IGraphLayer(void);
    virtual ~IGraphLayer(void) {};

public:  //--
    virtual void SetName(const char *sName) = 0;
    virtual const char *GetName(void) const = 0;

    virtual void SetLock(bool lock=true) { m_IsLocked = lock; }
    virtual bool IsLocked(void) const { return m_IsLocked; }
    virtual void SetVisible(bool visible=true) { m_IsVisible = visible; }
    virtual bool IsVisible(void) const { return m_IsVisible; };
    virtual void SetMarkable(bool mark=true) { m_IsMarkable = mark; }
    virtual bool IsMarkable(void) const { return m_IsMarkable; }

    virtual void SetPen(IMarkPen *pThePen) = 0;
    virtual IMarkPen *GetPen(void) = 0;

	virtual bool LoadFile(IFileReader *pTheReader) = 0;
	virtual bool SaveFile(IFileWriter *pTheWriter) = 0;
protected:
    bool m_IsLocked;
    bool m_IsVisible;
    bool m_IsMarkable;
};


#endif
