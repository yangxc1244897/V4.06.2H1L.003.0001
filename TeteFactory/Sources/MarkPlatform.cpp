/**
******************************************************************************
* @模块 
* @作者 jchq
* @版本 V1.00.00
* @日期 2018-09-21
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明 
*    
*    
*    
*      
*        
*    
*    
******************************************************************************
*/
#include "stdafx.h"
#include "..\stdafx.h"
#include <stdlib.h>
#include "MarkPlatform.h"
#include "Configure.h"
#include "Common\fIniFile.h"
#include "LaserModule.h"
#include "Common\LogServer.h"

static char g_sCurrPath[MAX_PATH];
CMarkPlatform* CMarkPlatform::_instance = NULL;
//---------------------------------------------------------------------------
CMarkPlatform *CMarkPlatform::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CMarkPlatform();
    }
    return _instance;
}
CMarkPlatform::CMarkPlatform()
{    
    m_pTheMsgWnd = NULL;
    m_pLoadMotionDriver = NULL;
    m_pLoadVisionDriver = NULL;
    LoadConfig();    
}
CMarkPlatform::~CMarkPlatform()
{
    SaveConfig();
}
void CMarkPlatform::LoadConfig(void)
{
    /*CIniFile * pIniFile = new CIniFile();

    delete pIniFile;*/
}
void CMarkPlatform::SaveConfig(void)
{
    /*CIniFile * pIniFile = new CIniFile();

    delete pIniFile;*/
}
int32_t CMarkPlatform::Open(void)
{
    int32_t iError = 0;
    CLogServer::Instance()->SetPrinter((emLogPrinter_t)CConfigure::Instance()->m_iLogType);
    CLogServer::Instance()->SetLevel((emLogLevel_t)CConfigure::Instance()->m_iLogLevel);

    m_pLoadMarkCalib = new CLoadMarkCalib(GetSystemDirFile("tlMarkCalib.dll"));
    if(CConfigure::Instance()->m_IsAutoDriver)
        m_pLoadMarkDriver = new CLoadMarkDriver(this,GetSystemDirFile("tlMarkDriver.dll"),-1);
    else
        m_pLoadMarkDriver = new CLoadMarkDriver(this,GetSystemDirFile("tlMarkDriver.dll"),CConfigure::Instance()->m_iDriverType);

    CLaserModule laserModules;
    tlModule_t mod = laserModules.GetDefault();
    CStringA sDevFile;
    sDevFile.Format("%s\\Module\\%s", GetSystemPath(), mod.sFile);
    if(TRUE==PathFileExistsA(sDevFile))
        m_pLoadMarkDevice = new CLoadMarkDevice(sDevFile);
    else
        m_pLoadMarkDevice = new CLoadMarkDevice(GetSystemDirFile("tlMarkDevice.dll"));

    m_pLoadMarkEngine = new CLoadMarkEngine(GetSystemDirFile("tlMarkEngine.dll"));
    m_pLoadGraphEngine = new CLoadGraphEngine(GetSystemDirFile("tlObjectEngine.dll"));
    m_pLoadRenderEngine = new CLoadRenderEngine(GetSystemDirFile("tlGraphRender.dll"));
    if (CConfigure::Instance()->m_IsMotionEnable)
        m_pLoadMotionDriver = new CLoadMotionDriver(GetSystemDirFile("tlMotionDriver.dll"));
    else
        m_pLoadMotionDriver = NULL;
   // if (CConfigure::Instance()->m_IsVisionEnable)
        m_pLoadVisionDriver = new CLoadVisionDriver(GetSystemDirFile("tlVisionDriver.dll"));
    //else
    //    m_pLoadVisionDriver = NULL;

    return iError;
}
int32_t CMarkPlatform::Close(void)
{
    return 0;
}
int32_t CMarkPlatform::Notify(int32_t iUser, int32_t iCmd, void *pMsg)
{
    return 0;
}

