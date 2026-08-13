/**
******************************************************************************
* @文件     
* @作者 jchq 
* @版本 V1.00.00
* @日期 2018-08-01
* @简述 文件读写,文档管理  
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*
*
* 
******************************************************************************
*/ 

#pragma once

#include <stdint.h>
#include <string>
#include "tlGraphFile.h"
#include "WaferNode.h"
#include "tlString.h"


class CFileReader : public IFileReader {
public:
    virtual bool Open(const char *sFileName);
    virtual bool Close(void);

    virtual bool ReadCode(uint32_t *code); 
    virtual bool ReadBegin(uint32_t node);
    virtual bool ReadEnd(uint32_t node);
    virtual bool ReadBool();
    virtual bool ReadInt8();
    virtual bool ReadInt16();
    virtual bool ReadInt32();
    virtual bool ReadInt64();
    virtual bool ReadFloat();
    virtual bool ReadDouble();
    virtual bool ReadString();    

	uint32_t    ReadData(uint32_t *code);
    bool    GetBool( void)  { return m_bValue; };
    int8_t  GetInt8( void)  { return m_nValue8; };
    int16_t GetInt16( void) { return m_nValue16; };
    int32_t GetInt32( void) { return m_nValue32; };
    int64_t GetInt64( void) { return m_nValue64; };
    float   GetFloat( void) { return m_fValue; }; 
    double  GetDouble(void) { return m_dbValue; }; 
    void GetString( char *sName) { strcpy_s(sName, MAX_TXT_SIZE, m_sValue);}
	void GetStringT(TCHAR * sName) { _tcscpy_s(sName, MAX_TXT_SIZE, chA2T(m_sValue).c_str()); }
protected:
    std::ifstream *m_pTheFile;

    bool     m_bValue;
    int8_t   m_nValue8;
    int16_t  m_nValue16;
    int32_t  m_nValue32;
    int64_t  m_nValue64;
    float    m_fValue;
    double   m_dbValue;
    char     m_sValue[MAX_TXT_SIZE];
};


class CFileWriter : public IFileWriter {
public:
    virtual bool Open(const char *sFileName);
    virtual bool Close(void);

    virtual bool WriteBegin(uint32_t node);
    virtual bool WriteEnd(uint32_t node);
    virtual bool WriteBool(uint32_t code, bool value);
    virtual bool WriteInt8(uint32_t code, int8_t value);
    virtual bool WriteInt16(uint32_t code, int16_t value);
    virtual bool WriteInt32(uint32_t code, int32_t value);
    virtual bool WriteInt64(uint32_t code, int64_t value);
    virtual bool WriteFloat(uint32_t code, float value);
    virtual bool WriteDouble(uint32_t code, double value);
    virtual bool WriteString(uint32_t code, const char *text);
	virtual bool WriteStringT(uint32_t code, const TCHAR * text);
protected:
    std::ofstream  *m_pTheFile;
};

