#pragma once

// CDlgMarkPoint 对话框
class IMarkPen;
class CDlgMarkPoint : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMarkPoint)

public:
	CDlgMarkPoint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMarkPoint();

    IMarkPen *m_pThePen;
    
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MARK_POINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL MarkPoint(int nHead, double x, double y);

public:
    double m_dbPosX;
    double m_dbPosY;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOptionPen();
    afx_msg void OnBnClickedLaserOn();
    afx_msg void OnBnClickedLaserOff();
	afx_msg void OnBnClickedLaserOn2();
	double m_dbPosX2;
	double m_dbPosY2;
	afx_msg void OnBnClickedOk();
};
