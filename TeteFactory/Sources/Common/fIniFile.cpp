/**
  ******************************************************************************
  * @文件   
  * @作者 jchq 
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述 文件读写,文档管理  
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */ 
#include "stdafx.h"
#include <locale>
#include "fIniFile.h"


CIniFile::CIniFile()
{
}
CIniFile::~CIniFile()
{
    SaveFile();
}
bool  CIniFile::OpenFile(const char *strFileName)
{
    m_sIniFileName = strFileName;
    if (!stfFile.Open(m_sIniFileName, CFile::modeRead))
    {
        return false;
    }
    CString strFileLine;
    while (stfFile.ReadString(strFileLine))
    {
        m_aFileContainer.Add(strFileLine);
    }
    stfFile.Close();

    return true;
}
bool  CIniFile::SaveFile(void)
{
    setlocale( LC_CTYPE, ("chs"));
    if (stfFile.Open(m_sIniFileName, CFile::modeCreate | CFile::modeWrite) && m_aFileContainer.GetSize() > 0)
    {
        CString strParam;
        for (int i = 0; i< m_aFileContainer.GetSize(); i++)
        {
            strParam = m_aFileContainer[i];
            stfFile.WriteString(strParam + "\n");
        }
        stfFile.Close();
    }

    if (m_aFileContainer.GetSize() > 0)
    {
        m_aFileContainer.RemoveAll();
    }

    return true;
}
int CIniFile::SearchLine(const CString & strSection, const CString & strVarName)
{  
    if (m_aFileContainer.GetSize() > 0)
    {
        int i = 0;
        int iFileLines = m_aFileContainer.GetSize();
        while (i< iFileLines)
        {  
            CString strValue = m_aFileContainer[i++];
            strValue.TrimLeft();
            if (strValue.Find(L"[") >= 0 && strValue.Find(strSection, 1) >= 0)
            {
                while (i < iFileLines)
                {

                    CString strSectionList = m_aFileContainer[i++];
                    strSectionList.TrimLeft();
                    if (strSectionList.Find(L"//") >= 0)//找到注释行
                        continue;
                    if (strSectionList.Find(strVarName) >= 0)//找到
                    {
                        return i;
                    }
                    if (strSectionList.Find(L"[", 0) >= 0)//另外一个段落出现,寻找失败
                    {
                        return -2;
                    }

                }
            }
        }
    }
    return -1;
}
bool CIniFile::GetVar(const CString & strSection, const CString & strVarName, CString &strReturnValue)
{
    if (m_aFileContainer.GetSize() < 0)
        return false;

    int iLine = SearchLine(strSection, strVarName);
    if (iLine > 0)
    {

        CString		strParam = m_aFileContainer[iLine - 1];
        strReturnValue = strParam.Mid(strParam.Find(L"=") + 1);
        return true;
    }
    return false;
}
bool CIniFile::SetVar(const CString & strSection, const CString & strVarName, const CString & strVar, const int iType)
{
    if (m_aFileContainer.GetSize() == 0)
    {
        m_aFileContainer.Add(L"[" + strSection + L"]");
        m_aFileContainer.Add(strVarName + L"=" + strVar);
        return true;
    }

    int i = 0;
    int iFileLines = m_aFileContainer.GetSize();
    while (i< iFileLines)
    {
        CString		strValue = m_aFileContainer.GetAt(i++);
        strValue.TrimLeft();
        if ((strValue.Find(L"[") >= 0) && (strValue.Find(strSection) >= 0))
        {
            while (i < iFileLines)
            {

                CString strSectionList = m_aFileContainer[i++];
                strSectionList.TrimLeft();
                if (strSectionList.Find(L"//") >= 0)//找到注释行
                    continue;

                if (strSectionList.Find(strVarName) >= 0)//找到
                {

                    CString strParam = strVarName + L"=" + strVar;

                    //FileContainer.SetAt(i-1,strParam);
                    m_aFileContainer[i - 1] = strParam;

                    return true;
                }
                if (strSectionList.Find(L"[", 0) >= 0)//在原来文件的SECTION中,没有相应的变量需要添加而且,这种情况下,下边还有别的section
                {
                    //处理流程是这样的,首先把当前的数值依次向后移动,然后在当前位置加入新出现的数值
                    if (iType != 0)
                        return false;
                    CString strParam;
                    m_aFileContainer.Add(strParam);
                    int iPre = m_aFileContainer.GetSize() - 1;
                    while (iPre >= i)
                    {
                        CString strBehind = m_aFileContainer[iPre - 1];
                        m_aFileContainer[iPre] = strBehind;
                        iPre--;

                    }//*/
                    strParam = strVarName + L"=" + strVar;
                    m_aFileContainer.SetAt(i - 1, strParam);
                    return true;
                }
                if (i == iFileLines && iType == 0)
                {
                    m_aFileContainer.Add(strVarName + L"=" + strVar);
                    return true;
                }
            }
        }
    }
    if (iType == 0)
    {
        m_aFileContainer.Add(L"[" + strSection + L"]");
        m_aFileContainer.Add(strVarName + L"=" + strVar);
    }

    return true;

}
bool CIniFile::ReadBool(LPCWSTR lpSegName, LPCWSTR lpKeyName,bool bDefault)
{
    CString  strReturnVar;
    if (GetVar(lpSegName, lpKeyName, strReturnVar))
    {
        strReturnVar.TrimLeft();
        int iLen = strReturnVar.GetLength();
        int iValue = _wtoi(strReturnVar);

        return iValue != 0 ? true : false;
    }
    else
	    return bDefault;
}
bool CIniFile::WriteBool(LPCWSTR lpSegName, LPCWSTR lpKeyName, bool bValue)
{
    if(bValue)
        SetVar(lpSegName, lpKeyName, L"1", 0);
    else
        SetVar(lpSegName, lpKeyName, L"0", 0);
    return true;
}
int CIniFile::ReadInteger(LPCWSTR lpSegName, LPCWSTR lpKeyName, int iDefault)
{
    CString  strReturnVar;
    if (GetVar(lpSegName, lpKeyName, strReturnVar))
    {
        strReturnVar.TrimLeft();
        int iLen = strReturnVar.GetLength();
        int iValue = _wtoi(strReturnVar);
        return iValue;
    }
    else	
        return iDefault;
}
int CIniFile::WriteInteger(LPCWSTR lpSegName, LPCWSTR lpKeyName, int iValue)
{
    CString strVar;
    strVar.Format(L"%d", iValue);
    SetVar(lpSegName, lpKeyName, strVar, 0);
	return iValue;
}
double CIniFile::ReadFloat(LPCWSTR lpSegName, LPCWSTR lpKeyName, double dbDefault)
{
    CString  strReturnVar;
    if (GetVar(lpSegName, lpKeyName, strReturnVar))
    {
        strReturnVar.TrimLeft();
        double dbValue = _wtof(strReturnVar);
        return dbValue;
    }
    else
	    return dbDefault;
}
double CIniFile::WriteFloat(LPCWSTR lpSegName, LPCWSTR lpKeyName, double dbValue)
{
    CString strVar;
    strVar.Format(L"%f", dbValue);
    SetVar(lpSegName, lpKeyName, strVar, 0);
	return dbValue;
}
CString CIniFile::ReadString(LPCWSTR lpSegName, LPCWSTR lpKeyName, LPCWSTR lpDefault)
{
    CString sValue;
    if(GetVar(lpSegName, lpKeyName, sValue))
        return sValue.GetString();
    else
	    return lpDefault;
}
bool CIniFile::WriteString(LPCWSTR lpSegName, LPCWSTR lpKeyName, LPCWSTR lpValue)
{
	return SetVar(lpSegName, lpKeyName, lpValue, 0);
}