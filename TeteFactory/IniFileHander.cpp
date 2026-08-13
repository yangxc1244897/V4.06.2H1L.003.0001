#pragma once
#include "stdafx.h"
#include "IniFileHander.h"
#include <shobjidl.h> 

void IniFileWrite(const CString & section, const CString & variable, const CString & s, const CString& sFilePath)
{
	WritePrivateProfileString(section, variable, s, sFilePath);
}

void IniFileWrite(const CString & section, const CString & variable, int n, const CString& sFilePath)
{
	CString s;
	s.Format(_T("%d"), n);
	WritePrivateProfileString(section, variable, s, sFilePath);
}

void IniFileWrite(const CString & section, const CString & variable, double d, const CString& sFilePath)
{
	CString s;
	s.Format(_T("%f"), d);
	WritePrivateProfileString(section, variable, s, sFilePath);
}

void IniFileWrite(const CString & section, const CString & variable, bool b, const CString& sFilePath)
{
	CString s;
	s.Format(_T("%d"), (int)b);
	WritePrivateProfileString(section, variable, s, sFilePath);
}


CString IniFileRead(const CString & section, const CString & variable, const CString & sD, const CString& sFilePath)
{
	TCHAR buff[1024];
	GetPrivateProfileString(section, variable, sD, buff, 1024, sFilePath);
	return (CString)buff;
}

int IniFileRead(const CString & section, const CString & variable, int nD, const CString& sFilePath)
{
	return GetPrivateProfileInt(section, variable, nD, sFilePath);
}

double IniFileRead(const CString & section, const CString & variable, double dD, const CString& sFilePath)
{
	TCHAR buff[1024];
	CString s;
	s.Format(_T("%f"), dD);
	GetPrivateProfileString(section, variable, s, buff, 1024, sFilePath);
	return _ttof(buff);
}

bool IniFileRead(const CString & section, const CString & variable, bool bD, const CString& sFilePath)
{
	return (bool)GetPrivateProfileInt(section, variable, (int)bD, sFilePath);
}


CString AppendPathBackSlash(const CString& path)
{
	if (path.IsEmpty()) {
		return _T("");
	}

	CString sNewPath = path;
	CString sRight = sNewPath.Right(1);
	if (sRight != _T("\\") && sRight != _T("/")) {
		sNewPath += _T("\\");
	}
	return sNewPath;
}


bool CreateMultiPath(const CString& sFilePath)
{
	if (!PathIsDirectory((LPTSTR)(LPCTSTR)sFilePath))
	{
		CString s = AppendPathBackSlash(sFilePath);
		if (!MakeSureDirectoryPathExists(CStringA(s)))
		{
			return FALSE;
		}
	}
	return TRUE;
}

#if 0
bool OpenFolderSelectDlg(const CString& sTip, CString& sPath)
{
	BROWSEINFO   bi;                           //创建BROWSEINFO结构体;
	TCHAR   Buffer[512] = { 0 };
	TCHAR   FullPath[512] = { 0 };
	bi.hwndOwner = NULL;                 //窗口句柄;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;            //返回选择的目录名的缓冲区;
	bi.lpszTitle = sTip;           //弹出的窗口的文字提示;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;  //只返回目录。其他标志看MSDN;
	bi.lpfn = NULL;               //回调函数，有时很有用;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);   //显示弹出窗口，ITEMIDLIST很重要;
	::SHGetPathFromIDList(pidl, FullPath); //在ITEMIDLIST中得到目录名的整个路径;
	if (_tcslen(FullPath)>0)
	{
		sPath = FullPath;
		return true;
	}
	return false;
}
#else
bool OpenFolderSelectDlg(const CString& sTip, CString& sPath, CWnd* pParent)
{
	bool bret = false;
	IFileDialog* pfd = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pfd));
	if (SUCCEEDED(hr))
	{
		DWORD dwOptions;
		pfd->GetOptions(&dwOptions);
		pfd->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
		pfd->SetTitle(sTip);

		HWND hWnd = IsWindow(pParent->GetSafeHwnd()) ? pParent->GetSafeHwnd() : NULL;
		if (pfd->Show(hWnd) == S_OK)
		{
			IShellItem* psiResult;
			pfd->GetResult(&psiResult);
			PWSTR pszPath = NULL;
			psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
			sPath = pszPath;
			CoTaskMemFree(pszPath);
			psiResult->Release();
			bret = true;
		}
		pfd->Release();
	}
	return bret;
}

#endif


