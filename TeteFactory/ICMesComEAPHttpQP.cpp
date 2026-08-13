#include "stdafx.h"
#include "ICMesComEAPHttpQP.h"
#include <string>
#include "json\include\json.h"


using namespace std;
//#include "Dlg2DBarcodeCheck.h"



CICMesComEAPHttpQp::CICMesComEAPHttpQp()
{
	m_pSession = new CInternetSession(_T("HttpClient"));
	m_pWorkCtrol = NULL;
}

CICMesComEAPHttpQp::~CICMesComEAPHttpQp()
{
	if (nullptr == m_pSession) {
		m_pSession->Close();
		delete m_pSession;
		m_pSession = nullptr;
	}
}

bool CICMesComEAPHttpQp::ConnectMes(CString strIP,int nPort, CFormWorkControl* p)
{
	m_strIP = strIP;	
	m_nPort = nPort;	
	return true;
}

// ========== 7. JSON 解析函数 ==========
void SplitCstring(CString str, CString split, std::vector<CString>& vData)
{
	// 先清空输出容器
	vData.clear();

	// 边界判断：空字符串 或 分隔符为空直接返回
	if (str.IsEmpty() || split.IsEmpty())
	{
		if (!str.IsEmpty())
			vData.push_back(str);
		return;
	}

	int nSplitLen = split.GetLength();
	int nPos = 0;
	CString strTemp;

	// 循环查找分隔符位置
	while ((nPos = str.Find(split)) != -1)
	{
		// 截取分隔符前面的一段
		strTemp = str.Left(nPos);
		vData.push_back(strTemp);
		// 截掉已处理部分，剩下后面继续循环
		str = str.Mid(nPos + nSplitLen);
	}
	str.TrimLeft();
	str.TrimRight();
	// 最后剩下末尾一段也要加入
	if (!str.IsEmpty())
	{
		vData.push_back(str);
	}
}
void ParseJsonResponse(const CString& strResponse , vector< vector<CString> > &MapTemp)
{
	vector<CString>vData;

	SplitCstring(strResponse, _T(","), vData);
	
	for (int i = 0; i < vData.size(); i++)
	{
		CString strInfo = vData[i];
		strInfo.Replace(_T("\\"), _T(""));
		vector<CString>temp;
		SplitCstring(strInfo, _T(":"), temp);
		MapTemp.push_back(temp);
	}
	
}
void GetKeyValue(vector< vector<CString> >MapTemp,CString strKey, CString&strValue)
{
	
	strValue.Empty();
	for (int i=0;i<MapTemp.size();i++)
	{
		
		vector<CString> it = MapTemp[i];
		if (it.size() != 2)
		{
			continue;
		}
		int nRet = it[0].Find(strKey);
		if (nRet >=0)
		{
			strValue = it[1];
			break;
		}
	}

}
/**
* POST模式
* 通讯实例-JSON格式，UTF8编码
*发送：
*
*/
bool CICMesComEAPHttpQp::GetFinishLotInfo(map<CString, CString> MapInfo)
{
	
	CHttpFile* pFile;
	try 
	{
		CHttpConnection *pConnection = m_pSession->GetHttpConnection(m_strIP, (INTERNET_PORT)m_nPort);
		CString sFormActionUrl =  _T("/EQPDataRecord/PutLMKQTY/");
		pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFormActionUrl);
	}
	catch (...) 
	{
		return false;
	}
	//CString sRequestCmd = _T("{")
	//	_T("\"EQPID\": \"MK-0001\",")
	//	_T("\"LOTID\": \"X2628000083.17\",")
	//	_T("\"EQPMODEL\": \"HTM-3032\",")
	//	_T("\"QTY\": \"18005\",")
	//	_T("\"DEFECTQTY\": \"15\"")
	//	_T("}");
	//CString strEQPID, strLotID, strEQPModel, strQTY, strDefectQTY;
	//strEQPID = _T("MK-0001");
	//strLotID = _T("X2628000083.17");
	//strEQPModel = _T("HTM-3032");
	//strQTY = _T("18005");
	//strDefectQTY = _T("15");
	
	CString strCmd;
	strCmd = _T("{");
	int nsize = MapInfo.size();
	int nIndex = 0;
	CString strBuff;
	for(auto it:MapInfo)
	{
		if (nIndex != nsize - 1)
		{
			strBuff.Format(_T("\"%s\": \"%s\","), it.first, it.second);
			nIndex++;
		}
		else
		{
			strBuff.Format(_T("\"%s\": \"%s\""), it.first, it.second);

		}
		strCmd += strBuff;
	}
	strCmd += _T("}");
	int len = strCmd.GetLength();
	char* pchar = new char[len] {0};
	WideCharToMultiByte(CP_ACP, NULL, strCmd, strCmd.GetLength(), pchar, len, NULL, NULL);
	CString strHeaders = _T("Content-Type: application/json;charset=utf-8\r\n"); // 请求头
	if (m_pWorkCtrol!= NULL)
	{
		m_pWorkCtrol->PrintMessage(emFactoryMsgType_t::emMsgType_Normal, strCmd);
	}
	BOOL result = false;
	try 
	{
		
		result = pFile->SendRequest(strHeaders, (LPVOID)pchar, len );
		delete pchar;
		pchar =NULL;
	}
	catch (...)
	{
	//	PrintErrorMsg(_T("网址请求失败！"));
		delete pchar;
		pchar = NULL;
		return false;
	}

	DWORD dwStatusCode;
	std::string content;
	pFile->QueryInfoStatusCode(dwStatusCode);
	int nLen = 0;
	if (HTTP_STATUS_OK == dwStatusCode) 
	{
		while (true) 
		{
			char recvbuff[1024] = { 0 };
			int irecvlen = pFile->Read((void*)recvbuff, 1023);
			if (0 >= irecvlen)
				break;
			recvbuff[irecvlen] = '\0';
			content += recvbuff;
			if (irecvlen < 1023)
				break;
		}
	}
	pFile->Close();
	delete pFile;
	//std::string strStd = "Hello";
	CString strCStr = CA2W(content.c_str());
	if (m_pWorkCtrol!= NULL)
	{
		m_pWorkCtrol->PrintMessage(emFactoryMsgType_t::emMsgType_Normal, _T("Mes RecvInfo:") + strCStr);
	}
	vector< vector<CString>> vvData;
	ParseJsonResponse(strCStr, vvData);
	CString strSucess, strCode, strMsg;
	GetKeyValue(vvData, _T("success"), strSucess);
	GetKeyValue(vvData, _T("code"), strCode);
	GetKeyValue(vvData, _T("msg"), strMsg);
	/*Json::CharReaderBuilder builder;
	Json::Value root;
	JSONCPP_STRING err;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	Json::Value jsValue;
	if (!reader->parse(recvbuff, recvbuff + strlen(recvbuff), &root, &err))
		goto CleanUp;*/
	if(strSucess.Find(_T("true"))>=0)
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

