/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-17
  * @简述   
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */

#include "stdafx.h"
#include "modbusExt.h"
#include "mbMappingTable.h"

//---------------------------------------------------------------------------
/**
  * @功能  CModbusExt
  * @参数  None
  * @返回  None
  */
CModbusExt::CModbusExt()
{
    m_pTheLog = new CCLog;
    modbusInit();
}
/**
  * @功能  CModbusExt
  * @参数  None
  * @返回  None
  */
CModbusExt::~CModbusExt()
{
    //--
    delete m_pTheLog;
	delete m_pCMBControlProxy;
}
/**
  * @功能  CModbusExt
  * @参数  None
  * @返回  None
  */
void CModbusExt::modbusInit()
{
    mbap_head_length = 7;
    m_iTrans_identifier = 0;

    build_mbap_head(m_stModbus_mbap, 0, 0, 1, DEFAULT_UNIT_IDENTIFIER);

	m_pCMBControlProxy = new CMBControlProxy();
}
/**
  * @功能  CModbusExt
  * @参数  None
  * @返回  None
  */
void CModbusExt::packageMBAP(MBAP_HEAD mbap, uint8_t *dest)
{
    if(dest != NULL)
    {
        int n(0);
        dest[n++] = mbap.trans_id >> 8;
        dest[n++] = mbap.trans_id & 0xff;
        dest[n++] = mbap.protocol_id >> 8;
        dest[n++] = mbap.protocol_id & 0xff;
        dest[n++] = mbap.length >> 8;
        dest[n++] = mbap.length & 0xff;
        dest[n++] = mbap.unit_id;
    }   
}
/**
  * @功能  CModbusExt
  * @参数  None
  * @返回  None
  */
void CModbusExt::unpackageMBAP(MBAP_HEAD &mbap, uint8_t *dest)
{
    if(dest != NULL)
    {
        mbap.trans_id    = (dest[0] << 8) + dest[1];
        mbap.protocol_id = (dest[2] << 8) + dest[3];
        mbap.length      = (dest[4] << 8) + dest[5];
        mbap.unit_id     = dest[6];
    }   
}
/**
  * @功能  CModbusExt
  * @参数  None
  * @返回  None
  */
void CModbusExt::dealRequest(uint8_t *req,int ireqSize, uint8_t *rsp,int &rspSize)
{
    if(req != NULL)
    {
        MBAP_HEAD mbap;
        unpackageMBAP(mbap, req);
        int offset = mbap_head_length;
        int function = req[offset];
        
        switch(function)
        {           
        case MB_FC_READ_COILS:
			rspSize = responseReadCoils(req, rsp, mbap);
            break;          
        case MB_FC_READ_DISCRETE_INPUTS:
			rspSize = responseReadDiscreteInputs(req, rsp, mbap);
            break;          
        case MB_FC_READ_HOLDING_REGISTERS:
			rspSize = responseReadHoldingRegisters(req, rsp, mbap);
            break;          
        case MB_FC_READ_INPUT_REGISTERS:
			rspSize = responseReadInputRegisters(req, rsp, mbap);
            break;          
        case MB_FC_WRITE_SINGLE_COIL:
			rspSize = responseWriteSingleCoil(req, rsp, mbap);
            break;          
        case MB_FC_WRITE_SINGLE_REGISTER:
			rspSize = responseWriteSingleRegister(req, rsp, mbap);
            break;          
        case MB_FC_WRITE_MULTIPLE_COILS:
			rspSize = responseWriteMultipleCoils(req, rsp, mbap);
            break;          
        case MB_FC_WRITE_MULTIPLE_REGISTERS:
			rspSize = responseWriteMultipleRegisters(req, rsp, mbap);
            break;
        default:
			rspSize = responseException(mbap, function, MB_EXCEPTION_ILLEGAL_FUNCTION, rsp);
            break;
        }
    }
}
/**
  * @功能  响应读线圈命令
  * @参数  req:请求命令地址; rsp:响应命令地址;mbap:帧头
  * @返回  None
  */
