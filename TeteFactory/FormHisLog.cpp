// FormHisLog.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "FormHisLog.h"
#include "afxdialogex.h"


// CFormHisLog 对话框

IMPLEMENT_DYNAMIC(CFormHisLog, CDialogEx)

CFormHisLog::CFormHisLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FORM_HISTORY_LOG, pParent)
{

}

CFormHisLog::~CFormHisLog()
{
}

void CFormHisLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFormHisLog, CDialogEx)
END_MESSAGE_MAP()


// CFormHisLog 消息处理程序
