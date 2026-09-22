#include "stdafx.h"
#include "ICMesComEAPHttpQP.h"
#include <string>
#include "json\include\json.h"


using namespace std;
//#include "Dlg2DBarcodeCheck.h"

// ---------------------------------------------------------------------------
// 轻量 JSON 解析辅助函数（仅支持 string / number / bool / null / object / array）
// ---------------------------------------------------------------------------


void JsonSkipSpace(const CString& s, int& pos)
{
	while (pos < s.GetLength())
	{
		TCHAR ch = s[pos];
		if (ch == _T(' ') || ch == _T('\t') || ch == _T('\r') || ch == _T('\n'))
			++pos;
		else
			break;
	}
}

// 解析带引号的字符串，支持 \" \\ \/ \b \f \n \r \t \uXXXX 转义
bool JsonParseString(const CString& s, int& pos, CString& val)
{
	JsonSkipSpace(s, pos);
	if (pos >= s.GetLength() || s[pos] != _T('"'))
		return false;

	++pos;
	val.Empty();
	while (pos < s.GetLength())
	{
		TCHAR ch = s[pos];
		if (ch == _T('"'))
		{
			++pos;
			return true;
		}
		if (ch != _T('\\'))
		{
			val += ch;
			++pos;
			continue;
		}

		++pos; // 跳过反斜杠
		if (pos >= s.GetLength())
			return false;

		TCHAR esc = s[pos++];
		switch (esc)
		{
		case _T('n'): val += _T('\n'); break;
		case _T('t'): val += _T('\t'); break;
		case _T('r'): val += _T('\r'); break;
		case _T('b'): val += _T('\b'); break;
		case _T('f'): val += _T('\f'); break;
		case _T('u'):
		{
			if (pos + 4 > s.GetLength())
				return false;
			val += static_cast<TCHAR>(_tcstoul(s.Mid(pos, 4), nullptr, 16));
			pos += 4;
			break;
		}
		default: val += esc; break; // \" \\ \/ 及其它
		}
	}
	return false; // 字符串未闭合
}

// 跳过一个任意类型的值
bool JsonSkipValue(const CString& s, int& pos)
{
	JsonSkipSpace(s, pos);
	if (pos >= s.GetLength())
		return false;

	TCHAR ch = s[pos];
	if (ch == _T('"'))
	{
		CString tmp;
		return JsonParseString(s, pos, tmp);
	}

	if (ch == _T('{') || ch == _T('['))
	{
		int depth = 0;
		while (pos < s.GetLength())
		{
			TCHAR c = s[pos];
			if (c == _T('"'))
			{
				CString tmp;
				if (!JsonParseString(s, pos, tmp))
					return false;
				continue;
			}
			if (c == _T('{') || c == _T('['))
			{
				++depth;
			}
			else if (c == _T('}') || c == _T(']'))
			{
				--depth;
				if (depth == 0)
				{
					++pos;
					return true;
				}
			}
			++pos;
		}
		return false;
	}

	// number / true / false / null
	while (pos < s.GetLength())
	{
		TCHAR c = s[pos];
		if (c == _T(',') || c == _T('}') || c == _T(']') ||
			c == _T(' ') || c == _T('\t') || c == _T('\r') || c == _T('\n'))
			break;
		++pos;
	}
	return true;
}

