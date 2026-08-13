#include "stdAfx.h"
#include "..\stdAfx.h"
#include "DataProfile.h"
//--#include <stdlib.h>
//--#include <stdio.h>
//--#include <string.h>
CDataProfile::CDataProfile()
{
}

CDataProfile::CDataProfile(const char *sDataFile)
{
    strcpy_s(m_sFileName,MAX_FILE_NAME,sDataFile);
}

CDataProfile::~CDataProfile(void)
{
}

void CDataProfile::SetDataFile(const char *sFileName)
{
    strcpy_s(m_sFileName,MAX_FILE_NAME,sFileName);
}

bool CDataProfile::SetDataValue(const char *sSegName, const char *sKeyName, bool value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%d",value?1:0);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CDataProfile::SetDataValue(const char *sSegName, const char *sKeyName, int8_t value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%d",(int)value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CDataProfile::SetDataValue(const char *sSegName, const char *sKeyName, int32_t value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%i",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CDataProfile::SetDataValue(const char *sSegName, const char *sKeyName, uint32_t value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%u",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CDataProfile::SetDataValue(const char *sSegName, const char *sKeyName, float value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CDataProfile::SetDataValue(const char *sSegName, const char *sKeyName, double value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
bool CDataProfile::SetDataValue(const char *sSegName, const char *sKeyName, const char *value)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%s",value);
    return (::WritePrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sFileName)==TRUE)?true:false;
}
/***********************************************************************************************/
bool CDataProfile::GetBoolValue(const char *sSegName, const char *sKeyName, bool defValue)
{
    return (::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName)!=0)?true:false;
}
int8_t CDataProfile::GetInt8Value(const char *sSegName, const char *sKeyName, int8_t defValue)
{
    return ::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName);
}
int32_t CDataProfile::GetInt32Value(const char *sSegName, const char *sKeyName, int32_t defValue)
{
    return ::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName);
}
uint32_t CDataProfile::GetUnsignedValue(const char *sSegName, const char *sKeyName, uint32_t defValue)
{
    return ::GetPrivateProfileIntA(sSegName, sKeyName, defValue, m_sFileName);
}
float CDataProfile::GetFloatValue(const char *sSegName, const char *sKeyName, float defValue)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",defValue);
    ::GetPrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sDataValue,MAX_FILE_NAME,m_sFileName);
    return strtof(m_sDataValue,NULL);
}
double CDataProfile::GetDoubleValue(const char *sSegName, const char *sKeyName, double defValue)
{
    sprintf_s(m_sDataValue, MAX_FILE_NAME, "%f",defValue);
    ::GetPrivateProfileStringA(sSegName, sKeyName, m_sDataValue, m_sDataValue,MAX_FILE_NAME,m_sFileName);
    return atof(m_sDataValue);
}
char * CDataProfile::GetStringValue(const char *sSegName, const char *sKeyName, const char *defValue)
{
    ::GetPrivateProfileStringA(sSegName, sKeyName, defValue, m_sDataValue,MAX_FILE_NAME,m_sFileName);
    return m_sDataValue;
}