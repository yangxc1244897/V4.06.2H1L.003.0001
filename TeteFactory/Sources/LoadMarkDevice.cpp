// libMarkDevice.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "tlMarkDriver.h"
#include "tlMarkDevice.h"
#include "LoadMarkDevice.h"

CLoadMarkDevice::CLoadMarkDevice(const char *sName)
{
    m_IsOpened = false;
    m_pMarkDevice = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetEntity FuncGetModule = (TLM_GetEntity)GetProcAddress(m_hInstance, "TLM_GetEntity");
        FuncGetModule(&m_pMarkDevice,0,NULL);
    }
}
CLoadMarkDevice::~CLoadMarkDevice()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}