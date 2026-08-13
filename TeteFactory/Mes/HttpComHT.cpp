#include "stdafx.h"
#include "HttpComHT.h"
#include<afxinet.h>
#include "json\include\json.h"
#include "tlString.h"
#include "..\FormWorkControl.h"

//#define DEBUG_TEST

CHttpComHT::CHttpComHT(CFormWorkControl * pMainDlg)
{
	m_pConfig = CFactoryConfig::Instance();
	m_pMainDlg = pMainDlg;
}

CHttpComHT::~CHttpComHT()
{
}

bool CHttpComHT::ConnectMes()
{
	return true;
}

bool CHttpComHT::CloseMes()
{
	return true;
}

bool CHttpComHT::GetWaferThickness(const CString& sLotID)
{
	csLotID = sLotID;
	if (csLotID == L"")
	{
		return false;
	}
	return PostDataToServer(WaferThicknessPos);
}

float CHttpComHT::ReturnThickNess()
{
	return m_fthickness;
}


CHttpFile* CHttpComHT::WaferThicknessPos(CHttpConnection* pConnection, const CString& rootObject, const CString& strData, const CString &csLotID)
{
	if (nullptr == pConnection)
		return nullptr;
	CString path;
	path.Format(_T("/LabelPrintARC/GetMesPackageTickness?LotID=%s"), csLotID);
	CHttpFile* pHttpFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, rootObject + path, NULL, 1, NULL, NULL, 0);
	return pHttpFile;

}


bool CHttpComHT::PostDataToServer(PostData pFunc)
{
	CSingleLock lock(&m_csMes, TRUE);
	if (lock.IsLocked())
	{
		CInternetSession session(_T("HttpComHT"));
		CHttpConnection* pConnection = nullptr;
		CHttpFile* pHttpFile = nullptr;
		bool bPostRet = false;
		Json::CharReaderBuilder builder;
		Json::Value root;
		JSONCPP_STRING err;
		CStringA strResponse = "";
		// reader将Json字符串解析到root，root将包含Json里所有子元素
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		try
		{
			// 解析 URL
			CString strServer, strObject;
			INTERNET_PORT nPort;
			DWORD dwServiceType;
			if (!AfxParseURL(m_pConfig->m_sMesThickUrl/*_T("http://127.0.0.1:8080")*/, dwServiceType, strServer, strObject, nPort))
			{
				goto CleanUp;
			}
			// 建立连接
			pConnection = session.GetHttpConnection(strServer, nPort);
			//创建 POST 数据
			CString postData;
			if (!strObject.IsEmpty() &&
				(strObject.Right(1) == _T("/") || strObject.Right(1) == _T("\\")))
			{
				strObject = strObject.Left(strObject.GetLength() - 1);
			}
			pHttpFile = pFunc(pConnection, strObject, postData, csLotID);
			if (!pHttpFile)
			{
				goto CleanUp;
			}
			pHttpFile->AddRequestHeaders(_T("Content-Type: application/json"));

			// 发送请求
			pHttpFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)_T(""), 0);
			

			// 读取响应
			std::string content, data;
			while (true) {
				char recvbuff[1024] = { 0 };
				int irecvlen = pHttpFile->Read((void*)recvbuff, 1023);
				if (0 >= irecvlen)
					break;
				recvbuff[irecvlen] = '\0';
				data = recvbuff;
				content += data;
				if (irecvlen < 1023)
					break;
			}
			strResponse = CStringA(content.c_str());
		}
		catch (CInternetException* e)
		{
			TCHAR szErr[512] = { 0 };
			e->GetErrorMessage(szErr, 512);
			e->Delete();
			goto CleanUp;
		}
		const char* recvbuff = strResponse.GetString();
		if (nullptr == recvbuff)
			goto CleanUp;
		if (!reader->parse(recvbuff, recvbuff + strlen(recvbuff), &root, &err))
			goto CleanUp;
		try
		{
			if (root.isObject() && root["success"].asString()=="true")
				if (root["data"].isString()) 
				{
					m_fthickness = std::stof(root["data"].asString());
					bPostRet = true;
				}
		}
		catch (...)
		{
			goto CleanUp;
		}
		// 清理
	CleanUp:
		if (pHttpFile) pHttpFile->Close();
		if (pConnection) pConnection->Close();
		session.Close();
		return bPostRet;
	}
}


