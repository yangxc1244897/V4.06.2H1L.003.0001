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
#include "stdafx.h"
#include "..\stdafx.h"
#include "WaferFile.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
/***********************************************************************/
bool CFileReader::Open(const char *sFileName)
{
    m_pTheFile = new std::ifstream;
    m_pTheFile->open (sFileName, std::ios_base::in | std::ios::binary);
    if (!m_pTheFile->is_open())
        return false;
    if (!m_pTheFile->good())
        return false;
    return true;
}
bool CFileReader::Close(void)
{
    if(NULL!=m_pTheFile)
    {
        m_pTheFile->close();
        delete m_pTheFile;
        m_pTheFile = NULL;
    }
    return true;
}
uint32_t CFileReader::ReadData(uint32_t *codeData)
{
    uint32_t code;

    if (!ReadCode(&code))
        return CN_NULL_BASE;

    // *codeData = CODED_CODE(code)>>8;
	*codeData = CODED_CODE(code);
    switch (CODED_TYPE(code))
    {
    case emNODE_NULL:
        break;
    case emNODE_BOOL:
        ReadBool();
        break;
    case emNODE_INT8:
        ReadInt8();
        break;
    case emNODE_INT16:
        ReadInt16();
        break;
    case emNODE_INT32:
        ReadInt32();
        break;
    case emNODE_INT64:
        ReadInt64();
        break;
    case emNODE_FLOAT:
        ReadFloat();
        break;
    case emNODE_DOUBLE:
        ReadDouble();
        break;
    case emNODE_STRING:
        ReadString();
        break;
    default:break;
    }
	if (!m_pTheFile->good())
		return CN_NULL_BASE;
    return CODED_NODE(code);
}
bool CFileReader::ReadCode(uint32_t *code) 
{
    int32_t *int32p;
    char buffer[4];
    m_pTheFile->read(buffer,4);
    int32p = (int32_t *) buffer;
    //--LOG_INFO("%08X \n",*int32p);
    *code = *int32p;

    return (m_pTheFile->good());
}
bool CFileReader::ReadBegin(uint32_t node) 
{
    int32_t cn;
    char buffer[4];
    m_pTheFile->read(buffer,4);
    cn = *(int32_t *) buffer;

    if (!m_pTheFile->good())
        return false;
    if ((node == (cn&&0xFF000000))&&(0 == (cn&&0x00FFFF00)))//--Node一致且ID为0
        return true;
    else
        return false;
}
bool CFileReader::ReadEnd(uint32_t node) 
{
    int32_t cn;
    char buffer[4];
    m_pTheFile->read(buffer,4);
    cn = *(int32_t *) buffer;

    if (!m_pTheFile->good())
        return false;
    if ((node == (cn&0xFF000000))&&(0x00FFFF00 == (cn&0x00FFFF00)))//--Node一致且ID为0xFFFF
        return true;
    else
        return false;
}
bool CFileReader::ReadBool() 
{
    char buffer[1];
    m_pTheFile->read(buffer,1);
    m_bValue = (buffer[0]==0)?false:true;

    return (m_pTheFile->good());
}
bool CFileReader::ReadInt8() 
{
    char buffer[1];
    m_pTheFile->read(buffer,1);
    m_nValue8 = (int8_t)(buffer[0]);

    return (m_pTheFile->good());
}
bool CFileReader::ReadInt16() 
{
    char buffer[2];
    m_pTheFile->read(buffer,2);
    m_nValue16 = (int16_t)((buffer[1] << 8) | buffer[0]);

    return (m_pTheFile->good());
}
bool CFileReader::ReadInt32() 
{
    int32_t *int32p;
    char buffer[4];
    m_pTheFile->read(buffer,4);
    int32p = (int32_t *) buffer;
    m_nValue32 = *int32p;

    return (m_pTheFile->good());
}
bool CFileReader::ReadInt64() 
{
    int64_t *int64p; //64 bits integer pointer
    char buffer[8];
    m_pTheFile->read(buffer,8);
    int64p = (int64_t *) buffer;
    m_nValue64 = *int64p;

    return (m_pTheFile->good());
}
bool CFileReader::ReadFloat() 
{
    float *result;
    char buffer[8];
    m_pTheFile->read(buffer,8);
    result = (float *) buffer;
    m_fValue = *result;

    return (m_pTheFile->good());
}
bool CFileReader::ReadDouble() 
{
    double *result;
    char buffer[8];
    m_pTheFile->read(buffer,8);
    result = (double *) buffer;
    m_dbValue = *result;

    return (m_pTheFile->good());
}
bool CFileReader::ReadString() 
{
    std::string sValue;
	std::getline(*m_pTheFile, sValue, '\0');
	
	memset(m_sValue, 0, sizeof m_sValue);
	strcpy_s(m_sValue, sizeof m_sValue, sValue.c_str());
	
    return (m_pTheFile->good());
}

