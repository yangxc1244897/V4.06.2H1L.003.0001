#include "StdAfx.h"  
#include "MyFtp.h"

CYWFtpSession::CYWFtpSession(CWnd* pWnd):
m_pWnd(pWnd)
{  
	m_pFtpConn = NULL;  
	m_strServerAddr.Empty();  
	m_wServerPort = FTP_DEFAULT_PORT;  
	m_strUserName.Empty();  
	m_strPassWord.Empty();  
	m_bInitiativeMode = TRUE;  
	m_strLastErrorMsg.Empty();  
}  

CYWFtpSession::~CYWFtpSession(void)  
{  
	CloseConnection();  
}  

// 设置FTP服务器地址、端口  
void CYWFtpSession::SetServerParam(CString strServerAddr, INTERNET_PORT wServerPort)  
{  
	m_strServerAddr = strServerAddr;  
	m_wServerPort = wServerPort;  
}  

// 设置登录名和密码  
void CYWFtpSession::SetUserInfo(CString strUserName, CString strPassWord, BOOL bInitiativeMode)  
{  
	m_strUserName = strUserName;  
	m_strPassWord = strPassWord;  
	m_bInitiativeMode = bInitiativeMode;  
}  

// 连接到指定FTP服务器  
BOOL CYWFtpSession::ConnectToServer()  
{  
	try  
	{  
		CloseConnection();  
		// m_bInitiativeMode = TRUE 为被动模式  
		m_pFtpConn = m_cInetSession.GetFtpConnection(m_strServerAddr,   
			m_strUserName, m_strPassWord, m_wServerPort, m_bInitiativeMode);  
		if (NULL != m_pFtpConn)  
			return TRUE;  
	}  
	catch (CInternetException *e)  
	{  
		e->Delete();  
	}  
	return FALSE;  
}  

// 得到指定目录下的所有文件  
void CYWFtpSession::GetFileList(CString strDirPath, std::vector<FileProperty> &vctFileList)  
{  
	if (NULL == m_pFtpConn && !ConnectToServer())  
		return ;  

	vctFileList.empty();  

	CFtpFileFind cFtpFileFind(m_pFtpConn);  
	// 查找指定目录  
	BOOL bWorking = cFtpFileFind.FindFile(strDirPath);  
	while (bWorking)  
	{  
		bWorking = cFtpFileFind.FindNextFile();  
		// .或者..  
		if (cFtpFileFind.IsDots())  
			continue;  
		FileProperty sOneFile;  
		// 得到文件名  
		sOneFile.strFileName = cFtpFileFind.GetFileName();  
		sOneFile.dwFileType = 0;  
		// 文件夹  
		if (cFtpFileFind.IsDirectory())  
			sOneFile.dwFileType |= FILE_ATTRIBUTE_DIRECTORY;  
		else  
			sOneFile.dwFileType |= FILE_ATTRIBUTE_NORMAL;   // 普通文件  
		// 添加进文件列表  
		vctFileList.push_back(sOneFile);  
	}  
}  

CInternetFile* CYWFtpSession::OpenFile(CString strFilePath, ULONGLONG& uFileSize)
{
	if (NULL == m_pFtpConn && !ConnectToServer())  
		return NULL; 
	CInternetFile* pFtpFile = NULL;
	try  
	{  
		CFtpFileFind cFtpFileFind(m_pFtpConn);
		BOOL bWorking = cFtpFileFind.FindFile(_T("*"));
		CString csTmpFile = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind(_T('\\')) - 1);
		while(bWorking)
		{
			bWorking = cFtpFileFind.FindNextFile();
			if (!(cFtpFileFind.IsDirectory() || cFtpFileFind.IsDots()) && cFtpFileFind.GetFileName() == csTmpFile)
			{
				pFtpFile = m_pFtpConn->OpenFile(strFilePath);
				uFileSize = cFtpFileFind.GetLength();
				break;
			}
		}
	}
	catch (CInternetException *e)  
	{  
		e->Delete();  
		if (pFtpFile != NULL)
		{
			pFtpFile->Close();
			pFtpFile = NULL;
		}
	}
	return pFtpFile;
}


