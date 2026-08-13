#pragma once
#include "tlGraphRender.h"
#include "..\Wafer\WaferEngine.h"
#include "CoordTransform.h"

enum
{
	emShowContent_NULL = 0,
	emShowContent_Bin,
	emShowContent_Map,
	emShowContent_Map_Only
};

class CViewFactory : public CWnd
{
    // Construction
public:
    CViewFactory();

    // Attributes
public:

    ID2Coord  *m_pTheCoord;
    IGraphRender *m_pTheRender;
	CCoordTransform *m_pCoordTrans;
	CWaferGraph * m_pWaferGraph;
	CWaferMapDoc * m_pMapDoc;
	int m_nShowType;   // 显示类型
    // Operations
public:
	void SetShowContent(int nType, void * p = NULL);
	void pix2mm(CPoint pt, DPoint_t &dp) { m_pTheCoord->pix2mm(pt.x, pt.y, dp.x, dp.y); };
	void pix2mm(POINT pt, DPoint_t &dp) { m_pTheCoord->pix2mm(pt.x, pt.y, dp.x, dp.y); };
	void mm2pix(DPoint_t dp, CPoint &pt) { m_pTheCoord->mm2pix(dp.x, dp.y, pt.x, pt.y); };
	void mm2pix(DPoint_t dp, POINT &pt) { m_pTheCoord->mm2pix(dp.x, dp.y, pt.x, pt.y); };
	

	
	
	// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewFactory)
public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CViewFactory();

    // Generated message map functions
protected:
    //{{AFX_MSG(CViewFactory)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
 

private:

public:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
