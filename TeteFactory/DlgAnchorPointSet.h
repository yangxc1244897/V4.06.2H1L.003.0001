#pragma once
#include "afxcmn.h"
#include <vector>
#include <map>
#include "afxwin.h"
// CDlgAnchorPointSet 对话框
const int c_nAnchorCol = 3;
const int c_nAnchorRow = 3;

struct stAnchorPoints_t
{
	double x1, y1;
	double x2, y2;
};

class CConfigure;
class IMarkPen;
class IVisionDriver;
class FormWorkControl;
class CDlgAnchorPointSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAnchorPointSet)

public:
	CDlgAnchorPointSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAnchorPointSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANCHORPOINT_SET};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	IMarkPen * m_MarkPen;

	CConfigure *m_pConfig;
	FormWorkControl* m_pWorkControl;

public:
	void InitList(CListCtrl & list);
	void SetList(CListCtrl & list, double dbX, double dbY);
	void CalcAnchorP(double dbX, double dbY, double (*X)[c_nAnchorCol], double(*Y)[c_nAnchorCol]);
	/**
	* @brief 标记9点
	* @param[int] nHead = 0 主头, 1 从头, 2 双头
	* @return 如果标记失败，返回FALSE，否则返回TRUE
	*/
	BOOL MarkAnchorP(int nHead, double dbX, double dbY, double dbR, BOOL bNine = TRUE);

	void EnableButton(BOOL b = TRUE);
	BOOL Apply();
public:
	double m_dbCenterX;
	double m_dbCenterY;
	double m_dbCenterX2;
	double m_dbCenterY2;
	double m_dbPitch;
	double m_dbFillPitch;
	double m_dbRadius;
	CListCtrl m_listNine;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAnchor();
	afx_msg void OnBnClickedAnchor2();
	afx_msg void OnBnClickedButtonPenset();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonApply();
};
