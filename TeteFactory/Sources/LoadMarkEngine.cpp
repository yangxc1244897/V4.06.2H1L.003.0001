// libMarkDevice.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "tlMarkDriver.h"
#include "tlMarkDevice.h"
#include "LoadMarkEngine.h"

CLoadMarkEngine::CLoadMarkEngine(const char *sName)
{
    m_IsOpened = false;
    m_pMarkEngine = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetEntity FuncGetModule = (TLM_GetEntity)GetProcAddress(m_hInstance, "TLM_GetEntity");
        FuncGetModule(&m_pMarkEngine,0,NULL);
    }
}
CLoadMarkEngine::~CLoadMarkEngine()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}
/***********************************************************************************/
CLoadMotionDriver::CLoadMotionDriver(const char *sName)
{
    m_IsOpened = false;
    m_pMotionDriver = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetEntity FuncGetModule = (TLM_GetEntity)GetProcAddress(m_hInstance, "TLM_GetEntity");

        FuncGetModule(&m_pMotionDriver,0,NULL);
    }
}
CLoadMotionDriver::~CLoadMotionDriver()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}
/*****************************************************************************/
CLoadVisionDriver::CLoadVisionDriver(const char *sName)
{
    m_IsOpened = false;
    m_pVisionDriver = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetEntity FuncGetModule = (TLM_GetEntity)GetProcAddress(m_hInstance, "TLM_GetEntity");

        FuncGetModule(&m_pVisionDriver,0,NULL);
    }
}
CLoadVisionDriver::~CLoadVisionDriver()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}