bool CICMesComEAPHttpQp::QueryMesCheckLotInfo(map<CString, CString>& MapInfo)
{

	CHttpFile* pFile;
	try
	{
		CHttpConnection* pConnection = m_pSession->GetHttpConnection(m_strIP, (INTERNET_PORT)m_nPort);
		CString sFormActionUrl = _T("/api/WaferCIM/GetHtmEqpLot");
		pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFormActionUrl);
	}
	catch (...)
	{
		return false;
	}
	//CString sRequestCmd = _T("{")
	//	_T("\"EQPID\": \"MK-0001\",")
	//	_T("\"LOTID\": \"X2628000083.17\",")
	//	_T("\"EQPMODEL\": \"HTM-3032\",")
	//	_T("\"QTY\": \"18005\",")
	//	_T("\"DEFECTQTY\": \"15\"")
	//	_T("}");
	//CString strEQPID, strLotID, strEQPModel, strQTY, strDefectQTY;
	//strEQPID = _T("MK-0001");
	//strLotID = _T("X2628000083.17");
	//strEQPModel = _T("HTM-3032");
	//strQTY = _T("18005");
	//strDefectQTY = _T("15");

	CString strCmd;
	strCmd = _T("{");
	int nsize = MapInfo.size();
	int nIndex = 0;
	CString strBuff;
	for (auto it : MapInfo)
	{
		if (nIndex != nsize - 1)
		{
			strBuff.Format(_T("\"%s\": \"%s\","), it.first, it.second);
			nIndex++;
		}
		else
		{
			strBuff.Format(_T("\"%s\": \"%s\""), it.first, it.second);

		}
		strCmd += strBuff;
	}
	strCmd += _T("}");
	int len = strCmd.GetLength();
	char* pchar = new char[len] {0};
	WideCharToMultiByte(CP_ACP, NULL, strCmd, strCmd.GetLength(), pchar, len, NULL, NULL);
	CString strHeaders = _T("Content-Type: application/json;charset=utf-8\r\n"); // 请求头
	if (m_pWorkCtrol != NULL)
	{
		m_pWorkCtrol->PrintMessage(emFactoryMsgType_t::emMsgType_Normal, strCmd);
	}
	BOOL result = false;
	try
	{

		result = pFile->SendRequest(strHeaders, (LPVOID)pchar, len);
		delete pchar;
		pchar = NULL;
	}
	catch (...)
	{
		//	PrintErrorMsg(_T("网址请求失败！"));
		delete pchar;
		pchar = NULL;
		return false;
	}

	DWORD dwStatusCode;
	std::string content;
	pFile->QueryInfoStatusCode(dwStatusCode);
	int nLen = 0;
	char recvbuff[1024] = { 0 };
	int irecvlen = 0;
	if (HTTP_STATUS_OK == dwStatusCode)
	{
		while (true)
		{
			
			 irecvlen = pFile->Read((void*)recvbuff, 1023);
			if (0 >= irecvlen)
				break;
			recvbuff[irecvlen] = '\0';
			content += recvbuff;
			if (irecvlen < 1023)
				break;
		}
	}
	pFile->Close();
	delete pFile;
	//std::string strStd = "Hello";
	CString strCStr = CA2W(content.c_str());
	if (m_pWorkCtrol != NULL)
	{
		m_pWorkCtrol->PrintMessage(emFactoryMsgType_t::emMsgType_Normal, _T("Mes RecvInfo:") + strCStr);
	}
	//vector< vector<CString>> vvData;
	//ParseJsonResponse(strCStr, vvData);
	//CString strSucess, strCode, strMsg;
	//GetKeyValue(vvData, _T("success"), strSucess);
	//GetKeyValue(vvData, _T("code"), strCode);
	//GetKeyValue(vvData, _T("msg"), strMsg);
	char* pCharRecv = new char[irecvlen] { 0 };
	memcpy(pCharRecv,recvbuff, irecvlen);
	Json::CharReaderBuilder builder;
	Json::Value root;
	JSONCPP_STRING err;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	Json::Value jsValue;
	if (!reader->parse(recvbuff, recvbuff + strlen(recvbuff), &root, &err))
	{
		delete pCharRecv;
		pCharRecv = NULL;
		return false;
	}
	const Json::String jsStatus = root["Status"].toStyledString();
	const Json::String jsMessage = root["Message"].toStyledString();
	CString strStatus(jsStatus.c_str());
	CString strMsg(jsMessage.c_str());
	if (strStatus.Find(_T("true")) < 0)
	{
		return false;
	}
	// 检测成功 
	 Json::Value jsValueRet = root["Data"];
	 const Json::String jsLotid = jsValueRet["lotid"].toStyledString();
	 const Json::String jsEqpId = jsValueRet["eqpid"].toStyledString();
	 CString strLotId(jsLotid.c_str());
	 CString strEqpId(jsEqpId.c_str());
	 if (m_pWorkCtrol != NULL)
	 {
		 m_pWorkCtrol->PrintMessage(emFactoryMsgType_t::emMsgType_Normal, _T("Mes RecvInfo:LotId:") + strLotId + _T(" Eqpid:") + strEqpId);
	 }
	return true;
	
}

CString CICMesComEAPHttpQp::String2CString(Json::String str)
{
	return CString(str.c_str());
	//return CString();
}





