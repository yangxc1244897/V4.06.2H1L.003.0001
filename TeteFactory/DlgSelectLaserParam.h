#pragma once
#include <vector>
#include "tlMarkDevice.h"
#include "afxwin.h"

// CDlgSelectLaserParam 对话框

class CDlgSelectLaserParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelectLaserParam)

public:
	CDlgSelectLaserParam(std::vector<IMarkPen*>& refvMarkPen, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSelectLaserParam();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LASER_SELECT };
#endif

private:
	std::vector<IMarkPen*>& m_refvMarkPen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1();
};