int CModbusExt::responseReadCoils(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nCoilsAddr = (req[offset + 1] << 8) + req[offset + 2];  //--Coils Start Address
    int nCoilCount = (req[offset + 3] << 8) + req[offset + 4];  //--the Number of Coils  to read

    if (nCoilCount < 1 || MB_MAX_READ_BITS < nCoilCount) 
    {
        iMsgSize = responseException(mbap,function,MB_EXCEPTION_ILLEGAL_DATA_VALUE,rsp);
    }
    else 
    {
        memset(rsp, 0x00, MB_MAX_ADU_LENGTH);
        rsp[offset] = function;
        rsp[offset + 1] = (nCoilCount / 8) + ((nCoilCount % 8) ? 1 : 0);//--Coils Status Bytes
        offset = m_pCMBControlProxy->OnGetCoilsInfo(nCoilsAddr, nCoilCount, rsp+9);
        m_pTheLog->WriteLog("TETE MODBUS:>responseReadCoils: offset=%d\n", offset);
        if (-1 == offset)
        {
            iMsgSize = responseException(mbap, function, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else if (-2 == offset || 0 == offset)
        {
            iMsgSize = responseException(mbap, function, MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
		else
		{
			mbap.length = offset - mbap_head_length + 9;
			packageMBAP(mbap, rsp);
            iMsgSize = offset+8;
		}
    }
    return iMsgSize;
}
/**
  * @功能  响应读离散输入量命令
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseReadDiscreteInputs(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nAddress = (req[offset + 1] << 8) + req[offset + 2];
    int nCount = (req[offset + 3] << 8) + req[offset + 4];

    if (nCount < 1 || MB_MAX_READ_BITS < nCount) 
    {
        iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
    }
    else 
    {
        memset(rsp, 0x00, MB_MAX_ADU_LENGTH);
        rsp[offset] = function;
        rsp[offset + 1] = (nCount / 8) + ((nCount % 8) ? 1 : 0);
		offset = m_pCMBControlProxy->OnGetDiscreteInputsInfo(nAddress, nCount,rsp+9);
        if(-1==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else if(-2==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
		else
		{
			mbap.length = offset - mbap_head_length + 9;
			packageMBAP(mbap, rsp);
            iMsgSize = offset+8;
		}
    }
    return iMsgSize;
}
/**
  * @功能  响应读保持寄存器命令
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseReadHoldingRegisters(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nAddress = (req[offset + 1] << 8) + req[offset + 2];
    int nCount = (req[offset + 3] << 8) + req[offset + 4];

    if (nCount < 1 || MB_MAX_READ_REGISTERS < nCount) 
    {
        iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
    }
    else 
    {
        memset(rsp, 0x00, MB_MAX_ADU_LENGTH);
        rsp[offset++] = function;
        rsp[offset++] = nCount << 1;
		offset = m_pCMBControlProxy->OnGetHoldRegsInfo(nAddress, nCount, rsp+9);
        if(-1==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else if(-2==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
		else
		{
			mbap.length = offset - mbap_head_length + 9;
			packageMBAP(mbap, rsp);
            iMsgSize = offset +8;
		}
    }
    return iMsgSize;
}
/**
  * @功能  响应读输入寄存器命令
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseReadInputRegisters(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nAddress = (req[offset + 1] << 8) + req[offset + 2];
    int nCount = (req[offset + 3] << 8) + req[offset + 4];

    if (nCount < 1 || MB_MAX_READ_REGISTERS < nCount) 
    {
        iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
    }
    else 
    {        
        memset(rsp, 0x00, MB_MAX_ADU_LENGTH);
        rsp[offset++] = function;
        rsp[offset++] = nCount << 1;
		offset = m_pCMBControlProxy->OnGetInputRegsInfo(nAddress, nCount, rsp+9);
        if(-1==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else if(-2==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
		else
		{
			mbap.length = offset - mbap_head_length + 9;
			packageMBAP(mbap, rsp);
            iMsgSize = offset + 8;
		}
    }
    return iMsgSize;
}
/**
  * @功能  响应写单线圈命令
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseWriteSingleCoil(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nAddress = (req[offset + 1] << 8) + req[offset + 2];

    int data = (req[offset + 3] << 8) + req[offset + 4];
    if (data == 0xFF00 || data == 0x0000)
    {
        memcpy(rsp, req, mbap.length + mbap_head_length - 1);
		offset = m_pCMBControlProxy->OnSetCoilInfo(nAddress, data, rsp+9);
        if(-1==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        }
        else if(-2==offset)
        {
            iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
        }
		else
		{
			mbap.length = offset - mbap_head_length + 9;
			packageMBAP(mbap, rsp);
            iMsgSize = offset +8;
		}
    }
    else
    {
        iMsgSize = responseException(mbap, function, MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
    }
    return iMsgSize;
}
/**
  * @功能  响应写单寄存器命令
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseWriteSingleRegister(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nAddress = (req[offset + 1] << 8) + req[offset + 2];

    int data = (req[offset + 3] << 8) + req[offset + 4];
    memcpy(rsp, req, mbap.length + mbap_head_length - 1);
	offset = m_pCMBControlProxy->OnSetHoldRegInfo(nAddress, data, rsp+9);
    if(-1==offset)
        iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
    else if(-2==offset)
        iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
	else
	{
		mbap.length = offset - mbap_head_length + 9;
		packageMBAP(mbap, rsp);
        iMsgSize = offset + 8;
	}
    return iMsgSize;
}
/**
  * @功能  响应写多线圈命令
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseWriteMultipleCoils(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nAddress = (req[offset + 1] << 8) + req[offset + 2];
    int nCount = (req[offset + 3] << 8) + req[offset + 4];

    if (nCount < 1 || MB_MAX_WRITE_BITS < nCount) 
    {
            /* May be the indication has been truncated on reading because of
             * invalid address (eg. nb is 0 but the request contains values to
             * write) so it's necessary to flush. */
            iMsgSize = responseException(mbap, function, MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
    }
    else 
    {
            mbap.length = 6;
            memcpy(rsp, req, mbap_head_length + 5);
            packageMBAP(mbap, rsp);
			offset = m_pCMBControlProxy->OnSetCoilsInfo(nAddress, nCount, rsp);
            if(-1==offset)
                iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
            else if(-2==offset)
                iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
			else
			{
				mbap.length = offset - mbap_head_length + 9;
				packageMBAP(mbap, rsp);
                iMsgSize = offset + 8;
			}
    }
    return iMsgSize;
}
/**
  * @功能  响应写多寄存器命令
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseWriteMultipleRegisters(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap)
{
    int iMsgSize = 0;
    int offset = mbap_head_length;
    int function = req[offset];
    int nAddress = (req[offset + 1] << 8) + req[offset + 2];//--起始地址
    int nCount = (req[offset + 3] << 8) + req[offset + 4];  //--寄存器个数

    if (nCount < 1 || nCount> MB_MAX_WRITE_REGISTERS) 
    {
            iMsgSize = responseException(mbap, function, MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
    }
    else
    {
            mbap.length = 6;
            memcpy(rsp, req, mbap_head_length + 5);
            packageMBAP(mbap, rsp);
			offset = m_pCMBControlProxy->OnSetHoldRegsInfo(nAddress,nCount,rsp);
            if(-1==offset)
                iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
            else if(-2==offset)
                iMsgSize = responseException(mbap, function,MB_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
			else
			{
				mbap.length = offset - mbap_head_length + 9;
				packageMBAP(mbap, rsp);
                iMsgSize = offset + 8;
			}
    }
    return iMsgSize;
}
/**
  * @功能  响应异常
  * @参数  None
  * @返回  None
  */
int CModbusExt::responseException(MBAP_HEAD mbap, int function, int exception_code, uint8_t *rsp)
{
    int rsp_length = mbap_head_length;

    /* Build exception response */
    mbap.length = 3;
    packageMBAP(mbap, rsp);
    rsp[rsp_length] = function + 0x80;
    rsp[rsp_length + 1] = exception_code;

    return 9;
}
/**
  * @功能  Builds a TCP request header 
  * @参数  None
  * @返回  None
  */
int CModbusExt::build_request_basis(MBAP_HEAD &mbap, int function, int addr, int nb, uint8_t *req)
{
    int offset = mbap_head_length;
    req[offset] = function;
    req[offset + 1] = addr >> 8;
    req[offset + 2] = addr & 0x00ff;
    req[offset + 3] = nb >> 8;
    req[offset + 4] = nb & 0x00ff;
    mbap.length = 6;
    mbap.trans_id = m_iTrans_identifier++;
    packageMBAP(mbap, req);

    return mbap.length;
}
/**
  * @功能  None
  * @参数  None
  * @返回  None
  */
void CModbusExt::build_mbap_head(MBAP_HEAD &mbap, uint16_t trans_identifier, uint16_t protocol_identifier, uint16_t length, uint8_t unit_identifier)
{
    mbap.trans_id = trans_identifier;
    mbap.protocol_id = protocol_identifier;
    mbap.length = length;
    mbap.unit_id = unit_identifier;
}
/**
  * @功能  Computes the length of the expected response
  * @参数  None
  * @返回  None
  */
unsigned int CModbusExt::compute_response_length_from_request(uint8_t *req)
{
    int length;
    const int offset = mbap_head_length;

    switch (req[offset]) 
    {
    case MB_FC_READ_COILS:
    case MB_FC_READ_DISCRETE_INPUTS: 
        {
            /* Header + nb values (code from write_bits) */
            int nb = (req[offset + 3] << 8) | req[offset + 4];
            length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
        }
        break;
    case MB_FC_WRITE_AND_READ_REGISTERS:
    case MB_FC_READ_HOLDING_REGISTERS:
    case MB_FC_READ_INPUT_REGISTERS:
        /* Header + 2 * nb values */
        length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
        break;
    case MB_FC_READ_EXCEPTION_STATUS:
        length = 2;
        break;
    case MB_FC_MASK_WRITE_REGISTER:
        length = 7;
        break;
    default:
        length = 5;
    }

    return offset + length;
}
/**
  * @功能  None
  * @参数  None
  * @返回  None
  */
int CModbusExt::check_confirmation(uint8_t *req, uint8_t *rsp, int rsp_length)
{
    int rc;
    int rsp_length_computed;
    const int offset = mbap_head_length;
    const int function = rsp[offset];

    MBAP_HEAD mbap ;
    unpackageMBAP(mbap, rsp);
    int rspLen = mbap_head_length - 1 + mbap.length;

    rsp_length_computed = compute_response_length_from_request(req);

    /* Exception code */
    if (function >= 0x80) 
    {
        if (rspLen == (offset + 2) && req[offset] == (rsp[offset] - 0x80))
        {
            /* Valid exception code received */
            int exception_code = rsp[offset + 1];
            if (exception_code < MB_EXCEPTION_MAX) 
            {
                errno = MB_ENOBASE + exception_code;
            } 
            else 
            {
                errno = EMBBADEXC;
            }
            return -1;
        } 
        else 
        {
            errno = EMBBADEXC;
            return -1;
        }
    }

    /* Check length */
    if ((rspLen == rsp_length_computed || rsp_length_computed == MSG_LENGTH_UNDEFINED) && function < 0x80)
    {
        int req_nb_value;
        int rsp_nb_value;

        /* Check function code */
        if (function != req[offset]) 
        {
            errno = EMBBADDATA;
            return -1;
        }

        /* Check the number of values is corresponding to the request */
        switch (function) 
        {
        case MB_FC_READ_COILS:
        case MB_FC_READ_DISCRETE_INPUTS:
            /* Read functions, 8 values in a byte (nb
             * of values in the request and byte count in
             * the response. */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            req_nb_value = (req_nb_value / 8) + ((req_nb_value % 8) ? 1 : 0);
            rsp_nb_value = rsp[offset + 1];
            break;
        case MB_FC_WRITE_AND_READ_REGISTERS:
        case MB_FC_READ_HOLDING_REGISTERS:
        case MB_FC_READ_INPUT_REGISTERS:
            /* Read functions 1 value = 2 bytes */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 1] / 2);
            break;
        case MB_FC_WRITE_MULTIPLE_COILS:
        case MB_FC_WRITE_MULTIPLE_REGISTERS:
            /* N Write functions */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 3] << 8) | rsp[offset + 4];
            break;
        case MB_FC_REPORT_SLAVE_ID:
            /* Report slave ID (bytes received) */
            req_nb_value = rsp_nb_value = rsp[offset + 1];
            break;
        default:
            /* 1 Write functions & others */
            req_nb_value = rsp_nb_value = 1;
        }

        if (req_nb_value == rsp_nb_value) 
        {
            rc = rsp_nb_value;
        } 
        else 
        {
            errno = EMBBADDATA;
            rc = -1;
        }
    } 
    else 
    {
        errno = EMBBADDATA;
        rc = -1;
    }

    return rc;
}