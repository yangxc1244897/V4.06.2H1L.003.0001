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
  
#ifndef modbusExtH
#define modbusExtH
//---------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <stdint.h>

#include "fMarkLog.h"
#include "MBControlProxy.h"

#define WM_COILS_CHANGE        (WM_USER+1)
#define WM_INPUTREGS_CHANGE    (WM_USER+2)
#define WM_HOLDREGS_CHANGE     (WM_USER+3)

#ifndef MB_FALSE
#define MB_FALSE 0
#endif

#ifndef MB_TRUE
#define MB_TRUE 1
#endif

#ifndef MB_COIL_OFF
#define MB_COIL_OFF 0
#endif

#ifndef MB_COIL_ON
#define MB_COIL_ON 1
#endif

/* Modbus function codes */
#define MB_FC_READ_COILS                0x01    /*-读线圈*/
#define MB_FC_READ_DISCRETE_INPUTS      0x02    /*-读离散输入量*/
#define MB_FC_READ_HOLDING_REGISTERS    0x03    /*-读保持寄存器*/
#define MB_FC_READ_INPUT_REGISTERS      0x04    /*-读输入寄存器*/
#define MB_FC_WRITE_SINGLE_COIL         0x05    /*-写单个线圈*/
#define MB_FC_WRITE_SINGLE_REGISTER     0x06    /*-写单个寄存器*/
#define MB_FC_READ_EXCEPTION_STATUS     0x07    /*-*/
#define MB_FC_WRITE_MULTIPLE_COILS      0x0F    /*-写多个线圈*/
#define MB_FC_WRITE_MULTIPLE_REGISTERS  0x10    /*-写多个寄存器*/
#define MB_FC_REPORT_SLAVE_ID           0x11    /*-*/
#define MB_FC_MASK_WRITE_REGISTER       0x16    /*-屏蔽写寄存器*/
#define MB_FC_WRITE_AND_READ_REGISTERS  0x17    /*-读/写多个寄存器*/

#define MB_BROADCAST_ADDRESS    0

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define MB_MAX_READ_BITS              2000
#define MB_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define MB_MAX_READ_REGISTERS          125
#define MB_MAX_WRITE_REGISTERS         123
#define MB_MAX_WR_WRITE_REGISTERS      121
#define MB_MAX_WR_READ_REGISTERS       125

/* The size of the MODBUS PDU is limited by the size constraint inherited from
 * the first MODBUS implementation on Serial Line network (max. RS485 ADU = 256
 * bytes). Therefore, MODBUS PDU for serial line communication = 256 - Server
 * address (1 byte) - CRC (2 bytes) = 253 bytes.
 */
#define MB_MAX_PDU_LENGTH              253

/* Consequently:
 * - RTU MODBUS ADU = 253 bytes + Server address (1 byte) + CRC (2 bytes) = 256
 *   bytes.
 * - TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes.
 * so the maximum of both backend in 260 bytes. This size can used to allocate
 * an array of bytes to store responses and it will be compatible with the two
 * backends.
 */
#define MB_MAX_ADU_LENGTH              260

/* Max between RTU and TCP max adu length (so TCP) */
#define MAX_MESSAGE_LENGTH 260

/* Random number to avoid errno conflicts */
#define MB_ENOBASE 112345678

/* Protocol exceptions */
enum emMbException{
    MB_EXCEPTION_ILLEGAL_FUNCTION = 0x01,  /*非法功能*/
    MB_EXCEPTION_ILLEGAL_DATA_ADDRESS,     /*非法数据地址*/
    MB_EXCEPTION_ILLEGAL_DATA_VALUE,       /*非法数据值*/
    MB_EXCEPTION_SLAVE_OR_SERVER_FAILURE,  /*从站设备故障*/
    MB_EXCEPTION_ACKNOWLEDGE,              /*确认*/
    MB_EXCEPTION_SLAVE_OR_SERVER_BUSY,     /*从属设备忙*/
    MB_EXCEPTION_NEGATIVE_ACKNOWLEDGE,     /**/
    MB_EXCEPTION_MEMORY_PARITY,            /*存储奇偶性差错*/
    MB_EXCEPTION_NOT_DEFINED,              /**/
    MB_EXCEPTION_GATEWAY_PATH,             /*不可用网关路径*/
    MB_EXCEPTION_GATEWAY_TARGET,           /*网关目标设备响应失败*/
    MB_EXCEPTION_MAX                       /**/
};

#define EMBXILFUN  (MB_ENOBASE + MB_EXCEPTION_ILLEGAL_FUNCTION)
#define EMBXILADD  (MB_ENOBASE + MB_EXCEPTION_ILLEGAL_DATA_ADDRESS)
#define EMBXILVAL  (MB_ENOBASE + MB_EXCEPTION_ILLEGAL_DATA_VALUE)
#define EMBXSFAIL  (MB_ENOBASE + MB_EXCEPTION_SLAVE_OR_SERVER_FAILURE)
#define EMBXACK    (MB_ENOBASE + MB_EXCEPTION_ACKNOWLEDGE)
#define EMBXSBUSY  (MB_ENOBASE + MB_EXCEPTION_SLAVE_OR_SERVER_BUSY)
#define EMBXNACK   (MB_ENOBASE + MB_EXCEPTION_NEGATIVE_ACKNOWLEDGE)
#define EMBXMEMPAR (MB_ENOBASE + MB_EXCEPTION_MEMORY_PARITY)
#define EMBXGPATH  (MB_ENOBASE + MB_EXCEPTION_GATEWAY_PATH)
#define EMBXGTAR   (MB_ENOBASE + MB_EXCEPTION_GATEWAY_TARGET)

