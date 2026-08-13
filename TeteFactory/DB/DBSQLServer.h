#ifndef MES_DB_SALSERVER_H_
#define MES_DB_SALSERVER_H_

#include "DBInterface.h"


class CDBSQLServer : public CDBInterface  
{
public:
	CDBSQLServer();
	virtual ~CDBSQLServer();
	virtual bool ConnectDB(const stDBConnectInfo_t& stDBConInfo);
	virtual void CloseDB();
	virtual bool Executesql(const TString& sql);
	virtual bool IsExistTable(const TString& sTable);
	/*! 查询记录  */
	virtual bool GetRecord(const TString& sql, const DBFIELDS& vFields, DBRECORDSET& vRecords);

	/**
	* @brief 获取满足查询语句sql的记录数
	* @brief sql SQL查询语句
	* @return 等于-1，表示查询发生错误；等于0，表示查询记录为空；大于1，表示存在记录。
	*/
	virtual int GetRecordCount(const TString& sql);
	

protected:
	TString Vt2Str(VARIANT var);
	TString ParseComError(_com_error &e);

private:
	_ConnectionPtr m_pConn;
	_CommandPtr m_pCmd;
	_RecordsetPtr m_pRst;
};

#endif
