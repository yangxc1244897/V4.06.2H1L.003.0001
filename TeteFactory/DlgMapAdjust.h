#pragma once
#include "Wafer\WaferEngine.h"
#include "MapAdjustView.h"
#include "afxwin.h"

// CDlgMapAdjust 对话框

const CString  g_sMapOffsetfile = _T(".mob");
typedef struct OFFSET_VALUE
{
	double dx;
	double dy;
	double deg;
	OFFSET_VALUE()
	{
		dx = 0;
		dy = 0;
		deg = 0;
	}
}stOffsetValue_t;

class CDlgMapAdjust : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMapAdjust)

public:
	CDlgMapAdjust(CWaferMapDoc * pMapDoc, const CString & sMarkFileName, 
		CString & sMobFilePath, bool & bAdjust, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMapAdjust();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAP_ADJUST };
#endif
	CFont m_fntBig;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


protected:
	CWaferMapDoc * m_pMapDoc;
	CWaferMapView * m_pMapView;  // 视图显示
	stOffsetValue_t *m_ppOffsetValue[10];  // 记录10组，便于撤消
	stOffsetValue_t *m_DefalutOffsetValue;   // 初始偏移值
	int m_nCurIndex;
	CString & m_sMobFilePath;
	CString m_sMarkFileName;
	bool & m_bAdjust;

	BOOL FindNewMobFile();   // 找到最新的阵列修正文件, 删除最旧的文件
	void AdjustPos(double dbOffsetX, double dbOffsetY);
	void RotateSel(double dbDeg);

	static int32_t PreviewProc(LPVOID *pParam);

	void ShowBinSetButton();
public:

	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSelall();
	afx_msg void OnBnClickedButtonUnselall();
	afx_msg void OnBnClickedButtonSelleft();
	afx_msg void OnBnClickedButtonSelright();
	afx_msg void OnBnClickedButtonAdjust();
	afx_msg void OnBnClickedButtonUndo();
	CStatic m_ViewFrame;
	afx_msg void OnBnClickedButtonPrintsel();
	afx_msg void OnBnClickedButtonUnprintsel();
	double m_dbDeg;
	double m_dbOffsetX;
	double m_dbOffsetY;
	virtual void OnOK();
	afx_msg LRESULT ShowCellInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonSelRow();
	afx_msg void OnBnClickedButtonSelCol();
	afx_msg void OnBnClickedButtonOffsetUp();
	afx_msg void OnBnClickedButtonOffsetDown();
	afx_msg void OnBnClickedButtonOffsetLeft();
	afx_msg void OnBnClickedButtonOffsetRight();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedButtonZoomOut();
	afx_msg void OnBnClickedButtonZoomIn();
	afx_msg void OnBnClickedButtonZoomAll();
	afx_msg void OnBnClickedButtonForbitCanvs();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonRotate();
	afx_msg void OnBnClickedButtonRotateDown();
	afx_msg void OnBnClickedButtonRedPreview();
	afx_msg void OnBnClickedButtonPrintAll();
	afx_msg void OnBnClickedButtonUnprintall();
	int m_iMappingDebugSet;
	afx_msg void OnBnClickedRadioMappingNone();
	afx_msg void OnBnClickedButtonSetBin1();
};
