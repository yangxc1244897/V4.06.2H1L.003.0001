#include "stdafx.h"
#include "FinsTcpClient.h"
#include <string>         // for std::string
#include <vector>
#define ISDEBUG  0

CFinsTcpClient* CFinsTcpClient::_Instance = NULL;
CFinsTcpClient::CFinsTcpClient()
{
}


CFinsTcpClient::~CFinsTcpClient()
{
}

CFinsTcpClient * CFinsTcpClient::Instance()
{

	if (NULL == _Instance)
	{
		_Instance = new CFinsTcpClient();
	}
	return _Instance;
}

bool CFinsTcpClient::IsLinking()
{
	return IsLink();
}

/*
** FANS通讯PLC和上位机握手协议
**  前4字节 46 49 4E 53 是 ASCII 字符 "FINS"，表示这是 FINS 协议的标识。
**			00 00 00 0C：表示后续数据的长度（12字节）。
**			00 00 00 00：保留字段（通常为0）。
**			00 00 00 00：保留字段（通常为0）。
**			00 00 00 00：PC节点IP，当设置为0时，会自动获取节点IP。
*/


bool CFinsTcpClient::ConnServer(CString ip, int port)
{
	if (IsLink())
	{
		DisConnect();
	}
	// 建立TCP连接
	if (!Open(CStringA(ip), port)) {
		return false;
	}

	// 注册接收回调
	Register(this, port, MyReciveFunc);

	// 执行FINS握手
	if (!Handshake()) {
		Close();
		m_RecvBuffer.clear();
		return false;
	}

	// 提取目标IP的最后一段作为DA1
	CStringA ipAnsi(ip);
	CString strTargetIP(ipAnsi);
	int lastDotPos = strTargetIP.ReverseFind('.');
	if (lastDotPos == -1 || lastDotPos >= strTargetIP.GetLength() - 1) {
		Close();
		return false;
	}
	CString strLastOctet = strTargetIP.Mid(lastDotPos + 1);
	mDA1 = (char)_ttoi(strLastOctet); // 转为char类型

	return true;
}

bool CFinsTcpClient::DisConnect()
{
	Close();
	return true;
}

bool CFinsTcpClient::Handshake()
{
	char buf[1024] = { 0 };
	char HeaderBuf[4] = { 0x46,0x49,0x4e,0x53 };
	char LenghtBuf[4] = { 0x00,0x00,0x00,0x0C };
	char CommandBuf[4] = { 0x00,0x00,0x00,0x00 };
	char ErrorCodeBuf[4] = { 0x00,0x00,0x00,0x00 };
	char ClientNodeBuf[4] = { 0x00,0x00,0x00,0x00 };

	memcpy(buf, HeaderBuf, sizeof(HeaderBuf));
	memcpy(buf + 4, LenghtBuf, sizeof(LenghtBuf));
	memcpy(buf + 8, CommandBuf, sizeof(CommandBuf));
	memcpy(buf + 12, ErrorCodeBuf, sizeof(ErrorCodeBuf));
	memcpy(buf + 16, ClientNodeBuf, sizeof(ClientNodeBuf));

	memset(mRecvBufTemp, 0, sizeof(mRecvBufTemp));

	Send(buf, 20);


	if (!WaitFinishCmd()) {
		return false;
	}

	// 校验FINS帧头 "FINS"
	if ((unsigned char)mRecvBufTemp[0] != 0x46 ||
		(unsigned char)mRecvBufTemp[1] != 0x49 ||
		(unsigned char)mRecvBufTemp[2] != 0x4E ||
		(unsigned char)mRecvBufTemp[3] != 0x53) {
		return false;
	}

	// 错误码必须为0
	if ((unsigned char)mRecvBufTemp[12] != 0x00 ||
		(unsigned char)mRecvBufTemp[13] != 0x00 ||
		(unsigned char)mRecvBufTemp[14] != 0x00 ||
		(unsigned char)mRecvBufTemp[15] != 0x00) {
		return false;
	}

	unsigned char cmdH = (unsigned char)mRecvBufTemp[8];
	unsigned char cmdL = (unsigned char)mRecvBufTemp[9];

	if (cmdH == 0x00 && cmdL == 0x01)
	{
		// 标准握手回复（命令0x0001，24字节）：
		// FINS(4) + 长度(4) + 命令(4) + 错误码(4) + 客户端节点(4) + PLC节点(4)
		// 偏移19为客户端节点最后一字节，即本机PC节点号 -> 作为SA1
		mSA1 = mRecvBufTemp[19];
		return true;
	}
	else if (cmdH == 0x00 && cmdL == 0x00)
	{
		// 简化握手回复（命令0x0000，20字节，与请求格式一致）：
		// FINS(4) + 长度(4) + 命令(4) + 错误码(4) + 节点(4)
		mSA1 = mRecvBufTemp[19];
		return true;
	}

	return false;
}

