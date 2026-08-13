/**
  ******************************************************************************
  * @文件 fMarkFile.h    
  * @作者 jchq 
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述 文件读写,文档管理  
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *  
  *
  * 
  ******************************************************************************
  */ 
#ifndef H_TL_GRAPH_ENGINE_H
#define H_TL_GRAPH_ENGINE_H
//---------------------------------------------------------------------------
#include <stdint.h>
#include <list>
#include "tlGraphDefines.h"
#include "tlGraphDoc.h"

class TL_API IGraphEngine
{
public:
    virtual ~IGraphEngine() {};

public:
    //----文件读写----
    virtual bool SaveFile(const char *sFileName) = 0;   //--保存文件,文件名中必须含有扩展名
    virtual bool LoadFile(const char *sFileName) = 0;   //--载入文件,文件名中必须含有扩展名

    //-- 标记
    virtual int32_t Open(IPlatform *pPlatform) = 0;
    virtual int32_t Close(void) = 0;
    virtual int32_t GetEngineCtrl(int iCmd, void *pArg)=0;
    virtual int32_t SetEngineCtrl(int iCmd, void *pArg)=0;

    virtual int32_t MarkActiveDoc(int32_t iFilter=0) = 0;
    virtual int32_t MarkDoc(int32_t iDocIndex,int32_t iFilter=0) = 0;

public:
    virtual IGraphDoc* AppendDoc(const char *sDocName) = 0;    //--添加新文档        
    virtual void RemoveDoc(const char *sDocName) = 0;         //--删除指定索引的文档
    virtual void RemoveDoc(int32_t iDocID) = 0;               //--删除指定索引的文档  
    virtual void RemoveAllDoc(void) = 0;

    virtual IGraphDoc* GetActiveDoc(void) = 0;              //--获取当前文档的指针
    virtual IGraphDoc* GetDoc(int32_t iDocID) = 0;              //--获取指定索引的文档
    virtual IGraphDoc* GetDoc(const char *sDocName) = 0;       //--获取指定名称的文档
    virtual IGraphDoc* SetActiveDoc(int32_t iDocID) = 0;        //--指定索引的文档为当前文档
    virtual IGraphDoc* SetActiveDoc(const char *sDocName) = 0; //--指定名称的文档为当前文档

    virtual int32_t GetDocIndex(void)const = 0;                  //--获取当前文档的索引值
    virtual int32_t GetDocCount(void)const = 0;                  //--获取文档的个数

	virtual bool GetModified(void) = 0;       // 获取文件是否已经被修改
	virtual void ResetModified(void) = 0;     // 保存文件后，复位修改标志

    virtual const char * GetFileName(void) = 0;  // 获取文件保存名称
    virtual void SetFileName(const char * sName = "") = 0;  // 设置文件名称
};


#endif
