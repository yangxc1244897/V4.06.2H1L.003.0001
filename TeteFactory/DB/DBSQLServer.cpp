#include "stdafx.h"
#include "DBSQLServer.h"
#include <assert.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDBSQLServer::CDBSQLServer()
{	
	m_pConn.CreateInstance(__uuidof(Connection));
	m_pRst.CreateInstance(__uuidof(Recordset));
	m_pCmd.CreateInstance(__uuidof(Command));
}

CDBSQLServer::~CDBSQLServer()
{
	CloseDB();
}

bool CDBSQLServer::ConnectDB(const stDBConnectInfo_t & stDBConInfo)
{
	try
	{
		if (m_pConn->GetState())
			m_pConn->Close();

		TCHAR connectString[200];
		swprintf_s(connectString, _T("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%d")
			, stDBConInfo.pwd.c_str()
		    , stDBConInfo.user.c_str()
		    , stDBConInfo.db.c_str()
		    , stDBConInfo.ip.c_str()
		    , stDBConInfo.port);
		m_pConn->ConnectionString = connectString;
		m_pConn->Open(connectString, _T(""), _T(""), adConnectUnspecified);
		m_pCmd->put_ActiveConnection(_variant_t((IDispatch*)m_pConn));
	}
	catch (_com_error &e)///捕捉异常
	{
		TCHAR errormessage[2048];
		swprintf_s(errormessage, _T("数据库[IP:%s DB:%s]连接失败!\r\n, 错误信息:%s")
			, stDBConInfo.ip.c_str()
			, stDBConInfo.db.c_str()
			, ParseComError(e).c_str());
		m_sErr = errormessage;
		return false;
	}
	return true;
}

void CDBSQLServer::CloseDB()
{
	if (m_pRst) {
		if (m_pRst->GetState()) {
			m_pRst->Close();
		}
	}

	if (m_pConn) {
		if (m_pConn->GetState()) {
			m_pConn->Close();
		}
	}
}

bool CDBSQLServer::Executesql(const TString & sql)
{
	try
	{
		m_pCmd->CommandText = sql.c_str();
		m_pCmd->Execute(NULL, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		TCHAR errormessage[2048];
		swprintf_s(errormessage, _T("执行SQL[%s]出错!\r\n错误信息:%s")
			, sql.c_str()
			, ParseComError(e).c_str());
		m_sErr = errormessage;
		return false;
	}
	return true;
}

bool CDBSQLServer::IsExistTable(const TString & sTable)
{
	TCHAR sql[200];
	swprintf_s(sql, _T("SELECT * FROM SYS.OBJECTS WHERE NAME = '%s'")
	, sTable.c_str());
	try {
		if (m_pRst->GetState()) {
			m_pRst->Close();
		}
		m_pRst->Open((_bstr_t)sql, (IDispatch *)m_pConn, adOpenDynamic, adLockOptimistic, adCmdText);
		if (!m_pRst->rsEOF) {
			m_pRst->Close();
			return false;
		}
	}
	catch (_com_error e)
	{
		TCHAR errormessage[2048];
		swprintf_s(errormessage, _T("执行SQL[%s]出错!\r\n错误信息:%s")
			, sql
			, ParseComError(e).c_str());
		m_sErr = errormessage;
		return false;
	}
	m_pRst->Close();
	return true;
}

bool CDBSQLServer::GetRecord(const TString & sql, const DBFIELDS& vFields, DBRECORDSET& vRecords)
{
	try {
		if (m_pRst->GetState()) {
			m_pRst->Close();
		}
		m_pRst->Open((_bstr_t)sql.c_str(), (IDispatch *)m_pConn, adOpenDynamic, adLockOptimistic, adCmdText);
		while (!m_pRst->rsEOF) {
			DBRECORD dbRecord;
			for (int i = 0, nC = vFields.size(); i < nC; i++) {
				dbRecord.push_back(Vt2Str(m_pRst->GetCollect(vFields[i].c_str())));
			}
			vRecords.push_back(dbRecord);
			m_pRst->MoveNext();
		}
	}
	catch (_com_error e)
	{
		TCHAR errormessage[2048];
		swprintf_s(errormessage, _T("执行SQL[%s]出错!\r\n错误信息:%s")
			, sql.c_str()
			, ParseComError(e).c_str());
		m_sErr = errormessage;
		return false;
	}
	m_pRst->Close();
	return true;
}

int CDBSQLServer::GetRecordCount(const TString & sql)
{
	int nRecordC = 0;
	try {
		if (m_pRst->GetState()) {
			m_pRst->Close();
		}
		m_pRst->Open((_bstr_t)sql.c_str(), (IDispatch *)m_pConn, adOpenDynamic, adLockOptimistic, adCmdText);
		if (m_pRst->rsEOF) {
			m_pRst->Close();
			return 0;
		}
		nRecordC = m_pRst->GetRecordCount();
	}
	catch (_com_error e)
	{
		TCHAR errormessage[2048];
		swprintf_s(errormessage, _T("执行SQL[%s]出错!\r\n错误信息:%s")
			, sql.c_str()
			, ParseComError(e).c_str());
		m_sErr = errormessage;
		return -1;
	}
	m_pRst->Close();
	return nRecordC;
}

TString CDBSQLServer::Vt2Str(VARIANT var)
{
	TCHAR buff[1024] = {0};
	_variant_t var_t;
	_bstr_t bst_t;
	CTime time_value;
	COleCurrency var_currency;
	switch (var.vt)
	{
	case VT_EMPTY:
		break;
	case VT_UI1:
		swprintf_s(buff, _T("%d"), var.bVal);
		break;
	case VT_I2:
		swprintf_s(buff, _T("%d"), var.iVal);
		break;
	case VT_I4:
		swprintf_s(buff, _T("%d"), var.lVal);
		break;
	case VT_R4:
		swprintf_s(buff, _T("%f"), var.fltVal);
		break;
	case  VT_R8:
		swprintf_s(buff, _T("%f"), var.dblVal);
		break;
	case VT_CY:
		var_currency = var;
		swprintf_s(buff, _T("%s"), var_currency.Format(0).GetString());
		break;
	case VT_BSTR:
		var_t = var;
		bst_t = var_t;
		swprintf_s(buff, _T("%s"), (LPCTSTR)bst_t);
		break;
	case VT_NULL:
		break;
	case VT_BOOL:
		swprintf_s(buff, _T("%d"), var.boolVal);
		break;
	default:
		break;
	}
	TString retS = buff;
	return 	chTrimRight(retS);
}

TString CDBSQLServer::ParseComError(_com_error &e)
{
	TString ErrorStr; 
	TCHAR errbuff[1024] = { 0 };
	_bstr_t bstrSource(e.Source()); 
	_bstr_t bstrDescription(e.Description()); 
	swprintf_s(errbuff, _T("\nADO Error\nCode = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s\n\n")
		, e.Error(), e.ErrorMessage(), (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription);
	ErrorStr = errbuff;
	return ErrorStr;
}
