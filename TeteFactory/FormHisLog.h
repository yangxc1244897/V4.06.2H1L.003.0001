#pragma once


// CFormHisLog 对话框

class CFormHisLog : public CDialogEx
{
	DECLARE_DYNAMIC(CFormHisLog)

public:
	CFormHisLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFormHisLog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM_HISTORY_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