// 解析一条记录对象：{ "lotid": "...", "eqpid": "..." }
// 进入时 pos 指向 '{'，结束时 pos 指向 '}' 之后
bool ParseOneLotEqpObject(const CString& s, int& pos, LotEqpItem& item)
{
	if (pos >= s.GetLength() || s[pos] != _T('{'))
		return false;
	++pos;

	while (pos < s.GetLength())
	{
		JsonSkipSpace(s, pos);
		if (pos >= s.GetLength())
			break;
		if (s[pos] == _T('}'))
		{
			++pos;
			return true;
		}
		if (s[pos] == _T(','))
		{
			++pos;
			continue;
		}

		CString strKey;
		if (!JsonParseString(s, pos, strKey))
			return false;

		JsonSkipSpace(s, pos);
		if (pos >= s.GetLength() || s[pos] != _T(':'))
			return false;
		++pos;

		if (strKey.CompareNoCase(_T("lotid")) == 0)
		{
			CString strVal;
			if (JsonParseString(s, pos, strVal))
				item.strLotId = strVal;
			else
				JsonSkipValue(s, pos);
		}
		else if (strKey.CompareNoCase(_T("eqpid")) == 0)
		{
			CString strVal;
			if (JsonParseString(s, pos, strVal))
				item.strEqpId = strVal;
			else
				JsonSkipValue(s, pos);
		}
		else
		{
			JsonSkipValue(s, pos);
		}
	}
	return false; // 对象未闭合
}


BOOL ParseLotEqpJson(const CString& strJson, LotEqpResponse& resp)
{
	resp.bStatus = FALSE;
	resp.strMessage.Empty();
	resp.listData.clear();

	int pos = 0;
	JsonSkipSpace(strJson, pos);
	if (pos >= strJson.GetLength() || strJson[pos] != _T('{'))
		return FALSE;
	++pos;

	while (pos < strJson.GetLength())
	{
		JsonSkipSpace(strJson, pos);
		if (pos >= strJson.GetLength())
			break;
		if (strJson[pos] == _T('}'))
		{
			++pos;
			break;
		}
		if (strJson[pos] == _T(','))
		{
			++pos;
			continue;
		}

		CString strKey;
		if (!JsonParseString(strJson, pos, strKey))
			return FALSE;

		JsonSkipSpace(strJson, pos);
		if (pos >= strJson.GetLength() || strJson[pos] != _T(':'))
			return FALSE;
		++pos;

		if (strKey.CompareNoCase(_T("Status")) == 0)
		{
			JsonSkipSpace(strJson, pos);
			if (strJson.Mid(pos, 4).CompareNoCase(_T("true")) == 0)
			{
				resp.bStatus = TRUE;
				pos += 4;
			}
			else if (strJson.Mid(pos, 5).CompareNoCase(_T("false")) == 0)
			{
				resp.bStatus = FALSE;
				pos += 5;
			}
			else
			{
				JsonSkipValue(strJson, pos);
			}
		}
		else if (strKey.CompareNoCase(_T("Message")) == 0)
		{
			CString strVal;
			if (JsonParseString(strJson, pos, strVal))
				resp.strMessage = strVal;
			else
				JsonSkipValue(strJson, pos);
		}
		else if (strKey.CompareNoCase(_T("Data")) == 0)
		{
			JsonSkipSpace(strJson, pos);

			// 单条记录："Data": { "lotid": "...", "eqpid": "..." }
			if (pos < strJson.GetLength() && strJson[pos] == _T('{'))
			{
				LotEqpItem item;
				if (ParseOneLotEqpObject(strJson, pos, item))
					resp.listData.push_back(item);
				else
					return FALSE;
				continue;
			}

			// 多条记录："Data": [ {...}, {...} ]
			if (pos >= strJson.GetLength() || strJson[pos] != _T('['))
			{
				JsonSkipValue(strJson, pos);	// null 或其它类型，忽略
				continue;
			}
			++pos;

			while (pos < strJson.GetLength())
			{
				JsonSkipSpace(strJson, pos);
				if (pos >= strJson.GetLength())
					break;
				if (strJson[pos] == _T(']'))
				{
					++pos;
					break;
				}
				if (strJson[pos] == _T(','))
				{
					++pos;
					continue;
				}

				if (strJson[pos] != _T('{'))
				{
					if (!JsonSkipValue(strJson, pos))
						return FALSE;
					continue;
				}

				LotEqpItem item;
				if (!ParseOneLotEqpObject(strJson, pos, item))
					return FALSE;
				resp.listData.push_back(item);
			}
		}
		else
		{
			JsonSkipValue(strJson, pos);
		}
	}

	return TRUE;
}