bool CFinsTcpClient::ReadBool(std::string addr, unsigned short & value)
{
	if (addr.length() < 2) return false;
	int ipos = addr.find(".");
	if (ipos == std::string::npos)
	{
		return false;
	}

	int offset = stoi(addr.substr(ipos + 1, addr.length() - ipos - 1));

	std::string raddr = addr.substr(0, ipos);

	if (!ReadShort(raddr,value))
	{
		return false;
	}
	if (offset < 0 || offset > 15) return false;
	value = (value >> offset) & 0x01;

	return true;

}

bool CFinsTcpClient::WriteBool(std::string addr, unsigned short value)
{
	if (addr.length() < 2) return false;
	int ipos = addr.find(".");
	if (ipos == std::string::npos)
	{
		return false;
	}

	int offset = stoi(addr.substr(ipos + 1, addr.length() - ipos - 1));

	std::string raddr = addr.substr(0, ipos);
	value = (value && 0x01) << offset;

	if (!WriteShort(raddr, value))
	{
		return false;
	}

	return true;
}

bool CFinsTcpClient::ReadShort(std::string addr, unsigned short& value)
{
	if (addr.length() < 2) return false;
//	int offset = stoi(addr.substr(1, addr.length() - 1));
	size_t prefixLen = (addr[0] == 'D') ?
		((addr.size() > 1 && addr[1] == 'M') ? 2 : 1) : 0;
	if (prefixLen == 0) return false; // 非法前缀

	int offset = stoi(addr.substr(prefixLen)); // 跳过前缀
	char buf[1024] = { 0 };
	char HeaderBuf[4] = { 0x46,0x49,0x4e,0x53 };
	char LenghtBuf[4] = { 0x00,0x00,0x00,0x1A };
	char CommandBuf[4] = { 0x00,0x00,0x00,0x02 };
	char ErrorCodeBuf[4] = { 0x00,0x00,0x00,0x00 };
	char ICF[1] = { 0x80 };
	char RSV[1] = { 0x00 };
	char GCT[1] = { 0x02 };
	char DNA[1] = { 0x00 };
	char DA1[1] = { mDA1 };  //--PLC的IP地址最后一字节 0x00 - 0xff
	char DA2[1] = { 0x00 };
	char SNA[1] = { 0x00 };
	char SA1[1] = { mSA1 };  //--握手时、服务端返回的字节 0x00 - 0xff
	char SA2[1] = { 0x00 };
	char SID[1] = { 0x00 };
	char MRC[1] = { 0x01 };  //--主请求码
	char SRC[1] = { 0x01 };  //--从请求码
	char AREA[1] = { 0xB1 }; if (addr[0] == 'D') AREA[0] = 0x82;
	char DMADDR[3] = { (offset >> 8) & 0xff,offset & 0xff,0x00 };
	char ReadLenght[2] = { 0x00,0x01 };

	memcpy(buf, HeaderBuf, 4);
	memcpy(buf + 4, LenghtBuf, 4);
	memcpy(buf + 8, CommandBuf, 4);
	memcpy(buf + 12, ErrorCodeBuf, 4);
	memcpy(buf + 16, ICF, 1);
	memcpy(buf + 17, RSV, 1);
	memcpy(buf + 18, GCT, 1);
	memcpy(buf + 19, DNA, 1);
	memcpy(buf + 20, DA1, 1);
	memcpy(buf + 21, DA2, 1);
	memcpy(buf + 22, SNA, 1);
	memcpy(buf + 23, SA1, 1);
	memcpy(buf + 24, SA2, 1);
	memcpy(buf + 25, SID, 1);
	memcpy(buf + 26, MRC, 1);
	memcpy(buf + 27, SRC, 1);
	memcpy(buf + 28, AREA, 1);
	memcpy(buf + 29, DMADDR, 3);
	memcpy(buf + 32, ReadLenght, 2);

	memset(mRecvBufTemp, 0, sizeof(mRecvBufTemp));
	Send(buf, 34);
	if(!WaitFinishCmd()) 
	{
		return false;
	}

	if (mRecvBufTemp[0] == 0x46) {
		if (mRecvBufTemp[28] == 0x00 && (mRecvBufTemp[29] == 0x00 || mRecvBufTemp[29] == 0x40)) {
			value = (unsigned char)mRecvBufTemp[30] * 0x100 + (unsigned char)mRecvBufTemp[31];
			return true;
		}
	}
	return false;
}

