/**
  ******************************************************************************
  * @文件 fMarkFile.h    
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
#ifndef H_TL_GRAPH_RW_H
#define H_TL_GRAPH_RW_H

#include "tlGraphDefines.h"

//---------------------------------------------------------------------------
class TL_API IFileReader {
public:
    //--IFileReader(){};
    virtual ~IFileReader(){}    

    virtual bool Open(const char *sFileName) = 0;
    virtual bool Close(void) = 0;

    virtual uint32_t ReadData(uint32_t *code) = 0;    
    virtual bool    GetBool( void)  = 0;
    virtual int8_t  GetInt8( void)  = 0;
    virtual int16_t GetInt16( void) = 0;
    virtual int32_t GetInt32( void) = 0;
    virtual int64_t GetInt64( void) = 0;
    virtual float   GetFloat( void) = 0; 
    virtual double  GetDouble(void) = 0; 
    virtual void GetString(char *sName) = 0;
    virtual void GetStringT(wchar_t *sName) {};

protected:
    virtual bool ReadCode(uint32_t *code) = 0;   
    virtual bool ReadBegin(uint32_t node) = 0;
    virtual bool ReadEnd(uint32_t node) = 0;
    virtual bool ReadBool() = 0;
    virtual bool ReadInt8() = 0;
    virtual bool ReadInt16() = 0;
    virtual bool ReadInt32() = 0;
    virtual bool ReadInt64() = 0;
    virtual bool ReadFloat() = 0;
    virtual bool ReadDouble() = 0;
    virtual bool ReadString() = 0;
};

class TL_API IFileWriter {
public:
    //--IFileWriter(){m_pTheFile = NULL;}
    virtual ~IFileWriter() {};

    virtual bool Open(const char *sFileName) = 0;
    virtual bool Close(void) = 0;

    virtual bool WriteBegin(uint32_t node) = 0;
    virtual bool WriteEnd(uint32_t node) = 0;
    virtual bool WriteBool(uint32_t code, bool value) = 0;
    virtual bool WriteInt8(uint32_t code, int8_t value) = 0;
    virtual bool WriteInt16(uint32_t code, int16_t value) = 0;
    virtual bool WriteInt32(uint32_t code, int32_t value) = 0;
    virtual bool WriteInt64(uint32_t code, int64_t value) = 0;
    virtual bool WriteFloat(uint32_t code, float value) = 0;
    virtual bool WriteDouble(uint32_t code, double value) = 0;
    virtual bool WriteString(uint32_t code, const char *text) = 0;
    virtual bool WriteStringT(uint32_t code, const wchar_t *text) { return true; };
};


#endif
