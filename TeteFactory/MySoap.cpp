#include "stdafx.h"
#include "MySoap.h"
#include <string>
#include "ht_src\soapH.h"
#include "ht_src\Service1Soap.nsmap"
#include "ht_src\WebServiceOf2DSoap.nsmap"
#include "ht_src\WebServiceOfTrackInSoap.nsmap"

struct soap m_oSoap;

///using namespace std;
/*
<LotInfoData>
<LotId>TK46E001.2</LotId>
<LotParams>
<LotParamInfo>
<ParamNo>1</ParamNo>
<ParamName>$TEXT1</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>2</ParamNo>
<ParamName>$TEXT2</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>3</ParamNo>
<ParamName>$TEXT3</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>4</ParamNo>
<ParamName>$TEXT4</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>5</ParamNo>
<ParamName>$TEXT5</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>6</ParamNo>
<ParamName>$TEXT6</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>7</ParamNo>
<ParamName>$TEXT7</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>8</ParamNo>
<ParamName>$TEXT8</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>9</ParamNo>
<ParamName>$TEXT9</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>10</ParamNo>
<ParamName>$TEXT10</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>11</ParamNo>
<ParamName>$ACUSTCODE</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>12</ParamNo>
<ParamName>$ACUSTLOTID</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>13</ParamNo>
<ParamName>$ACUSTDEVICE</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>14</ParamNo>
<ParamName>$APACKAGECODE</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>15</ParamNo>
<ParamName>$ORDERNO</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>16</ParamNo>
<ParamName>$BOCNO</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>17</ParamNo>
<ParamName>$STAGE</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>18</ParamNo>
<ParamName>$QTY</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>19</ParamNo>
<ParamName>$T4000-PGM</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
<LotParamInfo>
<ParamNo>20</ParamNo>
<ParamName>$T4000-REV</ParamName>
<ParamValue></ParamValue>
</LotParamInfo>
</LotParams>
</LotInfoData>
*/
std::wstring GenerateInputDataList(const wchar_t* sLotID)
{
	std::wstring stext;
	wchar_t wszbuff[1024] = { 0 };
	wsprintf(wszbuff, L"<?xml version=\"1.0\"?><LotInfoData><LotId>%s</LotId><LotParams>", sLotID);
	stext += wszbuff;
	for (int i = 1; i <= 10; i++)
	{
		wsprintf(wszbuff, L"<LotParamInfo><ParamNo>%d</ParamNo><ParamName>$TEXT%d</ParamName><ParamValue></ParamValue></LotParamInfo>"
			, i, i);
		stext += wszbuff;
	}

	static std::wstring sConstValue[] = { L"$ACUSTCODE", L"$ACUSTLOTID", L"$ACUSTDEVICE", L"$APACKAGECODE"
		, L"$ORDERNO", L"$BOCNO" , L"$STAGE", L"$QTY" , L"$T4000-PGM", L"$T4000-REV", L"$MARKING-SPEC" };
	for (int i = 11; i <= 21; i++)
	{
		wsprintf(wszbuff, L"<LotParamInfo><ParamNo>%d</ParamNo><ParamName>%s</ParamName><ParamValue></ParamValue></LotParamInfo>"
			, i, sConstValue[i - 11].c_str());
		stext += wszbuff;
	}

	wsprintf(wszbuff, L"</LotParams></LotInfoData>");
	stext += wszbuff;

	return stext;
}

CMySoap::CMySoap(const CString& sUrl)
{
	m_sUrl = sUrl;
	soap_init(&m_oSoap);
}


CMySoap::~CMySoap()
{
	/// 关闭SOAP
//	soap_destroy(&m_oSoap);
//	soap_end(&m_oSoap);
	soap_done(&m_oSoap);
}

void CMySoap::SetUrl(const CString& sUrl)
{
	m_sUrl = sUrl;
}

