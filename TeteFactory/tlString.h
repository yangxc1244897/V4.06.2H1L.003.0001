#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <tchar.h>
#include <algorithm>
#include <map>


#if (defined _UNICODE) || (defined UNICODE)
#define TString std::wstring
#define TOfstream std::wofstream
#define TIfstream std::wifstream
#else
#define TString std::string
#define TOfstream std::ofstream
#define TIfstream std::ifstream
#endif

	class CharUtility
	{
	public:
		/**
		* @功能 Unicode-16字符转成本地Ansi编码字符，也就是宽字符转成多字节字符
		* @参数[in] nAcp ANSI代码页，常见的代码页编码为：936：GBK 简体中文， 950： Big5 繁体中文， 932：Shift-JIS 日语
		*/
		static int sUnicode2Ansi(const wchar_t wc, unsigned int nAcp = CP_ACP)
		{
			wchar_t ws[2] = { wc , 0 };
			std::string s = sW2A(ws, nAcp);

			int n = 0;
			n = ((unsigned char)s[0]) * 256 + ((unsigned char)s[1]);
			return n;
		}

		/**
		* @功能 Unicode-16字符转成本地Ansi编码字符，也就是宽字符转成多字节字符
		* @参数[in] nAcp ANSI代码页，常见的代码页编码为：936：GBK 简体中文， 950： Big5 繁体中文， 932：Shift-JIS 日语
		*/
		static std::string sUnicode2Ansi(const std::wstring &ws, unsigned int nAcp = CP_ACP)
		{
			return sW2A(ws, nAcp);
		}
		/**
		* @功能 本地Ansi字符转成Unicode-16字符，也就是多字节字符转成宽字符
		*/
		static std::wstring sAnsi2Unicode(const std::string &s, unsigned int nAcp = CP_ACP)
		{

			return sA2W(s, nAcp);
		}
		/**
		* @功能 Unicode-16字符转成UTF-8字符
		*/
		static std::string sUnicode2Utf8(const std::wstring &ws)
		{
			return sW2A(ws, CP_UTF8);
		}
		/**
		* @功能 UTF-8字符转成Unicode-16字符
		*/
		static std::wstring sUtf82Unicode(const std::string &s)
		{
			return sA2W(s, CP_UTF8);
		}
		/**
		* @功能 数字转成多字节字符
		*/
		static std::string sInt2StrA(int n)
		{
			char buff[16];
			memset(buff, 0, 16);
			sprintf_s(buff, "%d", n);
			return std::string(buff);
		}
		static std::wstring sInt2StrW(int n)
		{
			wchar_t buff[16];
			memset(buff, 0, 16);
			swprintf_s(buff, L"%d", n);
			return std::wstring(buff);
		}

		/**
		* @功能 将字符串中的字符转成小写
		*/
		static std::string & sToLowerA(std::string & s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			return s;
		}
		static std::wstring & sToLowerW(std::wstring & s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			return s;
		}

		/**
		* @功能 将字符串中的字符转成大写
		*/
		static std::string & sToUpperA(std::string & s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::toupper);
			return s;
		}
		static std::wstring & sToUpperW(std::wstring & s)
		{
			std::transform(s.begin(), s.end(), s.begin(), ::toupper);
			return s;
		}

		/**
		* @功能 将字符串按指定的字符分离
		*/
		static bool sParseStringA(const std::string& s, std::vector<std::string>& vsData, char cTrim = ',')
		{
			vsData.clear();
			if (s.empty()) return false;
			int iSize = s.size();
			std::string sTmp;
			for (int i = 0; i < iSize; i++)
			{
				if (s[i] != cTrim)
				{
					sTmp += s[i];
				}
				else
				{
					vsData.push_back(sTmp);
					sTmp.clear();
				}
			}
			if (!sTmp.empty()) vsData.push_back(sTmp);
			return true;
		}
		static bool sParseStringW(const std::wstring& s, std::vector<std::wstring>& vsData, wchar_t cTrim = L',')
		{
			vsData.clear();
			if (s.empty()) return false;
			int iSize = s.size();
			std::wstring sTmp;
			for (int i = 0; i < iSize; i++)
			{
				if (s[i] != cTrim)
				{
					sTmp += s[i];
				}
				else
				{
					vsData.push_back(sTmp);
					sTmp.clear();
				}
			}
			if (!sTmp.empty()) vsData.push_back(sTmp);
			return true;
		}

		/**
		* @功能 格式化string类型的数据
		* @warn 目标内容不要超过1024字节
		*/
		static int sFormatA(std::string & buf, const char * format, ...)
		{
			char szBuffer[1024];
			memset(szBuffer, 0, sizeof(szBuffer));
			va_list list;
			va_start(list, format);
			int nByteWritten = vsnprintf(szBuffer, sizeof(szBuffer), format, list);
			va_end(list);
			buf = szBuffer;
			return nByteWritten;
		}
		/**
		* @功能 格式化wstring类型的数据
		* @warn 目标内容不要超过1024字节
		*/
		static int sFormatW(std::wstring & buf, const wchar_t * format, ...)
		{
			wchar_t szBuffer[1024];
			memset(szBuffer, 0x0, sizeof(szBuffer));
			va_list list;
			va_start(list, format);
			int nByteWritten = _vsnwprintf(szBuffer, sizeof(szBuffer), format, list);
			va_end(list);
			buf = szBuffer;
			return nByteWritten;
		}

		/**
		* @brief 去掉string右边的空格
		*/
		static std::string& sTrimRightA(std::string& s)
		{
			std::string whitespaces(" ");
			std::size_t found = s.find_last_not_of(whitespaces);
			if (found != std::string::npos)
				s.erase(found + 1);
			else
				s.clear();            // str is all whitespace

			return s;
		}

		/**
		* @brief 去掉wstring右边的空格
		*/
		static std::wstring& sTrimRightW(std::wstring& ws)
		{
			std::wstring whitespaces(_T(" "));
			std::size_t found = ws.find_last_not_of(whitespaces);
			if (found != std::wstring::npos)
				ws.erase(found + 1);
			else
				ws.clear();            // str is all whitespace

			return ws;
		}

		/**
		* @brief 去掉string左边的空格
		*/
		static std::string& sTrimLeftA(std::string& s)
		{
			std::string whitespaces(" ");
			std::size_t found = s.find_first_not_of(whitespaces);
			if (found != std::string::npos)
				s.erase(0, found);
			else
				s.clear();            // str is all whitespace

			return s;
		}

		/**
		* @brief 去掉wstring左边的空格
		*/
		static std::wstring& sTrimLeftW(std::wstring& ws)
		{
			std::wstring whitespaces(_T(" "));
			std::size_t found = ws.find_first_not_of(whitespaces);
			if (found != std::wstring::npos)
				ws.erase(0, found);
			else
				ws.clear();            // str is all whitespace

			return ws;
		}


		/**
		* @brief 替换string中的某个元素
		*/
		static bool sReplaceA(std::string& s, const std::string& oldValue, const std::string& newValue)
		{
			size_t pos = 0;
			size_t a = oldValue.size();
			size_t b = newValue.size();
			while ((pos = s.find(oldValue, pos)) != std::string::npos)
			{
				s.replace(pos, a, newValue);
				pos += b;
			}
			return true;
		}

		/**
		* @brief 替换wstring中的某个元素
		*/
		static bool sReplaceW(std::wstring& ws, const std::wstring& oldValue, const std::wstring& newValue)
		{
			size_t pos = 0;
			size_t a = oldValue.size();
			size_t b = newValue.size();
			while ((pos = ws.find(oldValue, pos)) != std::wstring::npos)
			{
				ws.replace(pos, a, newValue);
				pos += b;
			}
			return true;
		}

	private:
		/**
		* @功能 宽字符转成多字节字符
		*/
		static std::string sW2A(const std::wstring &ws, unsigned int nACP)
		{
			// 预算-缓冲区中多字节的长度    
			int alen = WideCharToMultiByte(nACP, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
			// 给指向缓冲区的指针变量分配内存    
			char * buff = new char[alen + 1];
			memset(buff, 0, alen + 1);
			// 开始向缓冲区转换字节    
			WideCharToMultiByte(nACP, 0, ws.c_str(), -1, buff, alen, nullptr, nullptr);
			std::string rs = buff;
			delete[] buff;
			return rs;
		}
		/**
		* @功能 多字节字符转成宽字符
		*/
		static std::wstring sA2W(const std::string &s, unsigned int nACP)
		{
			// 预算-缓冲区中多字节的长度    
			int alen = MultiByteToWideChar(nACP, 0, s.c_str(), -1, nullptr, 0);
			// 给指向缓冲区的指针变量分配内存    
			wchar_t * buff = new wchar_t[alen + 1];
			wmemset(buff, 0, alen + 1);
			// 开始向缓冲区转换字节    
			MultiByteToWideChar(nACP, 0, s.c_str(), -1, buff, alen);
			std::wstring rs = buff;
			delete[] buff;
			return rs;
		}
	};


 