bool CFinsTcpClient::WriteShort(std::string addr, unsigned short value)
{
	if (addr.length() < 2) return false;
	//int offset = stoi(addr.substr(1, addr.length() - 1));
	// 支持 D 和 DM 开头的地址
		size_t prefixLen = (addr[0] == 'D') ?
		((addr.size() > 1 && addr[1] == 'M') ? 2 : 1) : 0;
	if (prefixLen == 0) return false; // 非法前缀

	int offset = stoi(addr.substr(prefixLen)); // 跳过前缀

	char buf[1024] = { 0 };
	char HeaderBuf[4] = { 0x46,0x49,0x4e,0x53 };
	char LenghtBuf[4] = { 0x00,0x00,0x00,0x1C };
	char CommandBuf[4] = { 0x00,0x00,0x00,0x02 };
	char ErrorCodeBuf[4] = { 0x00,0x00,0x00,0x00 };
	char ICF[1] = { 0x80 };
	char RSV[1] = { 0x00 };
	char GCT[1] = { 0x02 };
	char DNA[1] = { 0x00 };
	char DA1[1] = { mDA1 };  //--PLC的IP地址最后一位 0x00 - 0xff
	char DA2[1] = { 0x00 };
	char SNA[1] = { 0x00 };
	char SA1[1] = { mSA1 };
	char SA2[1] = { 0x00 };
	char SID[1] = { 0x00 };
	char MRC[1] = { 0x01 };  //--主请求码
	char SRC[1] = { 0x02 };  //--从请求码
	char AREA[1] = { 0xB1 }; if (addr[0] == 'D') AREA[0] = 0x82;
	char DMADDR[3] = { (offset >> 8) & 0xff,offset & 0xff,0x00 };
	char WriteLenght[2] = { 0x00,0x01 };
	char WriteBuf[2] = { (value >> 8) & 0xff,value & 0xff };

	memcpy(buf, HeaderBuf, 4);
	memcpy(buf + 4, LenghtBuf, 4);
	memcpy(buf + 8, CommandBuf, 4);
	memcpy(buf + 12, ErrorCodeBuf, 4);
	memcpy(buf + 16, ICF, 1);
	memcpy(buf + 17, RSV, 1);
	memcpy(buf + 18, GCT, 1);
	memcpy(buf + 19, DNA, 1);
	memcpy(buf + 20, DA1, 1);
	memcpy(buf + 21, DA2, 1);
	memcpy(buf + 22, SNA, 1);
	memcpy(buf + 23, SA1, 1);
	memcpy(buf + 24, SA2, 1);
	memcpy(buf + 25, SID, 1);
	memcpy(buf + 26, MRC, 1);
	memcpy(buf + 27, SRC, 1);
	memcpy(buf + 28, AREA, 1);
	memcpy(buf + 29, DMADDR, 3);
	memcpy(buf + 32, WriteLenght, 2);
	memcpy(buf + 34, WriteBuf, 2);

	memset(mRecvBufTemp, 0, sizeof(mRecvBufTemp));
	Send(buf, 36);

	if (!WaitFinishCmd()) {
		return false;
	}

	if (mRecvBufTemp[0] == 0x46) {
		if (mRecvBufTemp[28] == 0x00 && (mRecvBufTemp[29] == 0x00 || mRecvBufTemp[29] == 0x40)) {
			return true;
		}
	}
	return false;
}

