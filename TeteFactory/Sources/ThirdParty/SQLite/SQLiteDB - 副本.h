/*******************************************************************
*  Copyright(c) 2018 泰德激光
*  All rights reserved.
*
*  文件名称: SQLiteDB.h
*  简要描述: 封装SQLite3Ext的操作
*
*  创建日期: 2018-6-28
*  作者: hhhuang
*  说明:
******************************************************************/

#pragma once

#include "SQLite3Ext.h"
//---------------------------------------------------------------------------

class CSQLiteDB
{
public:
    bool m_IsDBOpened;
    CSQLite3DB m_SQLite3DB;
    CppSQLite3Query m_SQLite3Query;
    MarkedInfo_t m_stMarkedInfo;
public:
    CSQLiteDB();
    ~CSQLiteDB();
    void ImportFromFile(int iFileType,AnsiString sFileName); //
    void ExportIntoFile(int iFileType,AnsiString sFileName); //
    bool execSQL(AnsiString sSQLCmd);
    bool openDB(AnsiString sFileName);
    bool createMarkDataTable(AnsiString sTableName);
    bool writeDB(MarkedInfo_t *pMarkedInfo);
    bool modifyDB(MarkedInfo_t *pMarkedInfo);
    bool queryDBByDate(AnsiString sGraphNum,AnsiString sStartDate,AnsiString sEndDate);
    bool queryDBByTime(AnsiString sStartTime,AnsiString sEndTime);
    bool queryDBByDateTime(AnsiString sStartDate,AnsiString sEndDate,AnsiString sStartTime,AnsiString sEndTime);
    bool queryDBByDate(AnsiString sStartDate,AnsiString sEndDate,TStrings *pStrList);
    bool queryDBByTime(AnsiString sStartTime,AnsiString sEndTime,TStrings *pStrList);
    bool queryDBByDateTime(AnsiString sStartDate,AnsiString sEndDate,AnsiString sStartTime,AnsiString sEndTime,TStrings *pStrList);
    bool SearchCodeFromDB(AnsiString sSearchData);
protected:
private:
    AnsiString m_sApplicationPath;
    AnsiString m_sDBFileName;
};

