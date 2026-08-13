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
#ifndef H_LOAD_MARK_DRIVER_H
#define H_LOAD_MARK_DRIVER_H

#include <Windows.h>
#include "tlMarkDriver.h"

class CLoadMarkDriver
{
public:
    CLoadMarkDriver(IPlatform *pPlatform,const char *sName,int32_t iDrvType);
    ~CLoadMarkDriver();

    bool IsOpened(void)const { return m_IsOpened; };
    IMarkDriver *GetDriver(void);

protected:
    typedef void ( *TLM_GetInfo)(int cmd,void *argdx, int argcx);
    typedef int32_t ( *TLM_GetEntity)(IMarkDriver **pTheEntity,int32_t iFlags,void *pOption);

    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    IMarkDriver *m_pMarkDriver;   
};

#endif