bool CFinsTcpClient::ReadFloat(std::string addr, float & value)
{
	std::vector<unsigned char> veRtn;
	if (!ReadAddr(addr, veRtn, 2))
	{
		return false;
	}
	if (veRtn.size() < 4)
	{
		return false;
	}

	((unsigned char*)&value)[0] = veRtn[3];
	((unsigned char*)&value)[1] = veRtn[2];
	((unsigned char*)&value)[2] = veRtn[1];
	((unsigned char*)&value)[3] = veRtn[0];
	return true;
}

bool CFinsTcpClient::WriteFloat(std::string addr, float value)
{
	std::vector<unsigned char> veRtn;

	veRtn.push_back(((unsigned char*)&value)[3]); 
	veRtn.push_back(((unsigned char*)&value)[2]);
	veRtn.push_back(((unsigned char*)&value)[1]);
	veRtn.push_back(((unsigned char*)&value)[0]); 
	if (!WriteAddr(addr, veRtn))
	{
		return false;
	}
	return true;
}

bool CFinsTcpClient::ReadAddr(std::string addr, std::vector<unsigned char>& value, short lenght)
{

	if (addr.length() < 2) return false;
	int offset = stoi(addr.substr(1, addr.length() - 1));
	//unsigned short Lenght = value.size();

	char buf[1024] = { 0 };
	char HeaderBuf[4] = { 0x46,0x49,0x4e,0x53 };
	char LenghtBuf[4] = { 0x00,0x00,0x00,0x1A };
	char CommandBuf[4] = { 0x00,0x00,0x00,0x02 };
	char ErrorCodeBuf[4] = { 0x00,0x00,0x00,0x00 };
	char ICF[1] = { 0x80 };
	char RSV[1] = { 0x00 };
	char GCT[1] = { 0x02 };
	char DNA[1] = { 0x00 };
	char DA1[1] = { mDA1 };  //--PLC的IP地址最后一字节 0x00 - 0xff  17
	char DA2[1] = { 0x00 };
	char SNA[1] = { 0x00 };
	char SA1[1] = { mSA1 }; //PC结点地址 18
	char SA2[1] = { 0x00 };
	char SID[1] = { 0x00 };
	char MRC[1] = { 0x01 };  //--主请求码
	char SRC[1] = { 0x01 };  //--从请求码
	char AREA[1] = { 0xB1 }; if (addr[0] == 'D') AREA[0] = 0x82;
	char DMADDR[3] = { (offset >> 8) & 0xff,offset & 0xff,0x00 };
	char ReadLenght[2] = { lenght >> 8,lenght & 0xff };


	memcpy(buf, HeaderBuf, 4);
	memcpy(buf + 4, LenghtBuf, 4);
	memcpy(buf + 8, CommandBuf, 4);
	memcpy(buf + 12, ErrorCodeBuf, 4);
	memcpy(buf + 16, ICF, 1);
	memcpy(buf + 17, RSV, 1);
	memcpy(buf + 18, GCT, 1);
	memcpy(buf + 19, DNA, 1);
	memcpy(buf + 20, DA1, 1);
	memcpy(buf + 21, DA2, 1);
	memcpy(buf + 22, SNA, 1);
	memcpy(buf + 23, SA1, 1);
	memcpy(buf + 24, SA2, 1);
	memcpy(buf + 25, SID, 1);
	memcpy(buf + 26, MRC, 1);
	memcpy(buf + 27, SRC, 1);
	memcpy(buf + 28, AREA, 1);
	memcpy(buf + 29, DMADDR, 3);
	memcpy(buf + 32, ReadLenght, 2);

	memset(mRecvBufTemp, 0, sizeof(mRecvBufTemp));
	Send(buf, 34);

	if (!WaitFinishCmd()) {
		return false;
	}

	if (mRecvBufTemp[0] == 0x46) {
		if (mRecvBufTemp[28] == 0x00 && (mRecvBufTemp[29] == 0x00 || mRecvBufTemp[29] == 0x40)) {
			short rLenght = (unsigned char)mRecvBufTemp[6] * 0x100 + (unsigned char)mRecvBufTemp[7] - 0x16;
			for (short i = 0; i < rLenght; i++) {
				unsigned char ch = (unsigned char)mRecvBufTemp[i + 30];
				value.push_back(ch);
			}
			return true;
		}
	}
	return false;
}