/**********************************************************************/
bool CFileWriter::Open(const char *sFileName)
{
    //--打开二进制文件
    m_pTheFile = new std::ofstream;
    m_pTheFile->open (sFileName, std::ios_base::out | std::ios::binary | std::ios::trunc);
    if (!m_pTheFile->is_open())
        return false;
    if (!m_pTheFile->good())
        return false;
    return true;
}
bool CFileWriter::Close(void)
{
    if(NULL!=m_pTheFile)
    {
        m_pTheFile->close();
        delete m_pTheFile;
        m_pTheFile = NULL;
    }
    return true;
}

bool CFileWriter::WriteBegin(uint32_t node) 
{
    char buffer[4];
    uint32_t code = node&0xFF000000;
    //--LOG_INFO("CN=:%02X,%04X     --0\n",code>>24,(code>>8)&0x00FFFF);
    buffer[0] = code & 0x00;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    return (m_pTheFile->good());
}
bool CFileWriter::WriteEnd(uint32_t node) 
{
    char buffer[4];
    uint32_t code = node|0x00FFFF00;
    //--LOG_INFO("CN=:%02X,%04X     --F\n",code>>24,(code>>8)&0x00FFFF);
    buffer[0] = code & 0x00;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    return (m_pTheFile->good());
}
bool CFileWriter::WriteBool(uint32_t code, bool value)
{
    char buffer[4];
    code = CODED_DATA(code,emNODE_BOOL);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    buffer[0] = code & 0xFF;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    buffer[0] = value?1:0 ;
    m_pTheFile->write(buffer, 1);
    return (m_pTheFile->good());
}
bool CFileWriter::WriteInt8(uint32_t code, int8_t value)
{
    char buffer[4];
    code = CODED_DATA(code,emNODE_INT8);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    buffer[0] = code & 0xFF;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    buffer[0] = value ;
    m_pTheFile->write(buffer, 1);
    return (m_pTheFile->good());
}
bool CFileWriter::WriteInt16(uint32_t code, int16_t value)
{
    char buffer[4];
    code = CODED_DATA(code,emNODE_INT16);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    buffer[0] = code & 0xFF;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    buffer[0] = value & 0xFF;
    buffer[1] = value  >> 8;
    m_pTheFile->write(buffer, 2);
    return (m_pTheFile->good());
}
bool CFileWriter::WriteInt32(uint32_t code, int32_t value)
{
    char buffer[4];
    code = CODED_DATA(code,emNODE_INT32);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    buffer[0] = code & 0xFF;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    buffer[0] = value & 0xFF;
    buffer[1] = value  >> 8;
    buffer[2] = value  >> 16;
    buffer[3] = value  >> 24;
    m_pTheFile->write(buffer, 4);
    return (m_pTheFile->good());
}
bool CFileWriter::WriteInt64(uint32_t code, int64_t value)
{
    char buffer[8];
    code = CODED_DATA(code,emNODE_INT64);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    buffer[0] = code & 0xFF;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    buffer[0] = (char)(value & 0xFF);
    buffer[1] = (char)(value  >> 8);
    buffer[2] = (char)(value  >> 16);
    buffer[3] = (char)(value  >> 24);
    buffer[4] = (char)(value  >> 32);
    buffer[5] = (char)(value  >> 40);
    buffer[6] = (char)(value  >> 48);
    buffer[7] = (char)(value  >> 56);
    m_pTheFile->write(buffer, 8);
    return (m_pTheFile->good());
}
bool CFileWriter::WriteFloat(uint32_t code, float value)
{
    char buffer[4];
    code = CODED_DATA(code,emNODE_FLOAT);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    buffer[0] = code & 0xFF;
    buffer[1] = code  >> 8;
    buffer[2] = code  >> 16;
    buffer[3] = code  >> 24;
    m_pTheFile->write(buffer, 4);

    unsigned char *val;
    val = (unsigned char *) &value;
    for (int i=0; i<4; i++) {
        buffer[i] =val[i];
    }
    m_pTheFile->write(buffer, 8);
    return (m_pTheFile->good());
}
bool CFileWriter::WriteDouble(uint32_t code, double value)
{
    char bufcode[4];
    char buffer[8];
    code = CODED_DATA(code,emNODE_DOUBLE);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    bufcode[0] = code & 0xFF;
    bufcode[1] = code  >> 8;
    bufcode[2] = code  >> 16;
    bufcode[3] = code  >> 24;
    m_pTheFile->write(bufcode, 4);

    unsigned char *val;
    val = (unsigned char *) &value;
    for (int i=0; i<8; i++) {
        buffer[i] =val[i];
    }
    m_pTheFile->write(buffer, 8);
    return (m_pTheFile->good());
}
bool CFileWriter::WriteString(uint32_t code, const char *text)
{
    char bufcode[4];
    code = CODED_DATA(code,emNODE_STRING);
    //--LOG_INFO("CN=:%02X, %04X, %02X \n",code>>24,(code>>8)&0x00FFFF,code&0x00FF);
    bufcode[0] = code & 0xFF;
    bufcode[1] = code  >> 8;
    bufcode[2] = code  >> 16;
    bufcode[3] = code  >> 24;
    m_pTheFile->write(bufcode, 4);
    *m_pTheFile << text << '\0';
    return (m_pTheFile->good());
}

bool CFileWriter::WriteStringT(uint32_t code, const TCHAR * text)
{
	return WriteString(code, chT2A(text).c_str());
}

