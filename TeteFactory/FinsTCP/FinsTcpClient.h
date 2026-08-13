/*
以下指令均为16进制数，PC IP地址：192.168.250.24，PLC IP地址：192.168.250.23
一、Fins Tcp
1. 握手指令
1.1. 发送
46494E53 0000000C 00000000 00000000 00000018
46494E53：ASCII编码：FINS；
0000000C：指后面跟的字节长度；
00000000：固定命令；
00000000：错误代码；
00000018：PC节点IP，当设置为0时，会自动获取节点IP。
1.2. 反馈
46494E53 00000010 00000001 00000000 00000018 00000017
46494E53：ASCII编码：FINS；
00000010：指后面跟的字节长度；
00000001：固定命令；
00000000：错误代码；
00000018：本机电脑节点IP；
00000017：PLC节点IP。
2. 读取指令
读D100开始的2个地址，注：一次最多读1000个地址。
2.1. 发送
46494E53 0000001A 00000002 00000000 80 00 02 001700 001800 FF 0101 82 006400 0002
46494E53：ASCII编码：FINS；
0000001A：指后面跟的字节长度；
00000002：固定命令；
00000000：错误代码；
80：ICF；
00：RSV；
02：GCT；
00：PLC网络地址；
17：PLC节点地址；
00：PLC单元地址；
00：PC网络地址；
18：PC节点地址；
00：PC单元地址；
FF：SID；
0101：读指令；
82：读地址区(D位:02,D字:82,W位:31,C位:30,W字:B1,C字:B0)；
006400：起始地址；
0002：读个数。
2.2. 反馈
46494E53 0000001A 00000002 00000000 C0 00 02 001800 001700 FF 0101 0000 AABB CCDD
46494E53：ASCII编码：FINS；
0000001A：指后面跟的字节长度；
00000002：固定命令；
00000000：错误代码；
C0：ICF；
00：RSV；
02：GCT；
00：PC网络地址；
18：PC节点地址；
00：PC单元地址；
00：PLC网络地址；
17：PLC节点地址；
00：PLC单元地址；
FF：SID；
0101：读指令；
0000：读取成功标识；
AABB CCDD：读到的数据。
3. 写入指令
往W10，W11写入AABB，CCDD
3.1. 发送
46494E53 0000001E 00000002 00000000 80 00 02 001700 001800 FF 0102 B1 000A00 0002 AABBCCDD
46494E53：ASCII编码：FINS；
0000001E：指后面跟的字节长度；
00000002：固定命令；
00000000：错误代码；
80：ICF；
00：RSV；
02：GCT；
00：PLC网络地址；
17：PLC节点地址；
00：PLC单元地址；
00：PC网络地址；
18：PC节点地址；
00：PC单元地址；
FF：SID；
0102：写指令；
B1：读地址区(D位:02,D字:82,W位:31,C位:30,W字:B1,C字:B0)；
000A00：起始地址；
0002：写个数；
AABBCCDD：写入数据。
3.2. 反馈
46494E53 00000016 00000002 00000000 C0 00 02 001800 001700 FF 0102 0000
46494E53：ASCII编码：FINS；
00000016：指后面跟的字节长度；
00000002：固定命令；
00000000：错误代码；
C0：ICF；
00：RSV；
02：GCT；
00：PC网络地址；
18：PC节点地址；
00：PC单元地址；
00：PLC网络地址；
17：PLC节点地址；
00：PLC单元地址；
FF：SID；
0102：写指令；
0000：写入成功标识。
*/





#pragma once
#include "NBlockSocketFins.h"
#include <vector>
#include <string>         


#define FINSCLIENT CFinsTcpClient::Instance()

class CFinsTcpClient:public NSocketClientFins
{
public:
	CFinsTcpClient();
	~CFinsTcpClient();
	static CFinsTcpClient* Instance();
	static CFinsTcpClient* _Instance;

public:
	/*
	** 连接状态
	*/
	bool IsLinking();
	/*
	** 连接PLC（Fins协议）
	*/
	bool ConnServer(CString ip, int port);
	/*
	** 断开连接
	*/
	bool DisConnect();
	/*
	** Fins协议第三次握手
	** 发送实例：46 49 4E 53 00 00 00 0C 00 00 00 00 00 00 00 00 00 00 00 00
	** 接收实例： 46 49 4E 53 00 00 00 10 00 00 00 01 00 00 00 00 00 00 00 01 00 00 00 02
	*/
	bool Handshake();
	/*
	** 读取单个（2字节）BOOL值(一个地址可以读取0-15个bool值)
	** 参数：addr：PLC地址。value：存放读取的内容
	** 发送实例： 46 49 4E 53 00 00 00 1A 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 01 00 00 01 01 82 00 64 00 00 01 (D100.0/D100.1/D100.2...../D100.15)
	** 接收实例： 46 49 4E 53 00 00 00 18 00 00 00 02 00 00 00 00 C0 00 02 00 01 00 00 01 00 00 01 01 00 00 00 01
	** 实际解析的bool值 0x0001   ->0000 0000 0000 0001  D100.0 =1 (TRUE),D100.1 =0(FALSE),D100.2 = 0(FALSE).........
	*/
	bool ReadBool(std::string addr, unsigned short &value);
	/*
	** 写入单个（2字节）BOOL值(一个地址可以写入0-15个bool值)
	** 参数：addr：PLC地址。value：存放读取的内容
	** 发送实例：46 49 4E 53 00 00 00 1C 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 01 00 00 01 02 82 00 64 00 00 01 00 01 (当前写入D100.0 BOOL：true)
	** 接收实例：46 49 4E 53 00 00 00 16 00 00 00 02 00 00 00 00 C0 00 02 00 01 00 00 01 00 00 01 02 00 00
	** 实际PLC的地址数据   D100.0 =1 (TRUE),D100.1 =0(FALSE),D100.2 = 0(FALSE).........
	*/
	bool WriteBool(std::string addr, unsigned short value);
	
