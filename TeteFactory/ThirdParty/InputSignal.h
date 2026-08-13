#pragma once


/////////////////////////////////////////////////////////////////////////////
//

class CInputStatic : public CStatic
{
// Construction
public:
	CInputStatic();
    

private:
    COLORREF  m_clColor;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetStatus(unsigned short status);
	virtual ~CInputStatic();
	enum { SIGNAL, NON_SIGNAL, ALARM};
	

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorLed)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	
	virtual void PreSubclassWindow();
};

/////////////////////////////////////////////////////////////////////////////