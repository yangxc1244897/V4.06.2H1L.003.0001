#include "stdafx.h"
#include "DBSQLLite.h"
#include <assert.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDBSQLLite::CDBSQLLite()
{	
}

CDBSQLLite::~CDBSQLLite()
{
	CloseDB();
}

bool CDBSQLLite::ConnectDB(const stDBConnectInfo_t & stDBConInfo)
{
	try
	{
		CloseDB();
		m_SQLite3DB.open(chT2U(stDBConInfo.db).c_str());
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = chA2T(e.errorMessage());
		return false;
	}
	return true;
}

void CDBSQLLite::CloseDB()
{
	try
	{
		m_SQLite3DB.close();
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = chA2T(e.errorMessage());
	}
}

bool CDBSQLLite::Executesql(const TString & sql)
{
	try
	{
		m_SQLite3DB.execDML(chT2U(sql).c_str());
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = chA2T(e.errorMessage());
		return false;
	}
	return true;
}

bool CDBSQLLite::IsExistTable(const TString & sTable)
{
	return m_SQLite3DB.tableExists(chT2A(sTable).c_str());
}

bool CDBSQLLite::GetRecord(const TString & sql, const DBFIELDS& vFields, DBRECORDSET& vRecords)
{
	try
	{
		CppSQLite3Query query = m_SQLite3DB.execQuery(chT2U(sql).c_str());
		while (!query.eof())
		{
			DBFIELDRECORD mFieldRecord;
			for (int j = 0; j < query.numFields(); j++)
			{
				//对于汉字的处理，需要将UTF-8格式的字符转成ASCII格式的字符
				if (nullptr != query.fieldValue(j))
					mFieldRecord[chU2T(query.fieldName(j))] = chU2T(query.fieldValue(j));
			}
			DBRECORD mRecord;
			for (auto& item : vFields)
			{
				mRecord.push_back(mFieldRecord[item]);
			}
			vRecords.push_back(mRecord);
			
			query.nextRow();
		}
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = chA2T(e.errorMessage());
		return false;
	}
	return true;
}

int CDBSQLLite::GetRecordCount(const TString & sql)
{
	int nCount = -1;
	try
	{
		CppSQLite3Table query = m_SQLite3DB.getTable(chT2U(sql).c_str());
		nCount = query.numRows();
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = chA2T(e.errorMessage());
	}

	return nCount;
}

TString CDBSQLLite::Vt2Str(VARIANT var)
{
	TCHAR buff[1024] = {0};
	_variant_t var_t;
	_bstr_t bst_t;
	COleDateTime var_date;
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
	case VT_DATE:
		var_date = var;
		swprintf_s(buff, _T("%s"), var_date.Format(_T("%F %T")));
		break;
	default:
		break;
	}
	TString retS = buff;
	return 	chTrimRight(retS);
}

TString CDBSQLLite::ParseComError(_com_error &e)
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
