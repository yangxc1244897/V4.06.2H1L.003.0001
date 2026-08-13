#include "stdafx.h"
#include "tlString.h"
#include "StdioFileCodePage.h"

const CString CStdioFileCodePage::g_sFileCodePageName[6] = { _T("无效编码"), _T("ANSI（GB2312）") , _T("Unicode16_LE") , _T("Unicode16_BE"), _T("UTF8"), _T("UTF8_BOM") };

CStdioFileCodePage::CStdioFileCodePage()
{
}


CStdioFileCodePage::~CStdioFileCodePage()
{
}

CStdioFileCodePage::emFileCodePage CStdioFileCodePage::CheckFileCodePage(const CString& sFilePath)
{
	emFileCodePage emCP = FILE_CP_NONE;
	CStdioFile file;
	if (!file.Open(sFilePath, CFile::modeRead | CFile::typeBinary))
	{
		file.Close();
		return emCP;
	}

	unsigned char szRead[3] = { 0 };
	int iCount = file.Read(szRead, 3);
	if (iCount >= 2)
	{
		if (szRead[0] == 0xFE && szRead[1] == 0xFF)
		{
			emCP = FILE_CP_UNICODE16_BE;
			file.Close();
			return emCP;
		}
		else if (szRead[0] == 0xFF && szRead[1] == 0xFE)
		{
			emCP = FILE_CP_UNICODE16_LE;
			file.Close();
			return emCP;
		}
		else if (szRead[0] == 0xEF && szRead[1] == 0xBB)
		{
			if (iCount >= 3 && (szRead[2] == 0xBF))
			{
				emCP = FILE_CP_UTF8_BOM;
				file.Close();
				return emCP;
			}
		}
	}
	
	// 对于其他情况，需要进一步判断属于UTF8 还是 ANSI
	/**
	* 1字节：0xxxxxxx
    * 2字节：110xxxxx 10xxxxxx
    * 3字节：1110xxxx 10xxxxxx 10xxxxxx
    * 4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	bool IsUTF8 = false;
	ULONGLONG uLen = file.GetLength();  // 获取文本字节个数
	file.SeekToBegin();
	do
	{
		memset(szRead, 0, 3);
		iCount = file.Read(szRead, 1);
		if (szRead[0] < 0x80)  continue; // (10000000): 值小于0x80的为ASCII字符 
		else if (szRead[0] < 0xC0) break;    // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
		else if (szRead[0] < 0xE0) // (11100000): 此范围内为2字节UTF-8字符  
		{
			memset(szRead, 0, 3);
			iCount = file.Read(szRead, 1);
			if (iCount == 1)
			{
				if ((szRead[0] & 0x0C) != 0x80) 
					break;
				else
				{
					IsUTF8 = true;
					break;
				}	
			} 
		}
		else if (szRead[0] < 0xF0) // (11110000): 此范围内为3字节UTF-8字符 
		{
			memset(szRead, 0, 3);
			iCount = file.Read(szRead, 2);
			if (2 == iCount)
			{
				if ((szRead[0] & (0xC0)) != 0x80 || (szRead[1] & (0xC0)) != 0x80)
					break;
				else
				{
					IsUTF8 = true;
					break;
				}
			}
		}
		else
		{
			break;
		}
	} while (iCount == 1);
	
	emCP = IsUTF8 ? FILE_CP_UTF8 : FILE_CP_ANSI;


	file.Close();
	return emCP;
}

BOOL CStdioFileCodePage::OpenWithoutType(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	// 去掉nOpenFlags中的属性
	// typeUnicode =      (int) 0x00400
	// typeText =         (int) 0x04000
	// typeBinary =       (int) 0x08000
	nOpenFlags &= 0xF3BFF;
	nOpenFlags |= CFile::typeBinary;

	m_emFileCP = CheckFileCodePage(lpszFileName);

	return Open(lpszFileName, nOpenFlags);
}

void CStdioFileCodePage::JumpBOM()
{
	// 跳过BOM
	if (FILE_CP_UTF8_BOM == m_emFileCP) Seek(3, 0);
	if (FILE_CP_UNICODE16_BE == m_emFileCP) Seek(2, 0);
	if (FILE_CP_UNICODE16_LE == m_emFileCP) Seek(2, 0);
}

BOOL CStdioFileCodePage::ReadStringWithOutType(CString& sContent)
{
	sContent.Empty();
	switch (m_emFileCP)
	{
	case FILE_CP_ANSI:
	case FILE_CP_UTF8:
	case FILE_CP_UTF8_BOM:
	{
		unsigned char szRead[2] = { 0 };
		CStringA tmp;
		int iCount;
		bool bCR = false;
		do
		{
			memset(szRead, 0, 2);
			iCount = Read(szRead, 1);

			// 最后一个字符
			if (0 == iCount && tmp.IsEmpty()) return false;

			if (szRead[0] == 0x0A || bCR)  break;  // 0x0A LF \n 或者上一个字符是CR
			if (szRead[0] == 0x0D)
			{
				bCR = true;
				continue;  // 0x0D CR \r
			}
			tmp += szRead[0]; // 取低2位
		} while (iCount == 1);
		if (FILE_CP_ANSI == m_emFileCP)
			sContent = CString(tmp);
		else
			sContent = chU2T(tmp.GetString()).c_str();
	}
		break;

	case FILE_CP_UNICODE16_BE:
	{
		unsigned char szRead[2] = { 0 };
		int iCount;
		bool bCR = false;
		do
		{
			memset(szRead, 0, 2);
			iCount = Read(szRead, 2);
			// 最后一个字符
			if (0 == iCount && sContent.IsEmpty()) return false;

			if (szRead[0] == '\0')
			{ // ACSII
				if (szRead[1] == 0x0A || bCR)  break;  // 0x0A LF \n 或者上一个字符是CR
				if (szRead[1] == 0x0D)
				{
					bCR = true;
					continue;  // 0x0D CR \r
				}

			}
			wchar_t wchtmp = (((wchar_t)szRead[0]) << 8) | ((wchar_t)szRead[1]);
			sContent += wchtmp;
		
		} while (iCount == 2);
		break;
	}
	case FILE_CP_UNICODE16_LE:
	{
		if (!ReadString(sContent)) return false;
		
		sContent.Replace(_T("\r"), _T(""));
		break;
	}
	}

	return true;
}