bool CFinsTcpClient::WriteAddr(std::string addr, std::vector<unsigned char>& value)
{
	if (addr.length() < 2) return false;
	int offset = stoi(addr.substr(1, addr.length() - 1));
	short lenght = value.size();
	short wordCount = lenght / 2; // 计算 WORD 数量
	char buf[1024] = { 0 };
	char HeaderBuf[4] = { 0x46,0x49,0x4e,0x53 };
	char LenghtBuf[4] = { 0x00,0x00,0x00,0x1A + lenght };
	char CommandBuf[4] = { 0x00,0x00,0x00,0x02 };
	char ErrorCodeBuf[4] = { 0x00,0x00,0x00,0x00 };
	char ICF[1] = { 0x80 };
	char RSV[1] = { 0x00 };
	char GCT[1] = { 0x02 };
	char DNA[1] = { 0x00 };
	char DA1[1] = { mDA1 };  //--PLC的IP地址最后一位 0x00 - 0xff
	char DA2[1] = { 0x00 };
	char SNA[1] = { 0x00 };
	char SA1[1] = { mSA1 };
	char SA2[1] = { 0x00 };
	char SID[1] = { 0x00 };
	char MRC[1] = { 0x01 };  //--主请求码
	char SRC[1] = { 0x02 };  //--从请求码
	char AREA[1] = { 0xB1 }; if (addr[0] == 'D') AREA[0] = 0x82;
	char DMADDR[3] = { (offset >> 8) & 0xff,offset & 0xff,0x00 };
	//char WriteLenght[2] = { (lenght >> 8) & 0xff,lenght & 0xff };
	// 修正：按 WORD 数量设置写入个数
	char WriteLenght[2] = { (wordCount >> 8) & 0xff, wordCount & 0xff }; // 0002
	char WriteBuf[1024] = { 0 };
	for (int i = 0; i<lenght && i<1024; i++)
	{
		WriteBuf[i] = value[i];
	}

	memcpy(buf, HeaderBuf, 4);
	memcpy(buf + 4, LenghtBuf, 4);
	memcpy(buf + 8, CommandBuf, 4);
	memcpy(buf + 12, ErrorCodeBuf, 4);
	memcpy(buf + 16, ICF, 1);
	memcpy(buf + 17, RSV, 1);
	memcpy(buf + 18, GCT, 1);
	memcpy(buf + 19, DNA, 1);
	memcpy(buf + 20, DA1, 1);
	memcpy(buf + 21, DA2, 1);
	memcpy(buf + 22, SNA, 1);
	memcpy(buf + 23, SA1, 1);
	memcpy(buf + 24, SA2, 1);
	memcpy(buf + 25, SID, 1);
	memcpy(buf + 26, MRC, 1);
	memcpy(buf + 27, SRC, 1);
	memcpy(buf + 28, AREA, 1);
	memcpy(buf + 29, DMADDR, 3);
	memcpy(buf + 32, WriteLenght, 2);
	//memcpy(buf + 34, WriteBuf, 2);
	// 数据拷贝（拷贝全部数据）
	memcpy(buf + 34, value.data(), lenght); // 使用 value.data() 直接访问原始数据

	memset(mRecvBufTemp, 0, sizeof(mRecvBufTemp));
	//Send(buf, 36);
	// 发送（动态计算长度）
	Send(buf, 34 + lenght);

	if (!WaitFinishCmd()) {
		return false;
	}

	if (mRecvBufTemp[0] == 0x46) {
		if (mRecvBufTemp[28] == 0x00 && (mRecvBufTemp[29] == 0x00 || mRecvBufTemp[29] == 0x40)) {
			return true;
		}
	}
	return false;
}

