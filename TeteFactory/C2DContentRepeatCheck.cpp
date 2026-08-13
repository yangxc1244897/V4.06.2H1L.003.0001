#include "stdafx.h"
#include "C2DContentRepeatCheck.h"
#include "FactoryConfig.h"

const static CString g_sDatabaseName = _T("2DBarcodeContent.db");
const static CString g_sTableName = _T("BarcodeContent");
const static CString g_sCreateTableSQL = _T("CREATE TABLE BarcodeContent( \
STRIPID CHAR(32) NOT NULL, \
NUMBER INTEGER NOT NULL, \
CONTENT CHAR(64) NOT NULL PRIMARY KEY, \
TEST_TIME NUMERIC DEFAULT (datetime('now','localtime')) \
);");


C2DContentRepeatCheck::C2DContentRepeatCheck()
{
	m_pDBInterface = CDBInstance::Instance()->CreateDBInstance(emDB_SQLITE3);
}


C2DContentRepeatCheck::~C2DContentRepeatCheck()
{
	m_pDBInterface->CloseDB();
	CDBInstance::Instance()->DeleteDBInstance(m_pDBInterface);
}

bool C2DContentRepeatCheck::InitDB()
{
	// 创建路径
	CString sDBPath = CFactoryConfig::Instance()->m_s2DPath + _T("\\");
	if (!PathIsDirectory((LPTSTR)(LPCTSTR)sDBPath))
	{
		if (!MakeSureDirectoryPathExists(CStringA(sDBPath)))
		{
			m_sErrMsg.Format(_T("数据库路径[%s]创建失败！"), sDBPath);
			return false;
		}
	}
	stDBConnectInfo_t dbinfo;

	dbinfo.db = sDBPath + g_sDatabaseName;
	if (!m_pDBInterface->ConnectDB(dbinfo))
	{
		m_sErrMsg.Format(_T("数据库连接失败！详细信息：%s"), m_pDBInterface->GetLastError());
		return false;
	}

	if (!m_pDBInterface->IsExistTable(g_sTableName.GetString()))
	{
		if (!m_pDBInterface->Executesql(g_sCreateTableSQL.GetString()))
		{
			m_sErrMsg.Format(_T("数据表[%s]创建失败，失败代码：%s"), m_pDBInterface->GetLastError());
			return false;
		}
	}

	return true;
}

CString C2DContentRepeatCheck::GetLastErrorMsg()
{
	return m_sErrMsg;
}

bool C2DContentRepeatCheck::Check(const std::vector<CString>& vs2DData)
{
	if (vs2DData.size() == 0) return true;

	switch (CFactoryConfig::Instance()->m_nCheck2DBarcodeRepeat)
	{
	case 0:
		return true;   // 不检查
		break;
	case 1:
		if (!checkSingleSame(vs2DData))
			return false;
		break;
	case 2:
		if (!checkSingleRepeat(vs2DData))
			return false;
		break;
	default:
		break;
	}

	CString sql;
	m_pDBInterface->Executesql(_T("BEGIN;"));
	for (int i = 0, iSize = vs2DData.size(); i < iSize; i++)
	{
		if (!vs2DData[i].IsEmpty())
		{
			sql.Format(_T("SELECT * FROM  BarcodeContent WHERE CONTENT = '%s'"), vs2DData[i]);

			int nRet = m_pDBInterface->GetRecordCount(sql.GetString());
			if (-1 == nRet)
			{
				m_pDBInterface->Executesql(_T("ROLLBACK;"));
				m_sErrMsg.Format(_T("数据查询失败！失败信息：%s"), m_pDBInterface->GetLastError());
				return false;
			}
			else if (nRet > 0)
			{
				m_pDBInterface->Executesql(_T("ROLLBACK;"));
				// 数据库存在数据
				DBRECORDSET m_dbRecord;
				DBFIELDS dbFields;
				dbFields.push_back(_T("STRIPID"));
				dbFields.push_back(_T("CONTENT"));
				dbFields.push_back(_T("NUMBER"));
				dbFields.push_back(_T("TEST_TIME"));
				m_dbRecord.clear();
				if (!m_pDBInterface->GetRecord(sql.GetString(), dbFields, m_dbRecord))
				{
					m_sErrMsg.Format(_T("数据查询失败！失败信息：%s"), m_pDBInterface->GetLastError());
					return false;
				}
				m_sErrMsg.Format(_T("第%d颗存在重复的二维码数据：(之前重复的数据：Strip ID:%s, 2D:%s, ID:%s, TestTime:%s)")
					, i, m_dbRecord[0][0].c_str(), m_dbRecord[0][1].c_str(), m_dbRecord[0][2].c_str(), m_dbRecord[0][3].c_str());
				return false;
			}
			
			// 因为这种模式下，所有的二维码数据都是一致，所以只需要查询一条
			if (1 == CFactoryConfig::Instance()->m_nCheck2DBarcodeRepeat)
				break;
		}

	}
	m_pDBInterface->Executesql(_T("COMMIT;"));
	return true;
}

