// CsvFile.cpp: implementation of the CCsvFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CsvFile.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCsvFile::CCsvFile()
{
	m_strCsvFileHeader.Empty();
}

CCsvFile::~CCsvFile()
{
	if (m_curFile.m_hFile != CFile::hFileNull)
	{
		m_curFile.Close();
	}
	OutputDebugString(_T("CCsvFile"));
}

void CCsvFile::SetCsvFileHeader(const vector<CString>& vHeader)
{
	m_strCsvFileHeader = _T("");
	for (int i = 0; i < vHeader.size(); i++)
	{
		if (i > 0)
		{
			m_strCsvFileHeader += _T(",");
		}
		m_strCsvFileHeader += _T("\"");
		m_strCsvFileHeader += vHeader[i];
		m_strCsvFileHeader += _T("\"");
	}
	m_strCsvFileHeader += _T("\r\n");
}
BOOL CCsvFile::WriteString(CString strFilePath, const vector<CString>& vString,BOOL bFirst)
{
	// 当前打开的文件不是指定的文件
	if (m_curFile.m_hFile != CFile::hFileNull)
	{
		if (m_curFile.GetFilePath() != strFilePath)
		{
			m_curFile.Close();
			if (!OpenFile(strFilePath))
			{
				return TRUE;
			}
		}
	}
	// 当前没有打开的文件
	else
	{
		if (!OpenFile(strFilePath))
		{
			return TRUE;
		}
	}
	if (bFirst)
	{
		// 写入数据（从结尾写入）
		m_curFile.SeekToBegin();
		Write(m_strCsvFileHeader);
	}
	else
	{
		m_curFile.SeekToEnd();

	}



	

	CString strTemp;
	for (int i = 0; i < vString.size(); i++)
	{
		if (i > 0)
		{
			strTemp += _T(",");
		}
		strTemp += _T("\"");
		strTemp += vString[i];
		strTemp += _T("\"");
	}
	strTemp += _T("\r\n");
	Write(strTemp);

	return TRUE;
}
BOOL CCsvFile::WriteString(CString strFilePath, const vector<CString>& vString)
{
	// 当前打开的文件不是指定的文件
	if (m_curFile.m_hFile != CFile::hFileNull)
	{
		if (m_curFile.GetFilePath() != strFilePath)
		{
			m_curFile.Close();
			if (!OpenFile(strFilePath))
			{
				return TRUE;
			}
		}
	}
	// 当前没有打开的文件
	else
	{
		if (!OpenFile(strFilePath))
		{
			return TRUE;
		}
	}

	// 写入数据（从结尾写入）
	m_curFile.SeekToEnd();
	CString strTemp;
	for (int i = 0; i < vString.size(); i++)
	{
		if (i > 0)
		{
			strTemp += _T(",");
		}
		strTemp += _T("\"");
		strTemp += vString[i];
		strTemp += _T("\"");
	}
	strTemp += _T("\r\n");
	Write(strTemp);

	return TRUE;
}

BOOL CCsvFile::WriteString(CString strFilePath, CString strText)
{
	// 当前打开的文件不是指定的文件
	if (m_curFile.m_hFile != CFile::hFileNull)
	{
		if (m_curFile.GetFilePath() != strFilePath)
		{
			m_curFile.Close();
			if (!OpenFile(strFilePath))
			{
				return TRUE;
			}
		}
	}
	// 当前没有打开的文件
	else
	{
		if (!OpenFile(strFilePath))
		{
			return TRUE;
		}
	}

	// 写入数据（从结尾写入）
	m_curFile.SeekToEnd();
	strText += _T("\r\n");
	Write(strText);

	return TRUE;
}

BOOL CCsvFile::WriteString(CString strFilePath, CString strText, BOOL bFirst)
{
	// 当前打开的文件不是指定的文件
	if (m_curFile.m_hFile != CFile::hFileNull)
	{
		if (m_curFile.GetFilePath() != strFilePath)
		{
			m_curFile.Close();
			if (!OpenFile(strFilePath))
			{
				return TRUE;
			}
		}
	}
	// 当前没有打开的文件
	else
	{
		if (!OpenFile(strFilePath))
		{
			return TRUE;
		}
	}
	// 写入数据（从结尾写入）
	if (bFirst)
	{
		m_curFile.SeekToBegin();
	}
	else
	{
		m_curFile.SeekToEnd();
	}
	strText += _T("\r\n");
	Write(strText);

	return TRUE;

}

BOOL CCsvFile::ReadString(CString strFilePath, CString& strRead, int nStartPos)
{
	// 当前打开的文件不是指定的文件
	if (m_curFile.m_hFile != CFile::hFileNull)
	{
		if (m_curFile.GetFilePath() != strFilePath)
		{
			m_curFile.Close();
			if (!OpenFile(strFilePath, FALSE))
			{
				return FALSE;
			}
		}
	}
	// 当前没有打开的文件
	else
	{
		if (!OpenFile(strFilePath, FALSE))
		{
			return FALSE;
		}
	}

	// 读入文件（从头读入）
	m_curFile.Seek(nStartPos, CFile::begin);
	char *pBuf = new char[m_curFile.GetLength() + 1];
	memset(pBuf, 0, (m_curFile.GetLength() + 1) * sizeof(char));
	m_curFile.Read(pBuf, m_curFile.GetLength());
	//mul(CP_ACP,NULL,)
	TCHAR *ptBuf = new TCHAR[m_curFile.GetLength() + 1];
	//WideCharToMultiByte()
	memset(ptBuf, 0, (m_curFile.GetLength() + 1) * sizeof(char));

	MultiByteToWideChar(CP_ACP,NULL,pBuf,(m_curFile.GetLength() + 1) * sizeof(char),ptBuf,(m_curFile.GetLength() + 1) * sizeof(TCHAR));
	strRead.Format(_T("%s"), ptBuf);
	delete[] pBuf;
	delete[] ptBuf;
	pBuf =NULL;
	ptBuf =NULL;

	return TRUE;
}

