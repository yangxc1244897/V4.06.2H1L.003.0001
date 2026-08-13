#pragma once
#include <vector>
#include "ViewSingleBin.h"

class CViewAllBin : public CWnd
{
    // Construction
public:
    CViewAllBin();

    // Attributes
public:

    // Operations
public:
	int m_nBinCount;
	std::vector<CViewSingleBin*> m_vpViewSingleBin;
	void SetMarkCount(int nBin, int nC);
	void SetBinContent(CWaferGraphDoc * pDoc);
	void SetSelBin(int nBin);
	// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewAllBin)
public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CViewAllBin();
	
    // Generated message map functions
protected:
    //{{AFX_MSG(CViewAllBin)
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
 

private:

public:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
