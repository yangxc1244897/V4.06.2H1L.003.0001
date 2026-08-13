#pragma once
// ColorLed.h : header file
//
/*
#define LED_OFF    0
#define LED_RED    1
#define LED_GREEN  2
#define LED_BLUE   3
*/
enum {
    emLED_OFF = 0,
    emLED_RED,
    emLED_GREEN,
    emLED_YELLOW
};

/////////////////////////////////////////////////////////////////////////////
// CColorLed window

class CColorLed : public CStatic
{
// Construction
public:
	CColorLed();
    

private:
    int       m_iMode;
    CString   m_sCaption;
    COLORREF  m_clColor;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorLed)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnSetLedInfo(CString sCaption,int iMode);
	virtual ~CColorLed();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorLed)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////