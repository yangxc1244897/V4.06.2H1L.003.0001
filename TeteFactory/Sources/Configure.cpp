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
#include <stdlib.h>
#include "Common\fIniFile.h"
#include "Configure.h"

static char g_sCurrPath[MAX_PATH];
CConfigure* CConfigure::_instance = NULL;
//---------------------------------------------------------------------------
CConfigure *CConfigure::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CConfigure();
    }
    return _instance;
}

void CConfigure::Delete()
{
	if (NULL != _instance)
	{
		delete _instance;
		_instance = NULL;
	}
}

CConfigure::CConfigure()
{    
    LoadConfig();    
}
CConfigure::~CConfigure()
{
    SaveConfig();
}
void CConfigure::LoadConfig(void)
{
    strcpy(m_sFileName,GetSystemDirFile("config\\Configure.ini"));
   
	m_nSoftwareMode = GetInt8Value("SYS_CONFIG", "SoftwareMode", 0);  // 默认工厂模式
    m_IsAutoRun = GetBoolValue("SYS_CONFIG", "AutoRun", 0);
    m_IsAutoLogin  = GetBoolValue("SYS_CONFIG", "AutoLogin", 1);
    m_IsStoreMM    = GetBoolValue("SYS_CONFIG", "StorePsw", 1);
    m_IsShowPath   = GetBoolValue("SYS_CONFIG", "ShowPath", 1);
    m_IsShowGrid   = GetBoolValue("SYS_CONFIG", "ShowGrid", 1);
    m_IsShowAcross = GetBoolValue("SYS_CONFIG", "ShowAcross", 1);
    m_IsShowAxis   = GetBoolValue("SYS_CONFIG", "ShowAxis", 1);
    m_iLogType    = GetInt32Value("SYS_CONFIG", "LogType", 0);
    m_iLogLevel   = GetInt32Value("SYS_CONFIG", "LogLevel", 0);

    m_iAxisMode     = GetInt32Value("SYS_CONFIG", "AxisMode", 0);
    m_dbAxisLimitX  = GetFloatValue("SYS_CONFIG", "AxisLimitX", 75.0);
    m_dbAxisLimitY  = GetFloatValue("SYS_CONFIG", "AxisLimitY", 75.0);
    m_dbAxisShiftX  = GetFloatValue("SYS_CONFIG", "AxisShiftX", 0.0);
    m_dbAxisShiftY  = GetFloatValue("SYS_CONFIG", "AxisShiftY", 0.0);
    m_dbAxisShiftA  = GetFloatValue("SYS_CONFIG", "AxisShiftA", 0.0);

    m_dbMoveStep = GetFloatValue("SYS_CONFIG", "MoveStep", 0.01);

    m_IsSingleDoc = GetBoolValue("DEV_CONFIG", "SingleDoc", 1);
    m_IsScannerEnable = false;  //--扫描枪扫描标记使能
    m_IsNetworkEnable = false;    //--网络标记使能
    m_IsPedalEnable = GetBoolValue("DEV_CONFIG", "PedalEnable", 1);
    m_IsOnlineEnable = GetBoolValue("DEV_CONFIG", "OnlineEnable", 1); 

    m_IsAutoDriver = GetBoolValue("HDW_CONFIG", "AutoDriver", 1);
    m_iDriverType  = GetInt32Value("HDW_CONFIG", "DriverType", 0);
    m_iScanerType  = GetInt32Value("HDW_CONFIG", "ScanerType", 0);
    m_iMainCtrlVer = GetInt32Value("HDW_CONFIG", "MainCtrlVer", 0);
    m_IsMotionEnable = GetBoolValue("HDW_CONFIG", "MotionEnable", 1);
    m_iMotionType  = GetInt32Value("HDW_CONFIG", "MotionType", 0);
    m_IsVisionEnable = GetBoolValue("HDW_CONFIG", "VisionEnable", 1);
    m_iVisionType  = GetInt32Value("HDW_CONFIG", "VisionType", 0);

	m_dbNinePRadius = GetDoubleValue("CCDAnchor", "Nine_Radius", 0.5);
	m_dbNinePPitch = GetDoubleValue("CCDAnchor", "Nine_Pitch", 1.5);
	m_dbNinePFillPitch = GetDoubleValue("CCDAnchor", "Nine_FillPitch", 0.05);
	m_dbNinePCenterX = GetDoubleValue("CCDAnchor", "Nine_CenterX", 0);
	m_dbNinePCenterY = GetDoubleValue("CCDAnchor", "Nine_CenterY", 0);
	m_dbNinePCenterX2 = GetDoubleValue("CCDAnchor", "Nine_CenterX2", 0);
	m_dbNinePCenterY2 = GetDoubleValue("CCDAnchor", "Nine_CenterY2", 0);

	m_dbAnchorRadius = GetDoubleValue("CCDAnchor", "Radius", 1);
	m_dbAnchorFillPitch = GetDoubleValue("CCDAnchor", "FillPitch", 0.05);

	m_dbAnchorCalibAX = GetDoubleValue("CCDAnchor", "OffsetA_X", 0);
	m_dbAnchorCalibAY = GetDoubleValue("CCDAnchor", "OffsetA_Y", 0);
	m_dbAnchorCalibBX = GetDoubleValue("CCDAnchor", "OffsetB_X", 0);
	m_dbAnchorCalibBY = GetDoubleValue("CCDAnchor", "OffsetB_Y", 0);

	m_bShowWaferInfo = GetBoolValue("MAP_CONFIG", "ShowWaferInfo", false);

	m_bShowTextFrame = GetBoolValue("DRAW_CONFIG", "ShowTextFrame", false);

	m_dbAreaOffsetX = GetDoubleValue("AreaMarking", "offsetX", 0);
	m_dbAreaOffsetY = GetDoubleValue("AreaMarking", "offsetY", 0);
}
void CConfigure::SaveConfig(void)
{
	SetDataValue("SYS_CONFIG", "SoftwareMode", m_nSoftwareMode);
    SetDataValue("SYS_CONFIG", "AutoRun", m_IsAutoRun);
    SetDataValue("SYS_CONFIG", "AutoLogin", m_IsAutoLogin);
    SetDataValue("SYS_CONFIG", "StorePsw", m_IsStoreMM);
    SetDataValue("SYS_CONFIG", "ShowPath", m_IsShowPath);
    SetDataValue("SYS_CONFIG", "ShowGrid", m_IsShowGrid);
    SetDataValue("SYS_CONFIG", "ShowAcross", m_IsShowAcross);
    SetDataValue("SYS_CONFIG", "ShowAxis", m_IsShowAxis);
    SetDataValue("SYS_CONFIG", "LogType", m_iLogType);
    SetDataValue("SYS_CONFIG", "LogLevel", m_iLogLevel);

    SetDataValue("SYS_CONFIG", "AxisMode", m_iAxisMode);
    SetDataValue("SYS_CONFIG", "AxisLimitX", m_dbAxisLimitX);
    SetDataValue("SYS_CONFIG", "AxisLimitY", m_dbAxisLimitY);
    SetDataValue("SYS_CONFIG", "AxisShiftX", m_dbAxisShiftX);
    SetDataValue("SYS_CONFIG", "AxisShiftY", m_dbAxisShiftY);
    SetDataValue("SYS_CONFIG", "AxisShiftA", m_dbAxisShiftA);

    SetDataValue("SYS_CONFIG", "MoveStep", m_dbMoveStep);


    SetDataValue("DEV_CONFIG", "SingleDoc", m_IsSingleDoc);
    m_IsScannerEnable = false;  //--扫描枪扫描标记使能
    m_IsNetworkEnable = false;    //--网络标记使能
    SetDataValue("DEV_CONFIG", "PedalEnable", m_IsPedalEnable);
    SetDataValue("DEV_CONFIG", "OnlineEnable", m_IsOnlineEnable); 

    SetDataValue("HDW_CONFIG", "AutoDriver", m_IsAutoDriver);
    SetDataValue("HDW_CONFIG", "DriverType", m_iDriverType);
    SetDataValue("HDW_CONFIG", "ScanerType", m_iScanerType);
    SetDataValue("HDW_CONFIG", "MainCtrlVer", m_iMainCtrlVer);
    SetDataValue("HDW_CONFIG", "MotionEnable", m_IsMotionEnable);
    SetDataValue("HDW_CONFIG", "MotionType", m_iMotionType);
    SetDataValue("HDW_CONFIG", "VisionEnable", m_IsVisionEnable);
    SetDataValue("HDW_CONFIG", "VisionType", m_iVisionType);

	SetDataValue("CCDAnchor", "Nine_Radius", m_dbNinePRadius);
	SetDataValue("CCDAnchor", "Nine_Pitch", m_dbNinePPitch);
	SetDataValue("CCDAnchor", "Nine_FillPitch", m_dbNinePFillPitch);
	SetDataValue("CCDAnchor", "Nine_CenterX", m_dbNinePCenterX);
	SetDataValue("CCDAnchor", "Nine_CenterY", m_dbNinePCenterY);
	SetDataValue("CCDAnchor", "Nine_CenterX2", m_dbNinePCenterX2);
	SetDataValue("CCDAnchor", "Nine_CenterY2", m_dbNinePCenterY2);
	SetDataValue("CCDAnchor", "Radius", m_dbAnchorRadius);
	SetDataValue("CCDAnchor", "FillPitch", m_dbAnchorFillPitch);
	SetDataValue("CCDAnchor", "OffsetA_X", m_dbAnchorCalibAX);
	SetDataValue("CCDAnchor", "OffsetA_Y", m_dbAnchorCalibAY);
	SetDataValue("CCDAnchor", "OffsetB_X", m_dbAnchorCalibBX);
	SetDataValue("CCDAnchor", "OffsetB_Y", m_dbAnchorCalibBY);

	SetDataValue("MAP_CONFIG", "ShowWaferInfo", m_bShowWaferInfo);

	SetDataValue("DRAW_CONFIG", "ShowTextFrame", m_bShowTextFrame);

	SetDataValue("AreaMarking", "offsetX", m_dbAreaOffsetX);
	SetDataValue("AreaMarking", "offsetY", m_dbAreaOffsetY);
}
bool CConfigure::SetDataValue(const char *sSegName, const char *sKeyName, bool value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%d",value?1:0);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CConfigure::SetDataValue(const char *sSegName, const char *sKeyName, int8_t value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%d",(int)value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CConfigure::SetDataValue(const char *sSegName, const char *sKeyName, int32_t value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%i",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CConfigure::SetDataValue(const char *sSegName, const char *sKeyName, uint32_t value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%u",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CConfigure::SetDataValue(const char *sSegName, const char *sKeyName, float value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CConfigure::SetDataValue(const char *sSegName, const char *sKeyName, double value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CConfigure::SetDataValue(const char *sSegName, const char *sKeyName, const char *value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%s",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
/***********************************************************************************************/
bool CConfigure::GetBoolValue(const char *sSegName, const char *sKeyName, bool defValue)
{
    return (::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName)!=0)?true:false;
}
int8_t CConfigure::GetInt8Value(const char *sSegName, const char *sKeyName, int8_t defValue)
{
    return ::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName);
}
int32_t CConfigure::GetInt32Value(const char *sSegName, const char *sKeyName, int32_t defValue)
{
    return ::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName);
}
uint32_t CConfigure::GetUnsignedValue(const char *sSegName, const char *sKeyName, uint32_t defValue)
{
    return ::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName);
}
float CConfigure::GetFloatValue(const char *sSegName, const char *sKeyName, float defValue)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",defValue);
    ::GetPrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sDataValue,MAX_FILE_NAME,m_sFileName);
    return strtof(m_sDataValue,NULL);
}
double CConfigure::GetDoubleValue(const char *sSegName, const char *sKeyName, double defValue)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",defValue);
    ::GetPrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sDataValue,MAX_FILE_NAME,m_sFileName);
    return atof(m_sDataValue);
}
char * CConfigure::GetStringValue(const char *sSegName, const char *sKeyName, const char *defValue)
{
    ::GetPrivateProfileStringA(sSegName, sKeyName, defValue, m_sDataValue,MAX_FILE_NAME,m_sFileName);
    return m_sDataValue;
}