// 按行分割字符串
BOOL CCsvFile::SpliteLine(CString& strSrc, std::vector<CString>& vDest)
{
	int nStart = 0;
	int nEnd = 0;
	CString strTemp;

	nEnd = strSrc.Find(_T("\r\n"), nStart);
	while (nEnd >= 0)
	{
		strTemp = strSrc.Mid(nStart, nEnd - nStart);
		vDest.push_back(strTemp);

		nStart = nEnd + 2;
		nEnd = strSrc.Find(_T("\r\n"), nStart);
	}
	if (nStart == 0 || nStart < strSrc.GetLength())
	{
		strTemp = strSrc.Mid(nStart, strSrc.GetLength() - nStart);
		vDest.push_back(strTemp);
	}

	return TRUE;
}

// 按符号分割字符串，兼容双引号和逗号分隔符
BOOL CCsvFile::SpliteItem(CString& strSrc, vector<CString>& vDest)
{
	LPTSTR pBuf = strSrc.GetBuffer(0);
	CString strTemp = _T("");

	BOOL bSymbolFlag = FALSE; // 双引号开始标记位
	for (int i = 0; i < strSrc.GetLength(); i++)
	{
		// 双引号标记
		if (bSymbolFlag)
		{
			if (pBuf[i] == _T('"') && (i+1) < strSrc.GetLength())
			{
				// 双引号分割符结束
				if (pBuf[i+1] == _T(','))
				{
					vDest.push_back(strTemp);
					strTemp = _T("");
					bSymbolFlag = FALSE;
					i++;
				}
				// 双引号作为字符，不是结束标准
				else if (pBuf[i+1] == _T('"'))
				{
					strTemp += pBuf[i];
					i++;
				}
				// 舍弃
				else
				{}
			}
			else if (i != (strSrc.GetLength()-1))
			{
				strTemp += pBuf[i];
			}
		}
		else
		{
			// 双引号分割符开始
			if (pBuf[i] == _T('"') && strTemp == _T(""))
			{
				bSymbolFlag = TRUE;
			}
			// 逗号分隔符
			else if (pBuf[i] == _T(','))
			{
				vDest.push_back(strTemp);
				strTemp = _T("");
			}
			else
			{
				strTemp += pBuf[i];
			}
		}
	}
	if (strTemp != _T(""))
	{
		vDest.push_back(strTemp);
		strTemp = _T("");		
	}

	return TRUE;
}

BOOL CCsvFile::OpenFile(CString strFilePath, BOOL bNeedCreate)
{
	// 防止路径不存在
	CString strFolderPath;
	int nFindPos = strFilePath.ReverseFind(_T('\\'));
	if (nFindPos > 1)
	{
		strFolderPath = strFilePath.Mid(0, nFindPos + 1);
	}
	else
	{
		return FALSE;
	}
	// 路径文件夹不存在
	if (!PathIsDirectory(strFolderPath))
	{
		if (bNeedCreate)
		{
			if (!MakeDirectory(strFolderPath))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}	
	}

	// 打开文件
	if (m_curFile.Open(strFilePath, CFile::shareDenyNone | CFile::modeReadWrite))
	{
		return TRUE;
	}
	// 创建文件
	else
	{
		if (bNeedCreate)
		{
			if (m_curFile.Open(strFilePath, CFile::shareDenyNone | CFile::modeReadWrite | CFile::modeCreate))
			{
				Write(m_strCsvFileHeader);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CCsvFile::MakeDirectory(CString strPath)
{
	strPath += _T("\\");

	// 创建文件路径
	int nStart = 0;
	int nEnd = 0;
	CString strTemp = _T("");
	nEnd = strPath.Find(_T("\\"), nEnd);
	while (nEnd != -1)
	{
		strTemp += strPath.Mid(nStart, nEnd-nStart);
		CreateDirectory(strTemp, NULL);
		strTemp += _T("\\");

		nStart = nEnd + 1;	
		nEnd = strPath.Find(_T("\\"), nEnd + 1);
	}

	return TRUE;
}

BOOL CCsvFile::Write(const CString& strData)
{
#ifdef UNICODE
	int nLength = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	char* pszChar = (char*)alloca(nLength + 1);	//allocation of memory on the stack, automatic release
	if ( pszChar )
	{
		WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), pszChar, nLength, NULL, NULL);
		pszChar[nLength] = 0;
		m_curFile.Write(pszChar, nLength);
	}
#else
	m_curFile.Write(strData, strData.GetLength());
#endif

	return TRUE;
}

BOOL CCsvFile::Read(CString& strData)
{
	return TRUE;
}