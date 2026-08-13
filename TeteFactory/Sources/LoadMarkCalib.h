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
#ifndef H_LOAD_MARK_CALIB_H
#define H_LOAD_MARK_CALIB_H

#include <Windows.h>
#include "tlMarkCalib.h"

class CLoadMarkCalib
{
public:
    CLoadMarkCalib(const char *sName);
    ~CLoadMarkCalib();

    bool IsOpened(void)const { return m_IsOpened; };
    /*stModuleInfo_t GetModuleInfo(void)const { return m_stModuleInfo; };*/
    IMarkCalib *GetMasterCalib(void) { return m_pMasterCalib; };
    IMarkCalib *GetSlaverCalib(void) { return m_pSlaverCalib; };
protected:
    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    //--stModuleInfo_t m_stModuleInfo;
    IMarkCalib *m_pMasterCalib;
    IMarkCalib *m_pSlaverCalib;

    typedef void ( *TLM_GetInfo)(int cmd,void *argdx, int argcx);
    typedef int32_t ( *TLM_GetEntity)(IMarkCalib **pTheEntity,int32_t iFlags,void *pOption);
};

#endif