BOOL GroupLotByEqp(const std::list<LotEqpItem>& listData,
	std::list<std::pair<CString, std::list<CString>>>& listGroup)
{
	listGroup.clear();

	for (const LotEqpItem& item : listData)
	{
		auto itFound = listGroup.end();
		for (auto it = listGroup.begin(); it != listGroup.end(); ++it)
		{
			if (it->first == item.strEqpId)
			{
				itFound = it;
				break;
			}
		}

		if (itFound == listGroup.end())
			listGroup.push_back(std::make_pair(item.strEqpId, std::list<CString>()));

		listGroup.back().second.push_back(item.strLotId);
	}

	return TRUE;
}

// 测试用的接口返回数据：Data 为数组
CString GetTestLotEqpJson()
{

	//return CString(_T("{")) +
	//	_T("\"Status\": true,") +
	//	_T("\"Message\": \"查询成功\",") +
	//	_T("\"Data\":") +
	//	_T("{\"lotid\": \"TR34E314.3\", \"eqpid\": \"MK-0139\"}") +
	//	_T("}");

	return CString(_T("{")) +
		_T("\"Status\": true,") +
		_T("\"Message\": \"查询成功\",") +
		_T("\"Data\": [") +
		_T("{\"lotid\": \"TR34E314.3\", \"eqpid\": \"MK-0139\"},") +
		_T("{\"lotid\": \"TR34E314.4\", \"eqpid\": \"MK-0139\"},") +
		_T("{\"lotid\": \"TR34A087.6\", \"eqpid\": \"MK-0132\"},") +
		_T("{\"lotid\": \"TR34F070.3\", \"eqpid\": \"MK-0139\"},") +
		_T("{\"lotid\": \"TR34F070.2\", \"eqpid\": \"MK-0139\"},") +
		_T("{\"lotid\": \"TR34A087.4\", \"eqpid\": \"MK-0132\"},") +
		_T("{\"lotid\": \"TR34D400.5\", \"eqpid\": \"MK-0139\"},") +
		_T("{\"lotid\": \"TR34D400.9\", \"eqpid\": \"MK-0139\"}") +
		_T("]}");
}

// 测试用的接口返回数据：Data 为单个对象
CString GetTestLotEqpJsonSingle()
{
	return CString(_T("{")) +
		_T("\"Status\": true,") +
		_T("\"Message\": \"查询成功\",") +
		_T("\"Data\":") +
		_T("{\"lotid\": \"TR34E314.3\", \"eqpid\": \"MK-0139\"}") +
		_T("}");
}
//


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
	m_pWorkCtrol = p;

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





//远程下载mes信息
// 简单 JSON 解析辅助函数：定位某个 key 对应的值的起始位置
// 返回：value 在 strJson 中的起始下标（跳过空白后），找不到返回 -1
int  CICMesComEAPHttpQp::FindJsonValueStart(const CString& strJson, const CString& strKey)
{
	CString strPattern = _T("\"") + strKey + _T("\":");
	int nPos = strJson.Find(strPattern);
	if (nPos < 0)
		return -1;

	int nStart = nPos + strPattern.GetLength();
	while (nStart < strJson.GetLength() && (strJson[nStart] == _T(' ') || strJson[nStart] == _T('\t')))
		nStart++;

	return (nStart < strJson.GetLength()) ? nStart : -1;
}

// 简单 JSON 解析辅助函数：提取某个 key 对应的字符串值
// 例如 ExtractJsonString(strInfo, _T("Message")) 返回 "查询成功"
CString  CICMesComEAPHttpQp::ExtractJsonString(const CString& strJson, const CString& strKey)
{
	int nStart = FindJsonValueStart(strJson, strKey);
	if (nStart < 0 || strJson[nStart] != _T('"'))
		return CString();

	int nEnd = strJson.Find(_T('"'), nStart + 1);
	if (nEnd < 0)
		return CString();

	return strJson.Mid(nStart + 1, nEnd - nStart - 1);
}

