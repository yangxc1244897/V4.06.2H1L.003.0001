
// TeteFactoryDlg.h : 头文件
#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "FormHisLog.h"
#include "DlgFactorySet.h"
#include "FormWorkControl.h"
#include "MyTabCtrl.h"

#include"MyNSocketServer.h"
#define COM_TIME_OUT  60
const int STEPLEN = 1024 * 4;
const CString g_sWindowText = _T("深圳泰德激光IC激光标记系统 V4.06.2H1L.NJHT.0087");
const CString g_sUpdateTime = _T("2026-08-06");
#define MARKSTATUS_IDLE _T("空闲状态")
#define MARKSTATUS_LOADJOB _T("作业已加载")
#define MARKSTATUS_MARK _T("正在标记")
// CTeteFactoryDlg 对话框
class CTeteFactoryDlg : public CDialogEx
{
// 构造
public:
	CTeteFactoryDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CTeteFactoryDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TETEFACTORY_DIALOG };
#endif
	CFormWorkControl * m_pWorkControl;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	BOOL m_bInit;  
	CRect m_rcClient;

	// RichEdit 字体管理
	CHARFORMAT m_cfNor;	     // 正常字体
	CHARFORMAT m_cfTitle;    // 标题
	CHARFORMAT m_cfWarn;	 // 警告
	CHARFORMAT m_cfError;	 // 错误
	CHARFORMAT m_cfPass;	 // 通过

	CBrush m_BkBrush;   // 背景刷
	CFont m_StatusFont;
	CFont m_bigFont;
	CBrush m_EditBkBrush;  // 编辑框背景色

	CDlgFactorySet * m_pSet;
	CFormHisLog * m_pHisLog;   // 暂时不需要实现

	UINT m_uGetLastInputInfo;
public:
	void AdjustControl();   // 调整控件位置
	void InitRichFont();   // 初始化Rich字体
	void SetRichEditText(const CString& csText, CHARFORMAT& newcf, BOOL bChangecf = TRUE);
	void CreateChildWnd();
	void SwitchChildWnd();
	void ClearMessage();
	void SetStaticText(UINT uCtlID, const CString & s);
	void PrintMessage(emFactoryMsgType_t nType, const CString & s);  // 打印消息
	void PrintAndLogMessage(emFactoryMsgType_t nType, const CString & s);  //显示并打印消息
	
	void UpdateSet();
	BOOL ContectTest(const char * ip, unsigned short uport);
	BOOL StartServer(unsigned short uport);
	BOOL StartServerPLC(unsigned short uport);
	BOOL StartServerVisionCheck(unsigned short uport);
	void UpdateAppCaption();
public:
	CRichEditCtrl m_RichEdit;

	DECLARE_MESSAGE_MAP()
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnOK();
public:
	CMyTabCtrl m_tabMessage;
	CMyTabCtrl m_tabMainFrame;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSelchangingTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonHistorylog();
	afx_msg void OnBnClickedButtonLaserUsage();
	//lrf 修复弹出式菜单阻塞打标情况（下3个）
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedButtonIp();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonRotateCenter();
};