void CFinsTcpClient::MyReciveFunc(void *hWin, int id, const char *msg, int length)
{
	CFinsTcpClient* pDlg = (CFinsTcpClient*)hWin;

#if ISDEBUG
	CString hexStr;
	for (int i = 0; i < length; i++) {
		hexStr.AppendFormat(_T("%02X "), (BYTE)msg[i]);
	}
	AfxMessageBox(_T("Received raw data: ") + hexStr); 
#endif
													 
	pDlg->m_RecvBuffer.insert(pDlg->m_RecvBuffer.end(), msg, msg + length);

	//检查是否收到完整的数据
	while (pDlg->m_RecvBuffer.size() >= 8)
	{
		//FINS 头部标识 "FINS" (0x46 0x49 0x4E 0x53)
		if (pDlg->m_RecvBuffer[0] == 0x46 &&
			pDlg->m_RecvBuffer[1] == 0x49 &&
			pDlg->m_RecvBuffer[2] == 0x4E &&
			pDlg->m_RecvBuffer[3] == 0x53) 
		{
			//从第 5~8 字节（m_RecvBuffer[4]~[7]）解析出 FINS 数据部分的长度
			uint32_t dataLength =
				(pDlg->m_RecvBuffer[4] << 24) |
				(pDlg->m_RecvBuffer[5] << 16) |
				(pDlg->m_RecvBuffer[6] << 8) |
				pDlg->m_RecvBuffer[7];
			//完整的数据长度frameLength
			uint32_t frameLength = 8 + dataLength; 
			if (pDlg->m_RecvBuffer.size() >= frameLength)
			{
			
				memcpy(pDlg->mRecvBufTemp, pDlg->m_RecvBuffer.data(), frameLength);
				pDlg->mRecvBufTemp[frameLength] = '\0';
				pDlg->m_bDataReceived = true;
				pDlg->m_RecvBuffer.erase
					(
					pDlg->m_RecvBuffer.begin(),
					pDlg->m_RecvBuffer.begin() + frameLength
					);
			}
			else {
				break; 
			}
		}
		else {
			// （非 FINS 开头的数据）
			pDlg->m_RecvBuffer.clear();
			break;
		}
	}
}

bool CFinsTcpClient::IsRecvDataCur()
{
	return m_bDataReceived;
}

BOOL CFinsTcpClient::WaitFinishCmd()
{
	int iTimeOut = m_Timeout;
	m_bDataReceived = false;
	while (iTimeOut > 0)
	{
		if (IsRecvDataCur())
		{
			m_bDataReceived = false; 
			return TRUE;
		}
		iTimeOut -= 50;
		Delay(50);
	}
	return FALSE;
}

void CFinsTcpClient::Delay(uint32_t ms)
{
	ULONGLONG dwStart = GetTickCount();
	ULONGLONG dwEnd = dwStart;
	do {
		MSG msg;
		if (0 != PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		dwEnd = GetTickCount() - dwStart;
	} while (dwEnd < ms);
}