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
#ifndef H_TL_GRAPH_DOC_H
#define H_TL_GRAPH_DOC_H

#include<vector>
#include "tlGraphObject.h"
#include "tlGraphLayer.h"

  /* 对象阵列参数 */
typedef struct ArrayData_T {
    uint32_t nStyle;
    uint32_t nRowCount;
    uint32_t nColCount;

    double dbRowPitch;
    double dbColPitch;
    ArrayData_T(){
        nStyle = 0;
        nRowCount = 2;
        dbRowPitch = 0.0;
        nColCount = 2;
        dbColPitch = 0.0;
    };
}stArrayData_t;

class TL_API IGraphDoc
{
public:
    //--IGraphDoc(void);
    virtual ~IGraphDoc() {};

public:     /*------文件读写：文档、图层、对象的载入与存储-----*/
    virtual void Draw(IGraphRender *pTheRender) = 0;
    virtual int32_t Mark(int32_t iFilter = 0) = 0;
    virtual bool LoadFile(IFileReader *pTheReader) = 0;
    virtual bool SaveFile(IFileWriter *pTheWriter) = 0;
    virtual bool ImportFile(const char *sName) = 0;
    virtual bool ExportFile(const char *sName) = 0;

    virtual bool CopyToClipboard() = 0;       //--复制选中内容至剪切板
    virtual bool CopyFromClipboard() = 0;     //--从剪切板中复制内容至图档

    virtual bool ImportLayers(const char *sFileName) = 0;
    virtual bool ExportLayers(const char *sFileName) = 0;

public:     /*------文档属性操作    -----------*/
    virtual void SetName(const char *sName) = 0;
    virtual const char *GetName(void) const = 0;
    virtual void SetLock(bool lock) { m_IsLocked = lock; }
    virtual bool IsLocked(void) const { return m_IsLocked; }
    virtual void SetMarkable(bool mark) { m_IsMarkable = mark; }
    virtual bool IsMarkable(void) const { return m_IsMarkable; }
    virtual bool GetModified() const { return m_IsModify; }
    virtual void SetModified(bool modify=true) { m_IsModify = modify; }
    virtual bool Redo() = 0;
    virtual bool Undo() = 0;
    
public:     /*-------图层管理 --------------*/
    virtual int32_t GetLayerCount(void)const = 0;
    virtual IGraphLayer *AppendLayer(void) = 0;            //--添加图层
    virtual bool DeleteLayer(int32_t iLayerID) = 0;        //--删除图层
    //--virtual bool DeleteLayer(IGraphLayer *pTheLayer) = 0;  //--删除图层
    virtual void DeleteAllLayer(void) = 0;          //--清空图层

    virtual int32_t ActiveLayerID(void)const = 0;            //--当前图层
    virtual IGraphLayer *GetActiveLayer(void) = 0;       //--当前图层
    virtual IGraphLayer *GetLayer(int32_t iLayerID) = 0; //--指定索引的图层
    virtual IGraphLayer *SetActiveLayer(int32_t iLayerID) = 0;
    virtual IGraphLayer *SetActiveLayer(const char *sLayerName) = 0;
public:     /*--------对象管理----------------*/
    virtual int32_t GetObjCount(void) const = 0;
    virtual int32_t GetObjSelCount(void) const = 0;    
    virtual IGraphObject *AppendObject(int32_t iObjType) = 0;
    virtual void AppendObject(IGraphObject *pTheObj) = 0;
    virtual bool DeleteObject(int32_t iObjID) = 0;
    virtual bool DeleteSelObjects(void) = 0;
    virtual bool DeleteAllObjects(void) = 0; 

    virtual IGraphObject * GetObject(int32_t iObjIndex) = 0;    
    
    virtual void UpdateAllObjects(void) = 0;
    virtual void UpdateSelObjects(void) = 0;

    virtual void MergeSelObjects(void) = 0;   //--合并选中的对象
    virtual void SplitSelObjects(void) = 0;   //--拆分选中的对象
	virtual bool ShowAttributeWnd(void) = 0; // 显示第一个选中对象或文档的属性框
    virtual void ShowArrayWnd( void) = 0;    // 阵列
	virtual void ShowSortWnd(void) = 0;      // 排序
	virtual void ShowHatchWnd(void) = 0;     // 填充
	virtual void ShowRotateWnd(void) = 0;    // 旋转
	virtual void ShowSizeSelWnd(void) = 0;   // 位置大小

public:      /*--------对象选择--------------*/
    virtual bool SelectAllObjects(void) = 0;
    virtual bool UnSelectAllObjects(void) = 0;    
    
public:      /*------对象的缩放与布局--------*/
    virtual FField_t GetAllField(void) = 0;
    virtual FField_t GetSelField(void) = 0;

    virtual void Resise(double dLeft, double dTop, double dRight, double dBottom, int iAbs) = 0;
    virtual void Repos(double px, double py, int iAbs) = 0;         
    virtual void Rotate(double cx, double cy, double angle, int iAbs) = 0;  
    
    virtual void ResiseSel(double dLeft, double dTop, double dRight, double dBottom)= 0;
    virtual void ReposSel(double px, double py, int iAbs)= 0;       
    virtual void RotateSel(double cx, double cy, double angle, int iAbs)= 0;
    //----Objects Layout----
    virtual bool AlignHorLeft(void) = 0;
    virtual bool AlignHorCenter(void) = 0;
    virtual bool AlignHorRight(void) = 0;
    virtual bool AlignVerTop(void) = 0;
    virtual bool AlignVerCenter(void) = 0;
    virtual bool AlignVerBottom(void) = 0;
    virtual bool AlignCenter(void) = 0;

protected:
    int32_t     m_iDocID;       //--文档的索引值
    bool        m_IsMarkable;       //--是否标记
    bool        m_IsLocked;     //--是否锁定
    bool        m_IsModify;     //--是否修改过
};

#endif
