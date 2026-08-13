#pragma once


// CDlgMultiCalib 对话框
class IMarkEngine;
class IMarkPen;
class CDlgMultiCalib : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMultiCalib)

public:
	CDlgMultiCalib(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMultiCalib();

    IMarkPen *m_pThePen;
    void MarkGrid(IMarkEngine *pTheEngine,double dbLimitX, double dbLimitY,uint32_t nGridX,uint32_t nGridY);
    void MarkCross(IMarkEngine *pTheEngine,double dbLimitX, double dbLimitY);
	void MarkX(IMarkEngine *pTheEngine, double x, double y);
	void MarkY(IMarkEngine *pTheEngine, double x, double y);

    void LockButton(BOOL IsLock);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MULTI_CALIB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOppoTest();
    virtual BOOL OnInitDialog();
    INT m_dwMasterSafeX;
    INT m_dwMasterSafeY;
    INT m_dwSlaverSafeX;
    INT m_dwSlaverSafeY;
    double m_dbMasterOffsetX;
    double m_dbMasterOffsetY;
    double m_dbMasterRotateA;
    double m_dbSlaverOffsetX;
    double m_dbSlaverOffsetY;
    double m_dbSlaverRotateA;
    double m_dbOppoPitch;
    afx_msg void OnBnClickedMasterCalib();
    afx_msg void OnBnClickedMasterData();
    afx_msg void OnBnClickedSlaverCalib();    
    afx_msg void OnBnClickedSlaverData();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonMasterCalibTool();
	afx_msg void OnBnClickedButtonSlaverCalibTool();
};