#define chA2W(ASTR)     CharUtility::sAnsi2Unicode(ASTR)
#define chA2U(ASTR)     CharUtility::sUnicode2Utf8(CharUtility::sAnsi2Unicode(ASTR))
#define chW2A(WSTR)     CharUtility::sUnicode2Ansi(WSTR)
#define chW2U(WSTR)     CharUtility::sUnicode2Utf8(WSTR)
#define chU2A(USTR)     CharUtility::sUnicode2Ansi(CharUtility::sUtf82Unicode(USTR))
#define chU2W(USTR)     CharUtility::sUtf82Unicode(USTR)
#define chFormatA		CharUtility::sFormatA
#define chFormatW		CharUtility::sFormatW
#define chReplaceA      CharUtility::sReplaceA
#define chReplaceW      CharUtility::sReplaceW
#define chTrimLeftA      CharUtility::sTrimLeftA
#define chTrimLeftW      CharUtility::sTrimLeftW
#define chTrimRightA      CharUtility::sTrimRightA
#define chTrimRightW      CharUtility::sTrimRightW
#if (defined _UNICODE) || (defined UNICODE)
#define chT2A           chW2A
#define chT2W
#define chT2U           chW2U
#define chA2T           chA2W
#define chW2T
#define chU2T           chU2W

#define chInt2Str       CharUtility::sInt2StrW
#define chFormat        chFormatW
#define chToLower       CharUtility::sToLowerW
#define chToUpper       CharUtility::sToUpperW
#define chParseString   CharUtility::sParseStringW
#define chTrimLeft		chTrimLeftW
#define chTrimRight		chTrimRightW
#define chReplace		chReplaceW
#else
#define chT2A 
#define chT2W           chA2W
#define chA2T
#define chW2T           chW2A
#define chU2T           chU2A
#define chT2U           chA2U

#define chInt2Str       CharUtility::sInt2StrA
#define chFormat        chFormatA
#define chToLower       CharUtility::sToLowerA
#define chToUpper       CharUtility::sToUpperA
#define chParseString   CharUtility::sParseStringA
#define chTrimLeft		chTrimLeftA
#define chTrimRight		chTrimRightA
#define chReplace		chReplaceA
#endif
