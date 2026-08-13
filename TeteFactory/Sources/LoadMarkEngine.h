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
#ifndef H_LOAD_MARK_ENGINE_H
#define H_LOAD_MARK_ENGINE_H

#include <Windows.h>
#include "tlMarkEngine.h"
#include "tlMotionDriver.h"
#include "tlVisionDriver.h"

class CLoadMarkEngine
{
public:
    CLoadMarkEngine(const char *sName);
    ~CLoadMarkEngine();

    bool IsOpened(void)const { return m_IsOpened; };
    IMarkEngine *GetEngine(void) { return m_pMarkEngine; };
protected:
    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    IMarkEngine *m_pMarkEngine;

    typedef void ( *TLM_GetInfo)(int cmd,void *argdx, int argcx);
    typedef int32_t ( *TLM_GetEntity)(IMarkEngine **pTheEntity,int32_t iFlags,void *pOption);
};

class CLoadMotionDriver
{
public:
    CLoadMotionDriver(const char *sName);
    ~CLoadMotionDriver();

    bool IsOpened(void)const { return m_IsOpened; };
    IMotionDriver *GetMotion(void) { return m_pMotionDriver; };
protected:
    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    IMotionDriver *m_pMotionDriver;

    typedef void ( *TLM_GetInfo)(int cmd,void *argdx, int argcx);
    typedef int32_t ( *TLM_GetEntity)(IMotionDriver **pTheEntity,int32_t iFlags,void *pOption);

};

class CLoadVisionDriver
{
public:
    CLoadVisionDriver(const char *sName);
    ~CLoadVisionDriver();

    bool IsOpened(void)const { return m_IsOpened; };
    IVisionDriver *GetVision(void) { return m_pVisionDriver; };
protected:
    HINSTANCE  m_hInstance;
    bool m_IsOpened;
    IVisionDriver *m_pVisionDriver;

    typedef void ( *TLM_GetInfo)(int cmd,void *argdx, int argcx);
    typedef int32_t ( *TLM_GetEntity)(IVisionDriver **pTheEntity,int32_t iFlags,void *pOption);
};

#endif