bool CHttpComHT::CheckProductType(const CString& strLotID, const CString& strProductType, CString& errmsg)
{
	bool bResult = false;
	CInternetSession session(_T("HttpComHT"));
	CHttpFile* pFile = nullptr;
	CHttpConnection *pConnection = nullptr;
	CString sFormActionUrl;
	Json::CharReaderBuilder builder;
	Json::Value root;
	JSONCPP_STRING err;
	// reader将Json字符串解析到root，root将包含Json里所有子元素
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	try {
#ifndef DEBUG_TEST
		// 解析 URL
		CString strServer, strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		if (!AfxParseURL(m_pConfig->m_sMesProductType, dwServiceType, strServer, strObject, nPort))
		{
			errmsg.Format(_T("检查产品类型的URL[%s]格式错误"), m_pConfig->m_sMesProductType);
			goto CleanUp;
		}
		pConnection = session.GetHttpConnection(strServer, nPort);
		sFormActionUrl.Format(_T("/api/LotData/GetLotIdAndPatternByStatus?LotId=%s&Pattern=%s"), strLotID, strProductType);
		//AfxMessageBox(sFormActionUrl);
		m_pMainDlg->PrintMessage(emMsgType_Normal, sFormActionUrl);
		
		// 打开网址
		pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, sFormActionUrl);
		if (nullptr == pFile) {
			errmsg.Format(_T("打开网址[%s]失败"), sFormActionUrl);
			goto CleanUp;
		}
		// 添加头部
		if (!pFile->AddRequestHeaders(_T("Content-Type: application/json"))) {
			errmsg.Format(_T("添加头部参数失败"));
			goto CleanUp;
		}
		// 发送请求
		if (!pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)_T(""), 0)) {
			errmsg.Format(_T("发送请求失败"));
			goto CleanUp;
		}

		// 接收数据
		DWORD dwStatusCode;
		std::string content;
		pFile->QueryInfoStatusCode(dwStatusCode);
		if (HTTP_STATUS_OK == dwStatusCode) {
			while (true) {
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
		else {
			errmsg.Format(_T("获取数据失败！\r\n%d"), dwStatusCode);
			goto CleanUp;
		}
#else
		std::string content = "{\"Status\":true,  \"Message\":\"Get Success\",  \"Data\":{\"Status\":\"N\"}}";
#endif
		// 处理数据
		const char* recvbuff = content.c_str();
		if (nullptr == recvbuff) {
			errmsg.Format(_T("返回数据为空！"));
			goto CleanUp;
		}

		m_pMainDlg->PrintMessage(emMsgType_Normal, chU2T(content).c_str());
			
		if (!reader->parse(recvbuff, recvbuff + strlen(recvbuff), &root, &err))
		{
			errmsg.Format(_T("返回数据的格式不是标准的Json格式！\r\n%s"), chU2T(content).c_str());
			goto CleanUp;
		}
		try
		{
			Json::Value JsStatus = root["Status"];
			Json::Value JsMessage = root["Message"];
			bool bStatus = JsStatus.asBool();
			if (!bStatus) {
				errmsg.Format(_T("%s"), chU2T(JsMessage.asString()).c_str());
				goto CleanUp;
			}
			Json::Value JsData = root["Data"];
			Json::Value jsdataStatus = JsData["Status"];
			CString strResult = chU2T(jsdataStatus.asString()).c_str();
			if (strResult != _T("Y")) {
				errmsg.Format(_T("产品类型校验返回:N"));
				goto CleanUp;
			}
		}
		catch (...)
		{
			errmsg.Format(_T("返回数据的格式不符合要求！\r\n%s"), chU2T(content).c_str());
			goto CleanUp;
		}
	}
	catch (CInternetException* e)
	{
		TCHAR szErr[512] = { 0 };
		e->GetErrorMessage(szErr, 512);
		e->Delete();
		errmsg = szErr;
		goto CleanUp;
	}

	bResult = true;

CleanUp:
#ifndef DEBUG_TEST
	if (pFile) pFile->Close();
	if (pConnection) pConnection->Close();
	session.Close();
#endif
	return bResult;
}