// 上传一个文件到FTP服务器  
BOOL CYWFtpSession::PutFileToServer(CString strLocalFile, CString strPutPath)  
{  
	if (NULL == m_pFtpConn  
		&& !ConnectToServer())  
		return FALSE;  
	/*
	下面几种现象会出现上传失败：
	1、文件路径不存在，
	2、有用户正在上传同样的文件（如果文件已经上传完成，即可覆盖上传），
	CFtpConnection中CreateDirectory，每次只能创建一级目录
	*/
    if(!_CreateMultilDirectory(strPutPath))
	{
		CString csTmp;
		csTmp.Format(_T("在服务器中文件目录[%s]创建失败"), strPutPath);
		MessageBox(m_pWnd->m_hWnd, csTmp, _T("上传"), MB_ICONWARNING);
		return FALSE;
	}
	CFtpFileFind cFtpFileFind(m_pFtpConn);
	if(cFtpFileFind.FindFile(strPutPath))
	{
		CString csTmp;
		csTmp.Format(_T("服务器中已经存在此文件[%s]，请先删除后再上传"), strPutPath);
		MessageBox(m_pWnd->m_hWnd,csTmp, _T("上传"), MB_ICONWARNING);
		return FALSE;
	}
	BOOL bRet = m_pFtpConn->PutFile(strLocalFile, strPutPath);
	if(GetLastError()== ERROR_INTERNET_EXTENDED_ERROR)
	return bRet;
}  

// 从FTP服务器下载一个文件  
BOOL CYWFtpSession::GetFileFromServer(CString strRemoteFile, CString strLocalPath, BOOL bFailIfExists)  
{  
	BOOL bRet = FALSE;
	if (NULL == m_pFtpConn  
		&& !ConnectToServer())  
		return FALSE; 
	/*
	GetFile注意事项：
	*/
    try
    {
		bRet = m_pFtpConn->GetFile(strRemoteFile, strLocalPath, bFailIfExists,
			FILE_ATTRIBUTE_NORMAL, INTERNET_FLAG_RELOAD, 1);
    }
    catch (CInternetException *e)
    {
    	e->Delete(); 
		return FALSE;
    }
	return bRet;
}  

BOOL CYWFtpSession::DeleteFileFromServer(CString strRemoteFile)
{
	BOOL bRet = TRUE;
	if (NULL == m_pFtpConn  
		&& !ConnectToServer())  
		return FALSE; 

	CFtpFileFind cFtpFileFind(m_pFtpConn);
	if(cFtpFileFind.FindFile(strRemoteFile))
	{
        bRet = m_pFtpConn->Remove(strRemoteFile);
	}
	return bRet;
}
// 关闭连接  
void CYWFtpSession::CloseConnection()  
{  
	if (NULL != m_pFtpConn)  
	{  
		m_pFtpConn->Close();  
		delete m_pFtpConn;  
	}  
	m_pFtpConn = NULL;  
}  

// 检测对象是否存在  
BOOL CYWFtpSession::IsConnectionActive()  
{  
	if (NULL == m_pFtpConn)  
		return FALSE;  
	return TRUE;  
}  

// 获取最后一个错误信息  
CString CYWFtpSession::GetLastErrorMessage()  
{  
	m_strLastErrorMsg.Format(_T("错误码：%ld"), ::GetLastError());  
	return m_strLastErrorMsg;  
}  

BOOL CYWFtpSession::_CreateMultilDirectory(const CString& csDir)
{
	if (m_pFtpConn == NULL)
	{
		return FALSE;
	}
	CFtpFileFind cFtpFileFind(m_pFtpConn);
	CString csFileDir;
	int nIndex = csDir.Find(_T('\\'));
	csFileDir = csDir.Left(nIndex+1);
	while(nIndex != -1)
	{
		BOOL bExistDir = FALSE;
		BOOL bWorking = cFtpFileFind.FindFile(csFileDir);
		nIndex = csDir.Find(_T('\\'), nIndex+1);
		if (nIndex == -1) return TRUE;
		csFileDir = csDir.Left(nIndex);
		CString csTmp = csFileDir.Right(csFileDir.GetLength()-csFileDir.ReverseFind(_T('\\'))-1);
		while(bWorking)
		{
			bWorking = cFtpFileFind.FindNextFile();  
			if (cFtpFileFind.IsDirectory() && cFtpFileFind.GetFileName() == csTmp)  
			{
				bExistDir = TRUE;
				break;
			}
		}
		if (!bExistDir)
		{
			BOOL bRet = m_pFtpConn->CreateDirectory(csFileDir);
			if (FALSE == bRet) return FALSE;
		}
	}
	return TRUE;
}