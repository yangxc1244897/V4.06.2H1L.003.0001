#ifndef MES_DB_INTERFACE_H_
#define MES_DB_INTERFACE_H_
#include "tlString.h"

#import "msado60_Backcompat.tlb" no_namespace rename("EOF","rsEOF")

/*!DB类型 */
enum emDataBaseType_t {
	emDB_SQLSERVER = 0,   /*! SQL Server*/
	emDB_ACCESS,   /*! ACCESS Server*/
	emDB_SQLITE3,   /*! SQLITE3 Server*/
};

/*! 数据库连接参数 */
typedef struct _ST_DBCONNECT_INFO {
	TString ip;
	TString db;
	TString user;
	TString pwd;
	int port;
	_ST_DBCONNECT_INFO() {
		port = 1433;
	}
}stDBConnectInfo_t;

typedef std::map<TString, TString> DBFIELDRECORD;
typedef std::vector<TString> DBRECORD;
typedef std::vector<TString> DBFIELDS;
typedef std::vector<DBRECORD> DBRECORDSET;

class CDBInterface
{
public:
	virtual bool ConnectDB(const stDBConnectInfo_t& stDBConInfo) = 0;
	virtual void CloseDB() = 0;
	virtual bool Executesql(const TString& sql) = 0;
	virtual bool IsExistTable(const TString& sTable) = 0;
	/*! 查询记录  */
	virtual bool GetRecord(const TString& sql, const DBFIELDS& vFields, DBRECORDSET& vRecords) = 0;

	/**
	* @brief 获取满足查询语句sql的记录数
	* @brief sql SQL查询语句
	* @return 等于-1，表示查询发生错误；等于0，表示查询记录为空；大于1，表示存在记录。
	*/
	virtual int GetRecordCount(const TString& sql) = 0;


	const TCHAR * GetLastError() { return m_sErr.c_str(); }
protected:
	TString m_sErr;    //保存错误信息
};

#endif
