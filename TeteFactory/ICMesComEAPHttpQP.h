#pragma once
#include "FormWorkControl.h"
#include <afxinet.h>
#include <map>
#include <vector>
using namespace std;
#define TString std::wstring
// 单条批次/设备记录
struct LotEqpItem
{
	CString strLotId;	// lotid
	CString strEqpId;	// eqpid
};
struct LotEqpResponse
{
	BOOL	bStatus = FALSE;			// Status
	CString	strMessage;					// Message
	std::list<LotEqpItem> listData;		// Data 列表
};
class CICMesComEAPHttpQp 
{
	struct LotEqpItem
	{
		CString strLotId;	// lotid
		CString strEqpId;	// eqpid
	};

	// 接口返回的整体结果
	LotEqpResponse m_LotEqpResponse;

public:
	CICMesComEAPHttpQp();
	virtual ~CICMesComEAPHttpQp();
	virtual bool ConnectMes(CString strIP, int nPort, CFormWorkControl* p);
	
	virtual bool GetFinishLotInfo(map<CString, CString> MapInfo);
	/**
	* @brief 上传设备状态到MES
	* @param stDeviceStatus 设备的状态。@see emDeviceStatus_t
	*/	
public:
	CFormWorkControl* m_pWorkCtrol;
private:
	CInternetSession* m_pSession;
	CString m_strIP;
	int m_nPort;
	CString m_sReadStripID;
	std::vector<CString> m_vs2DBarcode;   // 2D Barcode数据
	//新增模块 远程下载mes
public:
	BOOL RemoveEQPInfomation(CString& strLotid, CString& EqpId);
	BOOL GetEQPInfomation( CString& strLotid, CString& EqpId);
	int  FindJsonValueStart(const CString& strJson, const CString& strKey);
	CString  ExtractJsonString(const CString& strJson, const CString& strKey);
	BOOL ExtractJsonBool(const CString& strJson, const CString& strKey);
	CString ExtractJsonObject(const CString& strJson, const CString& strKey);

};