CString CMySoap::GetLotInfo(const CString& lotid)
{
	// WebService调用对象
	class _LotInfo__GetLotInfo getLotInfo;
	// WebService返回对象
	class _LotInfo__GetLotInfoResponse getLotInfoResponseObject;

	std::wstring sInputDataList = GenerateInputDataList(lotid.GetString());
	getLotInfo.InputDataList = (wchar_t*)sInputDataList.data();
	soap_set_namespaces(&m_oSoap, namespaces);
	int nResult = soap_call___LotInfo2__GetLotInfo(&m_oSoap, CStringA(m_sUrl).GetString(), NULL,
		&getLotInfo, &getLotInfoResponseObject);
	// 操作成功
	if (SOAP_OK == nResult && (NULL == getLotInfoResponseObject.Status) && (NULL != getLotInfoResponseObject.OutputDataList))
	{
		CString strRet(getLotInfoResponseObject.OutputDataList);
			soap_destroy(&m_oSoap);
			soap_end(&m_oSoap);
		return strRet;
	}
	
	m_sErrMsg = getLotInfoResponseObject.Status;
	soap_destroy(&m_oSoap);
	soap_end(&m_oSoap);
	return _T("");
}

BOOL CMySoap::IsLotIDMatchStripID(const CString& stripid, CString& lotid)
{
	// WebService调用对象
	_Web2D__CheckEQPStripIdIsMatchLotId getLotInfo;
	// WebService返回对象
	_Web2D__CheckEQPStripIdIsMatchLotIdResponse getLotInfoResponseObject;

	CString szSubStrateID = stripid;
	getLotInfo.subStrateId = (wchar_t*)szSubStrateID.GetString();
	soap_set_namespaces(&m_oSoap, namespaces1);
	int nResult = soap_call___Web2D2__CheckEQPStripIdIsMatchLotId(&m_oSoap, CStringA(m_sUrl).GetString(), NULL,
		&getLotInfo, &getLotInfoResponseObject);
	// 操作成功
	if (SOAP_OK != nResult)
	{
		soap_destroy(&m_oSoap);
		soap_end(&m_oSoap);
		m_sErrMsg = _T("通讯失败！");
		return FALSE;
	}
	CString sResult, sLotID;
	sLotID = (*getLotInfoResponseObject.CheckEQPStripIdIsMatchLotIdResult).Item1;
	sResult = (*getLotInfoResponseObject.CheckEQPStripIdIsMatchLotIdResult).Item2;
	if (sResult.MakeLower() == _T("success"))
	{
		soap_destroy(&m_oSoap);
		soap_end(&m_oSoap);	
		lotid = sLotID;
		return TRUE;
	}
	soap_destroy(&m_oSoap);
	soap_end(&m_oSoap);
	m_sErrMsg = sLotID;  // 失败信息
	return FALSE;
}

BOOL CMySoap::IsEquipmentTrackInLotId(const CString& sEquipmentId, const CString& slotid)
{
	// WebService调用对象
	_TrackIn__IsEquipmentTrackInLotId TrackInLotIdInfo;
	// WebService返回对象
	_TrackIn__IsEquipmentTrackInLotIdResponse getTrackInLotIdResponseObject;

	TrackInLotIdInfo.EquipId = (wchar_t*)sEquipmentId.GetString();
	TrackInLotIdInfo.LotId = (wchar_t*)slotid.GetString();

	soap_set_namespaces(&m_oSoap, namespaces2);
	int nResult = soap_call___TrackIn2__IsEquipmentTrackInLotId(&m_oSoap, CStringA(m_sUrl).GetString(), NULL,
		&TrackInLotIdInfo, &getTrackInLotIdResponseObject);
	// 操作成功
	if (SOAP_OK != nResult)
	{
		soap_destroy(&m_oSoap);
		soap_end(&m_oSoap);
		m_sErrMsg = _T("通讯失败！");
		return FALSE;
	}

	if (getTrackInLotIdResponseObject.IsEquipmentTrackInLotIdResult)
	{
		soap_destroy(&m_oSoap);
		soap_end(&m_oSoap);
		return TRUE;
	}
	m_sErrMsg = getTrackInLotIdResponseObject.status;
	soap_destroy(&m_oSoap);
	soap_end(&m_oSoap);
	return FALSE;
}

CString& CMySoap::GetLastError()
{
	return m_sErrMsg;
}