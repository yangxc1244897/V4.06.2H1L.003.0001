// libMarkDevice.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "tlMarkDriver.h"
#include "tlMarkDevice.h"
#include "LoadGraphEngine.h"

typedef void ( *TLM_GetInfo)(int cmd,void *argdx, int argcx);
typedef int32_t ( *TLM_GetGraph)(IObjectEngine **pTheEngine,int32_t iFlags,void *pOption);
typedef int32_t ( *TLM_GetRender)(IRenderEngine **pTheEngine,int32_t iFlags,void *pOption);

CLoadGraphEngine::CLoadGraphEngine(const char *sName)
{
    m_IsOpened = false;

    m_pGraphEngine = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetGraph FuncGetModule = (TLM_GetGraph)GetProcAddress(m_hInstance, "TLM_GetEntity");

        FuncGetModule(&m_pGraphEngine,0,NULL);
    }
}
CLoadGraphEngine::~CLoadGraphEngine()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}

CLoadRenderEngine::CLoadRenderEngine(const char *sName)
{
    m_IsOpened = false;
 
    m_pRenderEngine = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetRender FuncGetModule = (TLM_GetRender)GetProcAddress(m_hInstance, "TLM_GetEntity"); 
        FuncGetModule(&m_pRenderEngine,0,NULL);
    }
}
CLoadRenderEngine::~CLoadRenderEngine()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}