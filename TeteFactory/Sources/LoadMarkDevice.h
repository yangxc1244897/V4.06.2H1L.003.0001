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
#ifndef H_LOAD_MARK_DEVICE_H
#define H_LOAD_MARK_DEVICE_H

#include <Windows.h>
#include "tlMarkDevice.h"


class CLoadMarkDevice
{
public:
    CLoadMarkDevice(const char *sName);
    ~CLoadMarkDevice();

    bool IsOpened(void)const { return m_IsOpened; };
    IMarkDevice *GetDevice(void) { return m_pMarkDevice; };
protected:
    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    IMarkDevice *m_pMarkDevice;

    typedef void ( *TLM_GetInfo)(int cmd,void *argdx, int argcx);
    typedef int32_t ( *TLM_GetEntity)(IMarkDevice **pTheEntity,int32_t iFlags,void *pOption);
};

#endif
