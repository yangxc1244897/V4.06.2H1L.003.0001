#include "stdafx.h"
#include "DBAccess.h"
#include <assert.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDBAccess::CDBAccess()
{	
	m_pConn.CreateInstance(__uuidof(Connection));
	m_pRst.CreateInstance(__uuidof(Recordset));
	m_pCmd.CreateInstance(__uuidof(Command));
}

CDBAccess::~CDBAccess()
{
	CloseDB();
}

/*
ConnectModeEnum
常量	值	描述
adModeUnknown	0	默认值。权限尚未设置或不能确定权限。
adModeRead	1	只读权限。
adModeWrite	2	只写权限。
adModeReadWrite	3	读/写权限。
adModeShareDenyRead	4	禁止其他人以读权限打开连接。
adModeShareDenyWrite	8	禁止其他人以写权限打开连接。
adModeShareExclusive	12	禁止其他人打开连接。
adModeShareDenyNone	16	允许其他人以任何权限打开连接。不拒绝其他人的读或写访问。
adModeRecursive	0x400000	与 adModeShareDenyNone, adModeShareDenyWrite 或 adModeShareDenyRead 一起使用，对当前 Record 的所有子记录设置权限。


adOpenForwardOnly （默认值）打开仅向前类型游标。
adOpenKeyset 打开键集类型游标。
adOpenDynamic 打开动态类型游标。
adOpenStatic 打开静态类型游标。

LockType 可选。确定提供者打开 Recordset 时应该使用的锁定（并发）类型的 LockTypeEnum 值，可为下列常量之一（参见 LockType 属性可获得详细信息）。
常量 说明
adLockReadOnly （默认值）只读 — 不能改变数据。
adLockPessimistic 保守式锁定（逐个） — 提供者完成确保成功编辑记录所需的工作，通常通过在编辑时立即锁定数据源的记录来完成。
adLockOptimistic 开放式锁定（逐个） — 提供者使用开放式锁定，只在调用 Update 方法时才锁定记录。
adLockBatchOptimistic 开放式批更新 — 用于批更新模式（与立即更新模式相对）。

Options 可选，长整型值，用于指示提供者如何计算 Source 参数（如果它代表的不是 Command 对象），或从以前保存 Recordset 的文件中恢复 Recordset。可为下列常量之一（参见 CommandType 属性可获得该列表中前五个常量的详细说明）。

常量 说明
adCmdText 指示提供者应该将 Source 作为命令的文本定义来计算。
adCmdTable 指示 ADO 生成 SQL 查询以便从 Source 命名的表返回所有行。
adCmdTableDirect 指示提供者更改从 Source 命名的表返回的所有行。
adCmdStoredProc 指示提供者应该将 Source 视为存储的过程。
adCmdUnknown 指示 Source 参数中的命令类型为未知。
adCommandFile 指示应从 Source 命名的文件中恢复持久（保存的）Recordset。
adExecuteAsync 指示应异步执行 Source。
adFetchAsync 指示在提取 CacheSize 属性中指定的初始数量后，应该异步提取所有剩余的行。
*/
bool CDBAccess::ConnectDB(const stDBConnectInfo_t & stDBConInfo)
{
	try
	{
		if (m_pConn->GetState() != adStateClosed)
			m_pConn->Close();

		TCHAR connectString[200];
		swprintf_s(connectString, _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"), stDBConInfo.db.c_str());
		m_pConn->Open(connectString, _T(""), _T(""), adModeShareDenyNone); 
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

void CDBAccess::CloseDB()
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

bool CDBAccess::Executesql(const TString & sql)
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

bool CDBAccess::IsExistTable(const TString & sTable)
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

bool CDBAccess::GetRecord(const TString & sql, const DBFIELDS& vFields, DBRECORDSET& vRecords)
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

int CDBAccess::GetRecordCount(const TString & sql)
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

TString CDBAccess::Vt2Str(VARIANT var)
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

TString CDBAccess::ParseComError(_com_error &e)
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
