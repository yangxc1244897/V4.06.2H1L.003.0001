/**
  ******************************************************************************
  * @文件     
  * @作者 jchq  
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
#ifndef H_TL_GRAPH_OBJECT_H
#define H_TL_GRAPH_OBJECT_H

#include "tlGraphDefines.h"
#include "tlGraphMeta.h"
#include <vector>
  /*定义对象的类型*/
typedef enum {
    emObjType_SHAPE=0,    /*  几何图形;         */
    emObjType_TEXT,       /*  文本字符;         */
    emObjType_AUTO,       /*  自动变量字符;     */
    emObjType_TIME,       /*  时间字符;         */
    emObjType_D1CODE,     /*  一维条码          */
    emObjType_D2CODE,     /*  二维条码          */
    emObjType_IMAGE,      /*  外部导入文件  */
}emObjType_t;

/*
 * 对象类的基类
 */
class IGraphRender;
class IMarkEngine;
class IFileReader;
class IFileWriter;
class TL_API IGraphObject
{
public:
    //--IGraphObject(void);
    virtual ~IGraphObject(void) {};
    virtual void Init(void) = 0;
    virtual IGraphObject *Clone(void)=0;
    virtual int32_t GetType(void) const = 0;
    virtual void SetLock(bool lock=true) { m_IsLocked = lock; };
    virtual bool IsLocked(void) const { return m_IsLocked; };
    virtual void SetVisible(bool visible=true) { m_IsVisible = visible; };
    virtual bool IsVisible(void) const { return m_IsVisible; };
    virtual void SetSelected(bool sel=true) { m_IsSelected = sel; };
    virtual bool IsSelected(void) const { return m_IsSelected; };
    virtual void SetMarkable(bool mark=true) { m_IsMarkable = mark; };
    virtual bool IsMarkable(void) const { return m_IsMarkable; };
    virtual void SetLayer(int32_t iLayerID) { m_iLayerID = iLayerID; };
    virtual int32_t GetLayer(void) const { return m_iLayerID; };
    virtual void SetID(int32_t iID) { m_iGraphID = iID; };
    virtual int32_t GetID(void) const { return m_iGraphID; };  
	virtual bool GetModified() const { return m_IsModify; }
	virtual void SetModified(bool modify = true) { m_IsModify = modify; }
	
	virtual bool IsMarkFrame(void) const { return m_IsMarkFrame; }
	virtual void SetMarkFrame(bool b) { m_IsMarkFrame = b; }
	virtual void ClearHatch(void) = 0; 
	virtual void AddHatch(const stHatch_t& stHatch) = 0;
	virtual uint32_t GetHatchCount(void) const = 0; 
	virtual bool GetHatch(uint32_t nID, stHatch_t & stHatch) = 0;

	void SetName(const char *sName) { strcpy_s(m_sName, MAX_TXT_SIZE, sName); }
	const char *GetName(void) const { return m_sName; }

	virtual bool IsVariable(void) const = 0;    // 是否为变量
        
    virtual void Draw(IGraphRender *pTheRenderer) = 0;
    virtual int32_t Mark(IMarkEngine *pTheEngine) = 0;
    virtual int32_t Split(IGraphSplit *pTheGraph) = 0;
    virtual bool LoadFile(IFileReader *pTheReader) = 0;
    virtual bool SaveFile(IFileWriter *pTheWriter) = 0;
    //-- 
    virtual int32_t SetConfigInfo(int iType, void *pConfigInfo) = 0;
    virtual int32_t GetConfigInfo(int iType, void *pConfigInfo)const = 0;

	virtual bool ShowAttributeWnd(void) = 0;  //  显示属性框
public://--Object Edit
    virtual void Resize(double dLeft,double dTop,double dRight,double dBottom,int iAbs) = 0;
    virtual void Resize(double dbZoomX,double dbZoomY,bool IsValid=true) = 0;
    virtual void Repos(double px,double py,int iAbs,bool IsValid=true) = 0;
    virtual void Rotate(double cx,double cy,double Angle,bool IsValid=true) = 0;
    virtual void Affine(double a,double b,bool IsValid=true) = 0;
    
    virtual void SetData(void *pData)=0;
    virtual void GetData(void *pData)const=0;
    virtual bool Update(bool IsValid=true) = 0;
    virtual FField_t GetField(void) = 0;

public:  // 图元编辑
    virtual int GetMetaCount() = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stDot_t stData) = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stLine_t stData) = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stPolyline_t stData) = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stPolygon_t stData) = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stRectangle_t stData) = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stEllipse_t stData) = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stCircle_t stData) = 0;
    virtual IGraphMeta *AppendMeta(int32_t iMetaTag,stArc_t stData) = 0;
    virtual IGraphMeta *AppendMeta(IGraphMeta *pTheMeta) = 0;
    virtual void AppendMeta(IGraphObject* pTheObj) = 0;
    virtual IGraphMeta *GetMeta(int32_t iID) = 0;

	virtual bool RemoveAllMeta(void) = 0;
	virtual bool RemoveAllFillMeta(void) = 0;
	virtual IGraphMeta * DetachMeta(int32_t iID) = 0;
	virtual bool DeleteMeta(int iSegID) = 0;
protected:
    bool m_IsLocked;
    bool m_IsVisible;
    bool m_IsSelected;
    bool m_IsMarkable;
	bool m_IsModify;  
	bool m_IsMarkFrame;             // 是否标记外框
    int32_t m_iLayerID;
    int32_t m_iGraphID;
	char m_sName[MAX_TXT_SIZE];   // 对象名称
};

#endif
