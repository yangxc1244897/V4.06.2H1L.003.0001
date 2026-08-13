/*******************************************************************
*  Copyright(c) 2018 泰德激光
*  All rights reserved.
*
*  文件名称: DataBase.h
*  简要描述: 封装数据库的操作
*
*  创建日期: 2018-6-28
*  作者: hhhuang
*  说明:
******************************************************************/

#pragma once

#include "ThirdParty/SQLite/SQLite3Ext.h"
#include <vector>
#include <map>
#include "Common\\CharConversion.h"



///////////////////////////////////////////////////////////////////////
///数据库类
typedef std::map<CString, CString> DBRECORD;
typedef std::vector<DBRECORD> DBRECORDSET;
class CDataBase
{
public:
	CDataBase();
	~CDataBase();

	bool Connect(const CString& sFileName);
	bool Close();
	bool ExcuteSQL(const CString& sql);
	bool IsExistTable(const CString& sTable);
	CString GetLastError();

	//查询函数集，后续优化机制
	bool GetRecord(const CString& sql, DBRECORDSET& vmRecord);

private:
	CSQLite3DB m_SQLite3DB;
	CString m_sErr;    //保存错误信息

protected:

};