// 简单 JSON 解析辅助函数：提取某个 key 对应的布尔值
// 例如 ExtractJsonBool(strInfo, _T("Status")) 返回 TRUE
BOOL  CICMesComEAPHttpQp::ExtractJsonBool(const CString& strJson, const CString& strKey)
{
	int nStart = FindJsonValueStart(strJson, strKey);
	if (nStart < 0)
		return FALSE;

	if (strJson.Mid(nStart, 4).CompareNoCase(_T("true")) == 0)
		return TRUE;

	return FALSE;
}

// 简单 JSON 解析辅助函数：提取某个 key 对应的对象值 { ... }
// 会正确配对嵌套的花括号
CString  CICMesComEAPHttpQp::ExtractJsonObject(const CString& strJson, const CString& strKey)
{
	int nStart = FindJsonValueStart(strJson, strKey);
	if (nStart < 0 || strJson[nStart] != _T('{'))
		return CString();

	int nDepth = 0;
	for (int i = nStart; i < strJson.GetLength(); i++)
	{
		if (strJson[i] == _T('{'))
			nDepth++;
		else if (strJson[i] == _T('}'))
		{
			nDepth--;
			if (nDepth == 0)
				return strJson.Mid(nStart, i - nStart + 1);
		}
	}

	return CString();
}
BOOL CICMesComEAPHttpQp::RemoveEQPInfomation(CString& strLotid, CString& EqpId)
{
	//CString strIP = _T("127.0.0.1");
	//int nPort = 8090;
	CHttpFile* pFile;
	try
	{
		CHttpConnection* pConnection = m_pSession->GetHttpConnection(m_strIP, (INTERNET_PORT)m_nPort);
		CString sFormActionUrl;
		sFormActionUrl.Format(_T("/api/WaferCIM/GetHtmEqpLot?lotid=%s&eqpid=%s&fatr=true"), strLotid, EqpId);// = ;
		pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFormActionUrl);
		if (m_pWorkCtrol != NULL)
		{
			m_pWorkCtrol->PrintMesMessage(emFactoryMsgType_t::emMsgType_Normal, _T("SendMsg:") + sFormActionUrl);
		}
	}
	catch (...)
	{
		return FALSE;
	}

	CString strCmd;
	strCmd.Format(_T(""));
	int nIndex = 0;
	CString strBuff;

	int len = strCmd.GetLength();
	char* pchar = new char[len] {0};
	WideCharToMultiByte(CP_ACP, NULL, strCmd, strCmd.GetLength(), pchar, len, NULL, NULL);
	CString strHeaders = _T("Content-Type: application/json;charset=utf-8\r\n"); // 请求头
	BOOL result = FALSE;
	try
	{
		pFile->AddRequestHeaders(strHeaders);

		result = pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)_T(""), 0);
		delete pchar;
		pchar = NULL;
	}
	catch (...)
	{
		//	PrintErrorMsg(_T("网址请求失败！"));
		delete pchar;
		pchar = NULL;
		return FALSE;
	}

	DWORD dwStatusCode;
	std::string content;
	pFile->QueryInfoStatusCode(dwStatusCode);
	int nLen = 0;
	char recvbuff[1024] = { 0 };

	if (HTTP_STATUS_OK == dwStatusCode)
	{
		while (true)
		{
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
	//CString strJson = CA2W(content.c_str());
	CString strJson = chU2T(content).c_str();
	if (m_pWorkCtrol != NULL)
	{
		m_pWorkCtrol->PrintMesMessage(emFactoryMsgType_t::emMsgType_Normal, _T("RecvMsg:") + strJson);
	}

	ParseLotEqpJson(strJson, m_LotEqpResponse);
	if (m_LotEqpResponse.bStatus && !m_LotEqpResponse.listData.empty())
	{
		strLotid = m_LotEqpResponse.listData.front().strLotId;
		EqpId = m_LotEqpResponse.listData.front().strEqpId;
	}
	else
	{
		return false;
	}
	//
	/*CString strMessage = ExtractJsonString(strJson, _T("Message"));
	BOOL bStatus = ExtractJsonBool(strJson, _T("Status"));
	CString strData = ExtractJsonObject(strJson, _T("Data"));


	if (bStatus)
	{
		strLotid = ExtractJsonString(strData, _T("lotid"));
		EqpId = ExtractJsonString(strData, _T("eqpid"));
	}*/


	return true;
}



BOOL CICMesComEAPHttpQp::GetEQPInfomation( CString& strLotid, CString& EqpId)
{
	//CString strIP = _T("127.0.0.1");
	//int nPort = 8090;
	CHttpFile* pFile;
	try
	{
		CHttpConnection* pConnection = m_pSession->GetHttpConnection(m_strIP, (INTERNET_PORT)m_nPort);
		CString sFormActionUrl;
		sFormActionUrl.Format(_T("/api/WaferCIM/GetHtmEqpLot?lotid=""&eqpid=%s&fatr=false"), EqpId);// = ;
		pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFormActionUrl);
		if (m_pWorkCtrol != NULL)
		{
			m_pWorkCtrol->PrintMesMessage(emFactoryMsgType_t::emMsgType_Normal, _T("SendMsg:") + sFormActionUrl);
		}
	}
	catch (...)
	{
		return FALSE;
	}

	CString strCmd;
	strCmd.Format(_T(""));
	int nIndex = 0;
	CString strBuff;

	int len = strCmd.GetLength();
	char* pchar = new char[len] {0};
	WideCharToMultiByte(CP_ACP, NULL, strCmd, strCmd.GetLength(), pchar, len, NULL, NULL);
	CString strHeaders = _T("Content-Type: application/json;charset=utf-8\r\n"); // 请求头
	BOOL result = FALSE;
	try
	{
			pFile->AddRequestHeaders(strHeaders);

		result = pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)_T(""), 0);
		delete pchar;
		pchar = NULL;
	}
	catch (...)
	{
		//	PrintErrorMsg(_T("网址请求失败！"));
		delete pchar;
		pchar = NULL;
		return FALSE;
	}

	DWORD dwStatusCode;
	std::string content;
	pFile->QueryInfoStatusCode(dwStatusCode);
	int nLen = 0;
	char recvbuff[1024] = { 0 };

	if (HTTP_STATUS_OK == dwStatusCode)
	{
		while (true)
		{
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
	//CString strJson = CA2W(content.c_str());
	CString strJson = chU2T(content).c_str();
	if (m_pWorkCtrol != NULL)
	{
		m_pWorkCtrol->PrintMesMessage(emFactoryMsgType_t::emMsgType_Normal, _T("RecvMsg:") + strJson);
	}  

	ParseLotEqpJson(strJson, m_LotEqpResponse);
	if(m_LotEqpResponse.bStatus && !m_LotEqpResponse.listData.empty())
	{
		strLotid = m_LotEqpResponse.listData.front().strLotId;
		EqpId = m_LotEqpResponse.listData.front().strEqpId;
	}
	else
	{
		return false;
	}
	//
	/*CString strMessage = ExtractJsonString(strJson, _T("Message"));
	BOOL bStatus = ExtractJsonBool(strJson, _T("Status"));
	CString strData = ExtractJsonObject(strJson, _T("Data"));


	if (bStatus)
	{
		strLotid = ExtractJsonString(strData, _T("lotid"));
		EqpId = ExtractJsonString(strData, _T("eqpid"));
	}*/


	return true;
}





