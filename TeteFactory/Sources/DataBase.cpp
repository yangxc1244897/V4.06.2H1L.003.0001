#include "stdafx.h"
#include "DataBase.h"
#include "Common\\CharConversion.h"



CDataBase::CDataBase()
{

}

CDataBase::~CDataBase()
{
	Close();
}

/**
* @功能 连接数据库
* @参数[in] sFileName SQLite数据库文件，如果文件不存在，则创建该文件；
* 如果文件名为空，则在内存中创建临时数据库
*/
bool CDataBase::Connect(const CString& sFileName)
{
	try
	{
		m_SQLite3DB.open(fcT2U(sFileName));
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = fcA2U(e.errorMessage());
		return false;
	}
	return true;
}

/**
* @功能 关闭数据库
*/
bool CDataBase::Close()
{
	try
	{
		m_SQLite3DB.close();
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = fcA2U(e.errorMessage());
		return false;
	}
	return true;
}

/**
* @功能 执行一条SQL语句
* @备注 执行DML数据库操作语言
*/
bool CDataBase::ExcuteSQL(const CString& sql)
{
	try
	{
		
		m_SQLite3DB.execDML(fcT2U(sql));
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = fcA2U(e.errorMessage());
		return false;
	}
	return true;
}

/**
* @功能 查询记录
* @参数[in] sql 查询语句
* @参数[out] vmRecord 返回所有的记录集
*/
bool CDataBase::GetRecord(const CString& sql, DBRECORDSET& vmRecord)
{
	try
	{
		CppSQLite3Query query =  m_SQLite3DB.execQuery(fcT2U(sql));
		while (!query.eof())
		{
			DBRECORD mRecord;
			for (int j = 0; j < query.numFields(); j++)
			{
				//对于汉字的处理，需要将UTF-8格式的字符转成ASCII格式的字符
				mRecord[fcU2T(query.fieldName(j))] = fcU2T(query.fieldValue(j));
			}
			vmRecord.push_back(mRecord);
			query.nextRow();
		}
	}
	catch (CppSQLite3Exception e)
	{
		m_sErr = fcA2U(e.errorMessage());
		return false;
	}
	return true;
}


/**
* @功能 获取最近一次的错误信息
*/
CString CDataBase::GetLastError()
{
	return m_sErr;
}

/**
* @功能 是否存在某个表格
*/
bool CDataBase::IsExistTable(const CString& sTable)
{
	return m_SQLite3DB.tableExists(fcT2A(sTable));
}