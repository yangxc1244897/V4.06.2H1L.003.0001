#include "stdafx.h"
#include "DBInstance.h"
#include "DBSQLServer.h"
#include "DBAccess.h"
#include "DBSQLLite.h"

static CDBInstance * g_Instance = nullptr;

CDBInstance* CDBInstance::Instance()
{
	if (nullptr == g_Instance) {
		g_Instance = new CDBInstance();
	}
	return g_Instance;
}

CDBInterface * CDBInstance::CreateDBInstance(const emDataBaseType_t& emDBType)
{
	CDBInterface* pInterface = nullptr;
	switch (emDBType) {
	case emDB_SQLSERVER:
		pInterface = new CDBSQLServer();
		break;
	case emDB_ACCESS:
		pInterface = new CDBAccess();
		break;
	case emDB_SQLITE3:
		pInterface = new CDBSQLLite();
		break;
	}
	return pInterface;
}

void CDBInstance::DeleteDBInstance(CDBInterface * p)
{
	if (nullptr != p) {
		delete p;
	}
}