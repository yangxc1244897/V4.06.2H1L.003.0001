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
#ifndef H_INI_FILE
#define H_INI_FILE

class CIniFile
{
public:
	CIniFile();
	~CIniFile();

    bool OpenFile(const char *strFileName);
    bool SaveFile(void);
	bool ReadBool(LPCWSTR lpSegName, LPCWSTR lpKeyName, bool bDefault);
	bool WriteBool(LPCWSTR lpSegName, LPCWSTR lpKeyName, bool bValue);

	int  ReadInteger(LPCWSTR lpSegName, LPCWSTR lpKeyName, int iDefault);
	int  WriteInteger(LPCWSTR lpSegName, LPCWSTR lpKeyName, int iValue);

	double ReadFloat(LPCWSTR lpSegName, LPCWSTR lpKeyName, double dbDefault);
	double WriteFloat(LPCWSTR lpSegName, LPCWSTR lpKeyName, double dbValue);

    CString ReadString(LPCWSTR lpSegName, LPCWSTR lpKeyName, LPCWSTR lpDefault);
    bool WriteString(LPCWSTR lpSegName, LPCWSTR lpKeyName, LPCWSTR lpValue);
private:
	CString m_sFileName;
private:
    bool GetVar(const CString &, const CString &, CString &);
    bool SetVar(const CString &, const CString &, const CString &, const int iType = 1);
    int  SearchLine(const CString &, const CString &);

private:
    CArray <CString, CString> m_aFileContainer;
    CStdioFile stfFile;
    CString m_sIniFileName;
};

#endif

