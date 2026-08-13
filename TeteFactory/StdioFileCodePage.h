#pragma once
#include "afx.h"

class CStdioFileCodePage :
	public CStdioFile
{
public:
	CStdioFileCodePage();
	~CStdioFileCodePage();

	typedef enum 
	{
		FILE_CP_NONE = 0,  // 无效编码
		FILE_CP_ANSI,   // 本地编码GB2312
		FILE_CP_UNICODE16_LE,  // UTF16_LE
		FILE_CP_UNICODE16_BE,  // UTF16_LE
		FILE_CP_UTF8,
		FILE_CP_UTF8_BOM,
	}emFileCodePage;
	const static CString g_sFileCodePageName[6];

	/**
	* @brief 判断一个文件的编码格式
	* @see emFileCodePage
	*/
	static emFileCodePage CheckFileCodePage(const CString& sFilePath);

	/**
	* @brief 打开文档
	* @param lpszFileName文件参数
	* @param nOpenFlags 打开标志。忽略CFile::typeBinary、CFile::typeText、CFile::typeUnicode这三个参数
	*/
	BOOL OpenWithoutType(LPCTSTR lpszFileName, UINT nOpenFlags);

	/**
	* @brief 打开文件后，返回一个文件的编码格式
	*/
	emFileCodePage GetFileCodePage() { return m_emFileCP; }
	const CString& GetFileCodePageName() { return g_sFileCodePageName[m_emFileCP]; };

	/**
	* @brief 读文档时，跳过BOM
	*/
	void JumpBOM();

	/**
	* @brief 读取一行文本
	*/
	BOOL ReadStringWithOutType(CString& sContent);

private:

	emFileCodePage m_emFileCP;
};