/* Native libmodbus error codes */
#define EMBBADCRC  (EMBXGTAR + 1)
#define EMBBADDATA (EMBXGTAR + 2)
#define EMBBADEXC  (EMBXGTAR + 3)
#define EMBUNKEXC  (EMBXGTAR + 4)
#define EMBMDATA   (EMBXGTAR + 5)
#define EMBBADSLAVE (EMBXGTAR + 6)

/* Internal use */
#define MSG_LENGTH_UNDEFINED -1

#define MODBUS_MAX_ADDRESS  0xFFFF
//typedef struct _modbus modbus_t;

/*
 *  ---------- Request     Indication ----------
 *  | Client | ---------------------->| Server |
 *  ---------- Confirmation  Response ----------
 */
typedef enum {
    /* Request message on the server side */
    MSG_INDICATION,
    /* Request message on the client side */
    MSG_CONFIRMATION
} msg_type_t;

#define DEFAULT_UNIT_IDENTIFIER 0xFF

typedef struct _MBAP_HEADER{
    uint16_t    trans_id;
    uint16_t    protocol_id;
    uint16_t    length;
    uint8_t     unit_id;
}MBAP_HEAD;

typedef struct {
    int nb_bits;
    int start_bits;
    int nb_input_bits;
    int start_input_bits;
    int nb_input_registers;
    int start_input_registers;
    int nb_registers;    //--寄存器个数
    int start_registers;
    uint8_t *tab_CoilsBits;
    uint8_t *tab_input_bits;
    uint16_t *tab_InputRegisters;
    uint16_t *tab_HoldRegisters;
} modbus_mapping_t;

typedef enum
{
    MODBUS_ERROR_RECOVERY_NONE          = 0,
    MODBUS_ERROR_RECOVERY_LINK          = (1<<1),
    MODBUS_ERROR_RECOVERY_PROTOCOL      = (1<<2)
} modbus_error_recovery_mode;

class CModbusExt
{
public:
    uint8_t request[MB_MAX_ADU_LENGTH];
    uint8_t response[MB_MAX_ADU_LENGTH];
    CMBControlProxy   *m_pCMBControlProxy;
	//--CCommSocket       * m_pModbusSocket;
public:
    CModbusExt();
    ~CModbusExt();

    // modbus Message Recv/Send
    //--int modbusSendMsg(uint8_t *msg, int len);
    //--int modbusRecvMsg(const uint8_t *msg,const int len);
	// deal with request
	void dealRequest(uint8_t *req, int ireqSize, uint8_t *rsp, int &rspSize);
private:
    // modbus response model
    int responseReadCoils(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);
    int responseReadDiscreteInputs(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);
    int responseReadHoldingRegisters(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);
    int responseReadInputRegisters(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);
    int responseWriteSingleCoil(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);
    int responseWriteSingleRegister(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);
    int responseWriteMultipleCoils(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);
    int responseWriteMultipleRegisters(uint8_t *req, uint8_t *rsp, MBAP_HEAD mbap);

    // modbus request model
    int requestWriteSingleCoil(MBAP_HEAD mbap, int addr, int status);
    int requestWriteSingleRegister(MBAP_HEAD mbap, int addr, int value);
    int requestWriteMultipleCoils(MBAP_HEAD mbap, int addr, int nb, const uint8_t *src);
    int requestWriteMultipleRegisters(MBAP_HEAD mbap, int addr, int nb, const uint16_t *src);
    int requestReadCoils(MBAP_HEAD mbap, int addr, int nb, uint8_t *dest);
    int requestReadDiscreteInputs(MBAP_HEAD mbap, int addr, int nb, uint8_t *dest);    
    int requestReadRegisters(MBAP_HEAD mbap, int addr, int nb, uint16_t *dest);
    int requestReadInputRegisters(MBAP_HEAD mbap, int addr, int nb, uint16_t *dest);
private:
    //--modbus_mapping_t *m_pModbus_table;  // modbus mapping table
    MBAP_HEAD         m_stModbus_mbap;  // modbus apu head
    int mbap_head_length;
    int m_iTrans_identifier;
    
    void modbusInit();
    
    void packageMBAP(MBAP_HEAD mbap, uint8_t *dest);
    void unpackageMBAP(MBAP_HEAD &mbap, uint8_t *dest);    
    
    //从tab_bits取出数据回答相关命令，每个uint8_t表示一个bit
    int response_io_status(uint8_t *tab_io_status, int address, int nb, uint8_t *rsp, int offset);
    //收到相关命令，写入tab_bits，也是每个uint8_t表示一个bit，但传过来的是uint8_t，所以需要对每个bit做处理，然后赋值给tab_bits
    void set_bits_from_bytes(uint8_t *dest, int idx, unsigned int nb_bits, const uint8_t *tab_byte);
    //exception
    int responseException(MBAP_HEAD mbap, int function, int exception_code, uint8_t *rsp);
    //--void response_exception(MBAP_HEAD mbap, int function, int exception_code, uint8_t *rsp, const char* str, ...);

    // modbus request model
    int build_request_basis(MBAP_HEAD &mbap, int function, int addr, int nb, uint8_t *req);
    int write_single(MBAP_HEAD mbap, int function, int addr, int value);

    int read_io_status(MBAP_HEAD mbap, int function, int addr, int nb, uint8_t *dest);
    int read_registers(MBAP_HEAD mbap, int function, int addr, int nb, uint16_t *dest);

    // build MBAP_HEAD
    void build_mbap_head(MBAP_HEAD &mbap, uint16_t trans_identifier, uint16_t protocol_identifier, uint16_t length, uint8_t unit_identifier);

    unsigned int compute_response_length_from_request(uint8_t *req);
    int check_confirmation(uint8_t *req, uint8_t *rsp, int rsp_length);
private:
    CCLog *m_pTheLog;
};


#endif
