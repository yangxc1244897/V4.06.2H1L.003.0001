#pragma once
#include "afxwin.h"
#include "FactoryConfig.h"
#include "ThirdParty\InputSignal.h"

// CDlgScannerSet 对话框

class CDlgScannerSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScannerSet)

public:
	CDlgScannerSet(class CFormWorkControl* pFormWorkControl, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgScannerSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCANNER_SET };
#endif

	CFactoryConfig* m_pFactoryConfig;
	class CFormWorkControl* m_pFormWorkControl;
	BOOL Apply();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboScanner();
	CComboBox m_comboScanner;
	afx_msg void OnBnClickedButtonStartEnd();
	afx_msg void OnBnClickedButtonCloseEnd();
	afx_msg void OnBnClickedButtonStartReady();
	afx_msg void OnBnClickedButtonCloseReady();
	afx_msg void OnBnClickedButtonStartBusy();
	afx_msg void OnBnClickedButtonCloseBusy();
	afx_msg void OnBnClickedButtonStartEnable2d();
	afx_msg void OnBnClickedButtonCloseEnable2d();
	afx_msg void OnBnClickedButtonStart2dReady();
	afx_msg void OnBnClickedButtonClose2dReady();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonStartReadOk();
	afx_msg void OnBnClickedButtonCloseReadOk();
	afx_msg void OnBnClickedButtonStartReadNg();
	afx_msg void OnBnClickedButtonCloseReadNg();
	afx_msg void OnBnClickedButtonStartInput();
	afx_msg void OnBnClickedButtonCloseInput();
	afx_msg void OnBnClickedButtonStartPwsEnable();
	afx_msg void OnBnClickedButtonClosePwsEnable();
	afx_msg void OnBnClickedButtonStartPwsOk();
	afx_msg void OnBnClickedButtonStartPwsNg();
	afx_msg void OnBnClickedButtonClosePwsFail();
	afx_msg void OnBnClickedButtonClosePwsOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CInputStatic m_InputStartSignal;
	CInputStatic m_Input2DStartSignal;
};
