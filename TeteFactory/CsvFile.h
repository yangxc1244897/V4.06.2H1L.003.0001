// CsvFile.h: interface for the CCsvFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSVFILE_H__CC37DF87_2A62_4D36_A5F7_CEE1B74FF49F__INCLUDED_)
#define AFX_CSVFILE_H__CC37DF87_2A62_4D36_A5F7_CEE1B74FF49F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"


#include <vector>
#include "afxmt.h"

using namespace std;

class CCsvFile  
{
public:
	CCsvFile();
	virtual ~CCsvFile();

	// csv文件的列表头字符串，可不设置
	void SetCsvFileHeader(const vector<CString>& vHeader);
	// 写入数据（从结尾写入），多线程调用该函数时要在外部加锁

	BOOL WriteString(CString strFilePath, const vector<CString>& vString);
	BOOL WriteString(CString strFilePath, const vector<CString>& vString,BOOL bFirst);
	BOOL WriteString(CString strFilePath, CString strText);
	BOOL WriteString(CString strFilePath, CString strText,BOOL bFirst);
	// 读入文件（默认从头读入），多线程调用该函数时要在外部加锁
	BOOL ReadString(CString strFilePath, CString& strRead, int nStartPos = 0);

	// 按行分割字符串
	static BOOL SpliteLine(CString& strSrc, std::vector<CString>& vDest);
	// 按符号分割字符串
	static BOOL SpliteItem(CString& strSrc, vector<CString>& vDest); 
	
private:
	BOOL OpenFile(CString strFilePath, BOOL bNeedCreate = TRUE);
	BOOL MakeDirectory(CString strPath);

	BOOL Write(const CString& strData);
	BOOL Read(CString& strData);

	CFile m_curFile;
	CString m_strCsvFileHeader;
};

#endif // !defined(AFX_CSVFILE_H__CC37DF87_2A62_4D36_A5F7_CEE1B74FF49F__INCLUDED_)
