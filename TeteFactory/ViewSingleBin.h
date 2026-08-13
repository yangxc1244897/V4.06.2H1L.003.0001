#pragma once
#include "tlGraphRender.h"
#include "..\Wafer\WaferEngine.h"

#define MAX_BIN_COUNT (32)
class CViewSingleBin : public CWnd
{
    // Construction
public:
    CViewSingleBin();

    // Attributes
public:
	CBrush m_Brush[MAX_BIN_COUNT];
	CBrush m_BrushSelFlag;
	CBrush m_BrushOKFlag;
    ID2Coord  *m_pTheCoord;
    IGraphRender *m_pTheRender;
	CWaferGraph * m_pWaferGraph;
	CWaferGraph* m_pWaferGraphDraw;
	int m_nBinIndex;
	int m_nMarkTimes;
	BOOL m_bShowSelFlag;
	BOOL m_bShowTitle;  // 是否显示标题
    // Operations
public:
	void SetShowContent(int nBin, CWaferGraph * pWafer = NULL);
	void SetMarkTimes(int n);
	void ShowSelFlag(BOOL bShow);
	void ShowTitle(BOOL bShow);
	void pix2mm(CPoint pt, DPoint_t &dp) { m_pTheCoord->pix2mm(pt.x, pt.y, dp.x, dp.y); };
	void pix2mm(POINT pt, DPoint_t &dp) { m_pTheCoord->pix2mm(pt.x, pt.y, dp.x, dp.y); };
	void mm2pix(DPoint_t dp, CPoint &pt) { m_pTheCoord->mm2pix(dp.x, dp.y, pt.x, pt.y); };
	void mm2pix(DPoint_t dp, POINT &pt) { m_pTheCoord->mm2pix(dp.x, dp.y, pt.x, pt.y); };
	void ChangeWaferDir();

	
	
	// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewSingleBin)
public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CViewSingleBin();

    // Generated message map functions
protected:
    //{{AFX_MSG(CViewSingleBin)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
 

private:

public:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
