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
#ifndef SQLiteDBH
#define SQLiteDBH

//--#include "define.h"
#include "SQLite3Ext.h"
//---------------------------------------------------------------------------
typedef struct ReportInfo_ST
{
        AnsiString  EXT_ID;            //WebService的生成ID
        AnsiString  FOLLOWING_CARD;    //--随件卡
        AnsiString  WORK_ORDER_NUMBER;
        AnsiString  WORK_ORDER_SUB_NUMBER;
        AnsiString  WORK_SEQUENCE;
        AnsiString  ITEM_BATCH;           //批次号
        AnsiString  SERIAL_NO;            //标识号
        AnsiString  ITEM_NUMBER;
        AnsiString  ITEM_NAME;
        AnsiString  FORCAST_PRODUCT_QUALITY;
        AnsiString  PRIMARY_UOM_CODE;
        AnsiString  SCAN_DATE;
        AnsiString  CREATE_DATE;
        AnsiString  QUALIFIED_QUANTITY;
        AnsiString  SCAN_TYPE;             //暂不填
        AnsiString  SERIAL_DIGIT;
        AnsiString  NON_QUALIFIED_QUANTITY;
        AnsiString  WORKER_ID;
        //AnsiString  TRANSACTION_DATE;
        AnsiString  BUFFER1;         //TETE
}ReportERP_t;

#define  MARK_FLAG_SUCCESS        1       //--一切正常
#define  MARK_FLAG_FAILED          -1  //--未识别
#define  MARK_FLAG_ERROR           -2    //--误码，识别到码但与目标不匹配
#define  MARK_FLAG_NOSCAN          0    //--扫描枪未响应或者不扫描
struct MarkedInfo_ST{
    int iIndex;        //按钮索引，从1开始，无快捷按钮的图应设为为0
    AnsiString sGraphNum;        //--产品图号，工具栏按钮名
    AnsiString sFileName;    //--模板文件名
    //--
    AnsiString sLineNum; //--生产线,A1,B1,C1..
    AnsiString sClassNum;    //--班次,1,2
    AnsiString sMarkDate;        //--标记日期
    AnsiString sMarkTime;        //--标记时刻
    //AnsiString sTxtContent;  //--明码内容,已取消
    AnsiString sDataMatrix;  //--条码内容
    int iLength;//--模板中条码内容的总长度，数据库中不记录
    int iFlag;               //--是否合格，合格为1，未扫到为-1，扫到但错误为-2，未响应为0
};
typedef MarkedInfo_ST MarkedInfo_t;

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



//---------------------------------------------------------------------------
#endif
