#pragma once
#include "FormWorkControl.h"
#include <afxinet.h>
#include <map>
#include <vector>
using namespace std;
#define TString std::wstring
#include <string>
#include "json\include\json.h"
class CICMesComEAPHttpQp 
{
public:
	CICMesComEAPHttpQp();
	virtual ~CICMesComEAPHttpQp();
	virtual bool ConnectMes(CString strIP, int nPort, CFormWorkControl* p =NULL);
	
	virtual bool GetFinishLotInfo(map<CString, CString> MapInfo);
	/**
	* @brief 上传设备状态到MES
	* @param stDeviceStatus 设备的状态。@see emDeviceStatus_t
	*/	
	//   新增需求  通过设备号和 工卡号 请求mes数据校验 进站之前校验

	bool QueryMesCheckLotInfo(map<CString, CString>& MapInfo);
	CString String2CString(Json::String str);
public:
	CFormWorkControl* m_pWorkCtrol;
private:
	CInternetSession* m_pSession;
	CString m_strIP;
	int m_nPort;
	CString m_sReadStripID;
	std::vector<CString> m_vs2DBarcode;   // 2D Barcode数据
};