	/*
	** 读取单个 unsigned short（2字节）值（一次性只能读取一个地址数据）
	** 参数：addr：PLC地址。value：存放读取的内容
	** 发送实例：46 49 4E 53 00 00 00 1A 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 18 00 00 01 01 82 00 64 00 00 01 (D100,1)
	** 接收实例：46 49 4E 53 00 00 00 1A 00 00 00 02 00 00 00 00 80 00 02 00 02 00 00 01 00 00 01 01 00 00 AA BB
	*/
	bool ReadShort(std::string addr, unsigned short &value);
	/*
	** 写入单个 unsigned short（2字节）值（一次性只能写入一个地址数据）
	** 参数：addr：PLC地址。value：写入的内容
	** 发送实例： 46 49 4E 53 00 00 00 1C 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 18 00 00 01 02 82 00 64 00 00 01 AA BB (D100,1)
	** 接收实例：46494E53 00000016 00000002 00000000 C0 00 02 001800 000100 00 0102 0000
	*/
	bool WriteShort(std::string addr, unsigned short value);

	/*
	** 读取单个 Float值（一次性只能读取一个地址数据）
	** 参数：addr：PLC地址。value：存放读取的内容
	** 发送实例： 46 49 4E 53 00 00 00 1A 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 01 00 00 01 01 82 00 64 00 00 02 (D100)
	** 接收实例： 46 49 4E 53 00 00 00 1A 00 00 00 02 00 00 00 00 C0 00 02 00 01 00 00 01 00 00 01 01 00 00 40 39 99 9A
	*/
	bool ReadFloat(std::string addr, float &value);
	/*
	** 读取单个 Float值（一次性只能写入一个地址数据）
	** 参数：addr：PLC地址。value：存放读取的内容
	** 发送实例：  46 49 4E 53 00 00 00 1E 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 01 00 00 01 02 82 00 64 00 00 02 40 83 33 33 (D100)
	** 接收实例：  46 49 4E 53 00 00 00 16 00 00 00 02 00 00 00 00 C0 00 02 00 01 00 00 01 00 00 01 02 00 00
	*/
	bool WriteFloat(std::string addr, float value);
	/*
	** 读地址数据（一次性最多读取1000个地址）
	** 参数：addr：PLC起始地址。value：存放读取的内容。lenght：读取PLC地址个数。
	** 发送实例：46 49 4E 53 00 00 00 1A 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 18 00 00 01 01 82 00 64 00 00 02 (D100,2)
	** 接收实例：46 49 4E 53 00 00 00 1A 00 00 00 02 00 00 00 00 C0 00 02 00 18 00 00 01 00 00 01 01 00 00 AA BB CC DD
	*/
	bool ReadAddr(std::string addr, std::vector<unsigned char> &value, short lenght);
	/*
	** 写地址数据（写入的内容长度必须是偶数AA/AABB，支持写入地址W01,W02 AABBCCDD）
	** 参数：addr：PLC地址。value：存放读取的内容。lenght：读取PLC地址个数。
	** 发送实例：46 49 4E 53 00 00 00 1E 00 00 00 02 00 00 00 00 80 00 02 00 01 00 00 18 00 00 01 02 B1 00 0A 00 00 02 AA BB CC DD (W10,W11,AABB ,CCDD)
	** 接收实例：46494E53 00000016 00000002 00000000 C0 00 02 001800 000100 00 0102 0000
	*/
	bool WriteAddr(std::string addr, std::vector<unsigned char> &value);
	/*
	** 接收服务端消息回调函数
	*/
	static void MyReciveFunc(void * hWin, int id, const char * msg, int length);
	bool IsRecvDataCur();
private:
	BOOL WaitFinishCmd();
	void Delay(uint32_t ms);
	int m_Timeout = 3000;
	char mDA1 = 0;
	char mSA1 = 0;
	char mRecvBufTemp[1024];      // 接收缓冲区
	bool m_bDataReceived;
	CString m_sRecvData;  // 保存接收数据
	std::vector<BYTE>m_RecvBuffer;
};

