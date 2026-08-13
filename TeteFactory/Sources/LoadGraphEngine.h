/**
******************************************************************************
* @文件
* @作者
* @版本 V1.00.00
* @日期 2018-09-12
* @简述
******************************************************************************
* @说明
*
*
*
******************************************************************************
*/
#ifndef H_LOAD_GRAPH_ENGINE_H
#define H_LOAD_GRAPH_ENGINE_H

#include <Windows.h>
#include "tlObjectEngine.h"


class CLoadGraphEngine
{
public:
    CLoadGraphEngine(const char *sName);
    ~CLoadGraphEngine();

    bool IsOpened(void)const { return m_IsOpened; };
    //--stModuleInfo_t GetModuleInfo(void)const { return m_stModuleInfo; };
    IObjectEngine *GetEngine(void) { return m_pGraphEngine; };
protected:
    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    //--stModuleInfo_t m_stModuleInfo;
    IObjectEngine *m_pGraphEngine;
};

class CLoadRenderEngine
{
public:
    CLoadRenderEngine(const char *sName);
    ~CLoadRenderEngine();

    bool IsOpened(void)const { return m_IsOpened; };
    //--stModuleInfo_t GetModuleInfo(void)const { return m_stModuleInfo; };
    IRenderEngine *GetEngine(void) { return m_pRenderEngine; };
protected:
    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    //--stModuleInfo_t m_stModuleInfo;
    IRenderEngine *m_pRenderEngine;

};

#endif
