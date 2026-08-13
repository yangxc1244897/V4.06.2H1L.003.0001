// libMarkDevice.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "tlMarkDriver.h"
#include "tlMarkDevice.h"
#include "LoadMarkCalib.h"

CLoadMarkCalib::CLoadMarkCalib(const char *sName)
{
    m_IsOpened = false;
    m_pMasterCalib = NULL;
    m_pSlaverCalib = NULL;
    m_hInstance = NULL;
    m_hInstance = LoadLibraryA(sName);
    if (NULL != m_hInstance)
    {
        m_IsOpened = true;
        TLM_GetInfo FuncModuleInfo = (TLM_GetInfo)GetProcAddress(m_hInstance, "TLM_GetInfo");
        TLM_GetEntity FuncGetModule = (TLM_GetEntity)GetProcAddress(m_hInstance, "TLM_GetEntity");

        FuncGetModule(&m_pMasterCalib,0,NULL);
        FuncGetModule(&m_pSlaverCalib,0,NULL);
        int32_t iHeadSel = 0;
        if (NULL != m_pMasterCalib)
            m_pMasterCalib->SetCalibCtrl(MC_CALIB_HEAD_SEL, &iHeadSel);
        iHeadSel = 1;
        if (NULL != m_pSlaverCalib)
            m_pSlaverCalib->SetCalibCtrl(MC_CALIB_HEAD_SEL, &iHeadSel);
    }
}
CLoadMarkCalib::~CLoadMarkCalib()
{
    if (NULL != m_hInstance)
    {
        m_IsOpened = false;
        FreeLibrary(m_hInstance);
        m_hInstance = NULL;
    }
}