/**
  ******************************************************************************
  * @文件
  * @作者
  * @版本 V1.00.00
  * @日期 2017-04-17
  * @简述
  ******************************************************************************
  * @说明
  *
  *
  *
  ******************************************************************************
  */
#pragma hdrstop

#include "stdafx.h"
#include "SQLiteDB.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CSQLiteDB::CSQLiteDB()
{
    //--m_sApplicationPath = ExtractFilePath(Application->ExeName);
    m_sDBFileName = GetCurrentDir() + "\\Databases.db";

    m_IsDBOpened = false;
    //if(!FileExists(m_sConfigureFileName))
    //m_SQLite3DB.open(m_sDBFileName.c_str());
}
//------------------------------------------------------------
CSQLiteDB::~CSQLiteDB()
{
    //--m_SQLite3DB.close();
}
//------------------------------------------------------------
void CSQLiteDB::ImportFromFile(int iFileType,AnsiString sFileName)
{}
//------------------------------------------------------------
void CSQLiteDB::ExportIntoFile(int iFileType,AnsiString sFileName)
{}
//------------------------------------------------------------
bool CSQLiteDB::execSQL(AnsiString sSQLCmd)
{
    m_SQLite3DB.execDML(sSQLCmd.c_str());
    
    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::openDB(AnsiString sFileName)
{
    if(sFileName.IsEmpty())
    {
        m_SQLite3DB.open(m_sDBFileName.c_str());
    }
    else
    {
        m_sDBFileName = sFileName;
        m_SQLite3DB.open(sFileName.c_str());
    }

    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        m_SQLite3DB.execDML("CREATE TABLE [MarkedData]([GraphCode] char(255),[LineNum] char(25),[ClassNum] char(25),\
            [MarkDate] DATE,[MarkTime] TIME,[DataMatrix] char(255),[Flag] int, [FileName] char(255));");
    }

    m_IsDBOpened = true;
    
    return true;
}
bool CSQLiteDB::createMarkDataTable(AnsiString sTableName)
{
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        m_SQLite3DB.execDML("CREATE TABLE [MarkedData]([GraphCode] char(255),[LineNum] char(25),[ClassNum] char(25),\
            [MarkDate] DATE,[MarkTime] TIME,[DataMatrix] char(255),[Flag] int, [FileName] char(255));");
    }
    
    return true;
}
//------------------------------------------------------------
/*
---- Database Setion -------->>
--1:GraphNum
--2:LineNum
--3:ClassNum
--4:MarkedDate
--5:MarkedTime
--6:DataMatrix
--7:Flag,1=OK
--8:FileName
---- Database Setion --------<<
*/
bool CSQLiteDB::writeDB(MarkedInfo_t *pMarkedInfo)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    
    AnsiString sSqlCmd;

    sSqlCmd.sprintf("INSERT INTO [MarkedData] (GraphCode,LineNum,ClassNum,MarkDate,MarkTime,DataMatrix,Flag,FileName)\
                    VALUES ('%s','%s','%s','%s','%s','%s','%d','%s');",
                    pMarkedInfo->sGraphNum,
                    pMarkedInfo->sLineNum,
                    pMarkedInfo->sClassNum,
                    pMarkedInfo->sMarkDate,
                    pMarkedInfo->sMarkTime,
                    pMarkedInfo->sDataMatrix,
                    pMarkedInfo->iFlag,
                    pMarkedInfo->sFileName);
    //--int nRows = m_SQLite3DB.execDML(sSqlCmd.c_str() );
    m_SQLite3DB.execDML(sSqlCmd.c_str() );

    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::modifyDB(MarkedInfo_t *pMarkedInfo)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    AnsiString sSqlCmd;
    sSqlCmd.sprintf("UPDATE [MarkedData] SET Flag = '%d' WHERE DataMatrix='%s' AND FileName='%s';",
                    pMarkedInfo->iFlag,
                    pMarkedInfo->sDataMatrix,
                    pMarkedInfo->sFileName);
    //--int nRows = m_SQLite3DB.execDML(sSqlCmd.c_str() );
    m_SQLite3DB.execDML(sSqlCmd.c_str() );

    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::queryDBByDate(AnsiString sGraphNum,AnsiString sStartDate,AnsiString sEndDate)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        return false;
    }
    //----
    AnsiString sSqlCmd;
    sSqlCmd.sprintf("select * from MarkedData where (GraphCode='%s') AND (MarkDate between '%s' and '%s');",sGraphNum,sStartDate,sEndDate);
    m_SQLite3Query = m_SQLite3DB.execQuery(sSqlCmd.c_str());
    
    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::queryDBByTime(AnsiString sStartTime,AnsiString sEndTime)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        return false;
    }
    //----
    AnsiString sSqlCmd;
    sSqlCmd.sprintf("select * from MarkedData where MarkTime between '%s' and '%s';",sStartTime,sEndTime);
    m_SQLite3Query = m_SQLite3DB.execQuery(sSqlCmd.c_str());
    
    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::queryDBByDateTime(AnsiString sStartDate,AnsiString sEndDate,AnsiString sStartTime,AnsiString sEndTime)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        return false;
    }
    //----
    AnsiString sSqlCmd;
    sSqlCmd.sprintf("select * from MarkedData where ( MarkTime >= '%s' and MarkTime<='%s') \
                      AND ( MarkDate>='%s' and MarkDate<='%s');",sStartTime,sEndTime,sStartDate,sEndDate);
    m_SQLite3Query = m_SQLite3DB.execQuery(sSqlCmd.c_str());
    
    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::queryDBByDate(AnsiString sStartDate,AnsiString sEndDate,TStrings *pStrList)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        return false;
    }
    //----
    AnsiString sSqlCmd;

    sSqlCmd.sprintf("select * from MarkedData where MarkDate between '%s' and '%s';",sStartDate,sEndDate);
    CppSQLite3Query q = m_SQLite3DB.execQuery(sSqlCmd.c_str());

    pStrList->Clear();
    //pStrList->Sorted = false;
    while (!q.eof())
    {
        sSqlCmd.sprintf("(%s,%s,%s,%s,%s) " ,q.fieldValue(0),q.fieldValue(1),q.fieldValue(2),q.fieldValue(3),q.fieldValue(4));
        pStrList->Append(sSqlCmd) ;
        q.nextRow();
    }
    
    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::queryDBByTime(AnsiString sStartTime,AnsiString sEndTime,TStrings *pStrList)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        return false;
    }
    //----
    AnsiString sSqlCmd;

    //--CppSQLite3Query q = db.execQuery("select * from contacts order by 1;");
    sSqlCmd.sprintf("select * from MarkedData where MarkTime between '%s' and '%s';",sStartTime,sEndTime);
    CppSQLite3Query q = m_SQLite3DB.execQuery(sSqlCmd.c_str());

    pStrList->Clear();
    //pStrList->Sorted = false;
    while (!q.eof())
    {
        sSqlCmd.sprintf("(%s,%s,%s,%s,%s) " ,q.fieldValue(0),q.fieldValue(1),q.fieldValue(2),q.fieldValue(3),q.fieldValue(4));
        pStrList->Append(sSqlCmd) ;
        q.nextRow();
    }
    
    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::queryDBByDateTime(AnsiString sStartDate,AnsiString sEndDate,AnsiString sStartTime,AnsiString sEndTime,TStrings *pStrList)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        return false;
    }
    //----
    AnsiString sSqlCmd;

    //--CppSQLite3Query q = db.execQuery("select * from contacts order by 1;");
    //--sDebugInfo.sprintf("select * from MarkedData where MarkDate between '%s' and '%s' AND MarkTime between '%s' and '%s';",sStartDate,sEndDate,sStartTime,sEndTime);
    sSqlCmd.sprintf("select * from MarkedData where ( MarkTime >= '%s' and MarkTime<='%s') \
                      AND ( MarkDate>='%s' and MarkDate<='%s');",sStartTime,sEndTime,sStartDate,sEndDate);
    CppSQLite3Query q = m_SQLite3DB.execQuery(sSqlCmd.c_str());

    pStrList->Clear();
    //pStrList->Sorted = false;
    while (!q.eof())
    {
        sSqlCmd.sprintf("(%s,%s,%s,%s,%s) " ,q.fieldValue(0),q.fieldValue(1),q.fieldValue(2),q.fieldValue(3),q.fieldValue(4));
        pStrList->Append(sSqlCmd) ;
        q.nextRow();
    }
    
    return true;
}
//------------------------------------------------------------
bool CSQLiteDB::SearchCodeFromDB(AnsiString sSearchData)
{
    if(!m_IsDBOpened)
        openDB(m_sDBFileName);
    if(!m_SQLite3DB.tableExists("MarkedData"))
    {
        return false;
    }
    //----
    AnsiString sSqlCmd;

    sSqlCmd.sprintf("select * from MarkedData where DataMatrix= '%s';",sSearchData);
    m_SQLite3Query = m_SQLite3DB.execQuery(sSqlCmd.c_str());

    //pStrList->Clear();
    //pStrList->Sorted = false;
    AnsiString sValue = m_SQLite3Query.fieldValue(5);
    if (sValue.IsEmpty())
    {
        return false;
    }
    
    return true;
}
//------------------------------------------------------------
