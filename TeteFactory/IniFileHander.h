#pragma once

void IniFileWrite(const CString & section, const CString & variable, const CString & s, const CString& sFilePath);

void IniFileWrite(const CString & section, const CString & variable, int n, const CString& sFilePath);

void IniFileWrite(const CString & section, const CString & variable, double d, const CString& sFilePath);

void IniFileWrite(const CString & section, const CString & variable, bool b, const CString& sFilePath);

CString IniFileRead(const CString & section, const CString & variable, const CString & sD, const CString& sFilePath);

int IniFileRead(const CString & section, const CString & variable, int nD, const CString& sFilePath);

double IniFileRead(const CString & section, const CString & variable, double dD, const CString& sFilePath);

bool IniFileRead(const CString & section, const CString & variable, bool bD, const CString& sFilePath);

CString AppendPathBackSlash(const CString& path);

/**
* @brief 创建多层目录。目录最后不要包含\\
*/
bool CreateMultiPath(const CString& sFilePath);

bool OpenFolderSelectDlg(const CString& sTip, CString& sPath, CWnd* pParent=nullptr);