emLogLevel_t CMarkPlatform::switchMsgTypeToLogType(const emMsgType_t& emMsgType) const
{
	switch (emMsgType)
	{
	case emMSG_TYPE_INFO:
		return emLOG_LEVEL_INFO;
		break;
	case emMSG_TYPE_WARN:
		return emLOG_LEVEL_WARN;
		break;
	case emMSG_TYPE_ERROR:
		return emLOG_LEVEL_ERROR;
		break;
	case emMSG_TYPE_FATAL:
		return emLOG_LEVEL_FATAL;
		break;
	default:
		return emLOG_LEVEL_INFO;
		break;
	}
}

emFactoryMsgType_t CMarkPlatform::switchMsgTypeToFactoryMsgType(const emMsgType_t& emMsgType) const
{
	switch (emMsgType)
	{
	case emMSG_TYPE_INFO:
		return emMsgType_Normal;
		break;
	case emMSG_TYPE_WARN:
		return emMsgType_Warn;
		break;
	case emMSG_TYPE_ERROR:
		return emMsgType_Error;
		break;
	case emMSG_TYPE_FATAL:
		return emMsgType_Error;
		break;
	default:
		return emMsgType_Normal;
		break;
	}
}

int32_t CMarkPlatform::Message(emMsgType_t emMsgType,const char * strOutputString,...)
{
    char strBuffer[1024] = {'\0'};
    va_list vlArgs;
    va_start(vlArgs,strOutputString);
    _vsnprintf(strBuffer, sizeof(strBuffer), strOutputString,vlArgs);
    va_end(vlArgs);
    //--OutputDebugStringA(strBuffer);
    if (NULL != m_pTheMsgWnd)
        m_pTheMsgWnd->PrintMesMessage(switchMsgTypeToFactoryMsgType(emMsgType), CString(strBuffer));
    return 0;
}
void CMarkPlatform::PrintLog(emLogLevel_t emLevel, const char * strOutputString, ...)
{
    char strBuffer[1024] = {'\0',};
    va_list vlArgs;
    va_start(vlArgs,strOutputString);

    char TimeBuffer[256];
    SYSTEMTIME systime;
    GetLocalTime(&systime);
    sprintf(TimeBuffer, "(%04u%02u%02u %u:%u:%u.%u) ", 
        systime.wYear, systime.wMonth, systime.wDay, 
        systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
    int len = strlen(TimeBuffer);
    memcpy(strBuffer, TimeBuffer, strlen(TimeBuffer));
    _vsnprintf(strBuffer+len, sizeof(strBuffer), strOutputString,vlArgs);
    va_end(vlArgs);
    int iLen = strlen(strBuffer);
    if('\n'!=strBuffer[iLen-1])
        strcat(strBuffer, "\n");
    switch(emLevel)
    {
    case emLOG_LEVEL_DEBUG:
        CLogServer::Instance()->debug(strBuffer);
        break;
    case emLOG_LEVEL_INFO:
        CLogServer::Instance()->info(strBuffer);
        break;
    case emLOG_LEVEL_WARN:
        CLogServer::Instance()->warn(strBuffer);
        break;
    case emLOG_LEVEL_ERROR:
        CLogServer::Instance()->error(strBuffer);
        break;
    case emLOG_LEVEL_FATAL:
        CLogServer::Instance()->fatal(strBuffer);
        break;
    default:
        break;
    }
    //--OutputDebugStringA(strBuffer);
}
const char *CMarkPlatform::GetSystemPath(void)
{
    char sPath[MAX_PATH];
    GetModuleFileNameA(NULL,sPath ,MAX_PATH);
    char sdrive[_MAX_DRIVE];
    char sdir[_MAX_DIR];
    char sname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath(sPath, sdrive, sdir, sname, ext);
    strcpy(g_sCurrPath, sdrive);
    strcat(g_sCurrPath, sdir);
    return g_sCurrPath;
}
const char *CMarkPlatform::GetSystemPath(const char *sSubPath)
{
    char sPath[MAX_PATH];
    GetModuleFileNameA(NULL,sPath ,MAX_PATH);
    char sdrive[_MAX_DRIVE];
    char sdir[_MAX_DIR];
    char sname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath(sPath, sdrive, sdir, sname, ext);
    strcpy(g_sCurrPath, sdrive);
    strcat(g_sCurrPath, sdir);
    strcat(g_sCurrPath, sSubPath);
    return g_sCurrPath;
}
void CMarkPlatform::SystemIdle(void)
{
    MSG msg;
    if(0!=PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
void CMarkPlatform::Delay(uint32_t ms)
{
    ULONGLONG dwStart = GetTickCount(); 
    ULONGLONG dwEnd = dwStart; 
    do { 
        MSG msg;
        if(0!=PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        dwEnd = GetTickCount()-dwStart; 
    }while(dwEnd<ms);
}
void CMarkPlatform::Delay(uint32_t min, uint32_t sec, uint32_t ms)
{
    uint32_t secs = min * 60 + sec;
    for (uint32_t s = 0; s < secs; s++)
        Delay(1000);
    Delay(ms);
}
IConfigure *CMarkPlatform::Configure(void)
{
    return CConfigure::Instance();
}
IMarkCalib *CMarkPlatform::MasterCalib(void)
{
    if (m_pLoadMarkCalib->IsOpened())
        return m_pLoadMarkCalib->GetMasterCalib();
    else
        return NULL;
}
IMarkCalib *CMarkPlatform::SlaverCalib(void)
{
    if (m_pLoadMarkCalib->IsOpened())
        return m_pLoadMarkCalib->GetSlaverCalib();
    else
        return NULL;
}
IMarkDriver *CMarkPlatform::MarkDriver(void)
{
	if (nullptr == m_pLoadMarkDriver)
		return nullptr;
    if (m_pLoadMarkDriver->IsOpened())
        return m_pLoadMarkDriver->GetDriver();
    else
        return NULL;
}
IMarkDevice *CMarkPlatform::MarkDevice(void)
{
	if (nullptr == m_pLoadMarkDevice)
		return nullptr;
    if (m_pLoadMarkDevice->IsOpened())
        return m_pLoadMarkDevice->GetDevice();
    else
        return NULL;
}
IMarkEngine *CMarkPlatform::MarkEngine(void)
{
	if (nullptr == m_pLoadMarkEngine)
		return nullptr;

    if (m_pLoadMarkEngine->IsOpened())
        return m_pLoadMarkEngine->GetEngine();
    else
        return NULL;
}
IMotionDriver *CMarkPlatform::MotionDriver(void)
{
    if (NULL != m_pLoadMotionDriver && m_pLoadMotionDriver->IsOpened())
        return m_pLoadMotionDriver->GetMotion();
    else
        return NULL;
}
IVisionDriver *CMarkPlatform::VisionDriver(void)
{
    if (NULL != m_pLoadVisionDriver && m_pLoadVisionDriver->IsOpened())
        return m_pLoadVisionDriver->GetVision();
    else
        return NULL;
}
IObjectEngine *CMarkPlatform::ObjectEngine(void)
{
    if (m_pLoadGraphEngine->IsOpened())
        return m_pLoadGraphEngine->GetEngine();
    else
        return NULL;
}
IRenderEngine *CMarkPlatform::RenderEngine(void)
{
    if (m_pLoadRenderEngine->IsOpened())
        return m_pLoadRenderEngine->GetEngine();
    else
        return NULL;
}
int32_t CMarkPlatform::GetPlatformCtrl(int32_t iCmd, void *pArg)
{
    int32_t iError(0);
    switch (iCmd)
    {
    case 0:break;
    default:break;
    }
    return iError;
}
int32_t CMarkPlatform::SetPlatformCtrl(int32_t iCmd, void *pArg)
{
    int32_t iError(0);
    switch (iCmd)
    {
    case 0:break;
    default:break;
    }
    return iError;
}

void CMarkPlatform::MsgWaitForThread(HANDLE * p)
{
	// 等待标记线程结束
	while (true)
	{
		DWORD dwRet = MsgWaitForMultipleObjectsEx(1, p, INFINITE, QS_ALLINPUT, 0);
		if (dwRet == WAIT_OBJECT_0 + 1)
		{
			CMarkPlatform::Instance()->SystemIdle();
		}
		else if (dwRet == WAIT_OBJECT_0)
		{
			return;
		}
	}
}