#pragma once  

#include <afxinet.h>  
#include <vector>  

#define FTP_DEFAULT_PORT 21  
const CString csFtpUserName = _T("ftptest");
const CString csFtpUserPwd = _T("ftptest");

struct FileProperty  
{  
	CString strFileName;  
	// FILE_ATTRIBUTE_DIRECTORY   
	// GetFileAttributes  
	DWORD dwFileType;  
};  

class CYWFtpSession  
{  
public:  
	CYWFtpSession(CWnd* pWnd = NULL);  
	~CYWFtpSession(void);  

	// 设置FTP服务器地址、端口  
	void SetServerParam(CString strServerAddr, INTERNET_PORT wServerPort);  
	// 设置登录名和密码  
	void SetUserInfo(CString strUserName, CString strPassWord, BOOL bInitiativeMode = FALSE);  
	// 连接到指定FTP服务器  
	BOOL ConnectToServer();  
	// 得到指定目录下的所有文件  
	void GetFileList(CString strDirPath, std::vector<FileProperty> &vctFileList);  
	//利CInternetFile打开指定文件
	CInternetFile* OpenFile(CString strFilePath, ULONGLONG& uFileSize);

	
	// 上传一个文件到FTP服务器  
	BOOL PutFileToServer(CString strLocalFile, CString strPutPath);  
	// 从FTP服务器下载一个文件  
	BOOL GetFileFromServer(CString strRemoteFile, CString strLocalPath, BOOL bFailIfExists = FALSE);  
	//删除一个文件
	BOOL DeleteFileFromServer(CString strRemoteFile);  
	// 关闭连接  
	void CloseConnection();  

	// 检测对象是否存在  
	BOOL IsConnectionActive();  

	// 获取最后一个错误信息  
	CString GetLastErrorMessage();  

private:  
	BOOL _CreateMultilDirectory(const CString& csDir);
	// FTP服务器地址  
	CString m_strServerAddr;  
	// 服务端口  
	INTERNET_PORT m_wServerPort;  
	// 登录用户名  
	CString m_strUserName;  
	// 登录密码  
	CString m_strPassWord;  
	// 被动模式-FALSE  主动模式-TRUE  
	BOOL m_bInitiativeMode;  
	// 记录最后一个错误信息  
	CString m_strLastErrorMsg;  

	// 创建并初始化一个或多个同时的Internet 会话  
	CInternetSession m_cInetSession;  
	// 管理与Internet服务器的FTP连接并允许直接操纵服务器中的目录和文件  
	CFtpConnection *m_pFtpConn;  

	//窗口句柄
	CWnd* m_pWnd;
};  