bool C2DContentRepeatCheck::Save(const CString& sStripID, const std::vector<CString>& vs2DData)
{
	if (0 == CFactoryConfig::Instance()->m_nCheck2DBarcodeRepeat)
		return true;

	if (vs2DData.size() == 0) return true;

	CString sql;
	CString sContent;
	m_pDBInterface->Executesql(_T("BEGIN;"));
	
	//Add by hcx 20240227
	data.clear();
	//End by hcx 20240227
	
	for (int i = 0; i < vs2DData.size(); i++) 
	{
		if (!vs2DData[i].IsEmpty())
		{
			sql.Format(_T("INSERT INTO BarcodeContent(STRIPID, NUMBER, CONTENT) VALUES('%s', '%d', '%s');"), sStripID, i, vs2DData[i]);
			
			//Add by hcx 20240227
			data.push_back(sql);
			//End by hcx 20240227

			if (!m_pDBInterface->Executesql(sql.GetString()))
			{
				m_pDBInterface->Executesql(_T("ROLLBACK;"));
				m_sErrMsg = CString(_T("数据插入失败！详细信息：")) + CString(m_pDBInterface->GetLastError());
				return false;
			}
			// 因为这种模式下，所有的二维码数据都是一致，所以只需要保存一条
			if (1 == CFactoryConfig::Instance()->m_nCheck2DBarcodeRepeat)
				break;
		}
	}
	m_pDBInterface->Executesql(_T("COMMIT;"));
	return true;
}

bool C2DContentRepeatCheck::checkSingleRepeat(const std::vector<CString>& vs2DData)
{
	int iSendCount = vs2DData.size();
	for (int i = 0; i < iSendCount - 1; i++)
	{
		for (int j = i + 1; j < iSendCount; j++)
		{
			if (!vs2DData[i].IsEmpty())
			{
				if (vs2DData[i] == vs2DData[j])
				{
					m_sErrMsg.Format(_T("二维码数据存在重复[%d和%d重复]！"), i, j);
					return false;
				}
			}
		}
	}

	return true;
}

bool C2DContentRepeatCheck::checkSingleSame(const std::vector<CString>& vs2DData)
{
	int iSendCount = vs2DData.size();
	if (iSendCount == 0)
		return true;

	CString sCur;
	int iCur = 0;
	// 获取第一个不为空的2D数据
	getFirstNoNullData(vs2DData, sCur, iCur);

	for (int i = iCur + 1; i < iSendCount; i++)
	{
		if (!vs2DData[i].IsEmpty())
		{
			if (sCur != vs2DData[i])
			{
				m_sErrMsg.Format(_T("二维码数据存在不相同[%d不同]！"), i);
				return false;
			}
		}
	}

	return true;
}

bool C2DContentRepeatCheck::DeleteDataOutOfDate()
{
	m_pDBInterface->Executesql(_T("BEGIN;"));
	CString sql, s, startTime;
	CTime time = CTime::GetCurrentTime();
	time -= CTimeSpan(CFactoryConfig::Instance()->m_nSavePeriod, 0, 0, 0);
	startTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));
	sql.Format(_T("DELETE FROM  BarcodeContent WHERE TEST_TIME <= '%s'"), startTime);
	if (!m_pDBInterface->Executesql(sql.GetString()))
	{
		m_pDBInterface->Executesql(_T("ROLLBACK;"));
		m_sErrMsg.Format(_T("数据删除失败！失败信息：%s"), m_pDBInterface->GetLastError());
		return false;
	}
	m_pDBInterface->Executesql(_T("COMMIT;"));

	// 清理数据库内存。不添加这句，删除数据后，数据空间不会减少
	m_pDBInterface->Executesql(_T("VACUUM"));
	return true;
}



bool C2DContentRepeatCheck::getFirstNoNullData(const std::vector<CString>& vs2DData, CString& str, int& index)
{
	index = 0;
	// 获取第一个不为空的2D数据
	for (auto& item : vs2DData)
	{
		if (!item.IsEmpty())
		{
			str = item;
			break;
		}
		index++;
	}
	return true;
}