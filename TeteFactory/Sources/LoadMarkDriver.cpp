// libMarkDevice.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "tlMarkDriver.h"
#include "tlMarkDevice.h"
#include "LoadMarkDriver.h"

CLoadMarkDriver::CLoadMarkDriver(IPlatform *pPlatform,const char *sName,int32_t iDrvType)
{
    m_IsOpened = false;

    m_pMarkDriver = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetEntity FuncGetModule = (TLM_GetEntity)GetProcAddress(m_hInstance, "TLM_GetEntity");

        FuncGetModule(&m_pMarkDriver,iDrvType,pPlatform);
    }
}
CLoadMarkDriver::~CLoadMarkDriver()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}
IMarkDriver *CLoadMarkDriver::GetDriver(void) 
{ 
    return m_pMarkDriver; 
}