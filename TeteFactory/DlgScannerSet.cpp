// DlgScannerSet.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgScannerSet.h"
#include "afxdialogex.h"
#include "MarkPlatform.h"
#include "FactoryConfig.h"
#include "FormWorkControl.h"

// CDlgScannerSet 对话框

IMPLEMENT_DYNAMIC(CDlgScannerSet, CDialogEx)

CDlgScannerSet::CDlgScannerSet(class CFormWorkControl* pFormWorkControl, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SCANNER_SET, pParent)
	, m_pFormWorkControl(pFormWorkControl)
{
	m_pFactoryConfig = CFactoryConfig::Instance();
}

CDlgScannerSet::~CDlgScannerSet()
{
}

void CDlgScannerSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SCANNER, m_comboScanner);
	DDX_Control(pDX, IDC_STATIC_START_SIGNAL, m_InputStartSignal);
	DDX_Control(pDX, IDC_STATIC_2D_START_SIGNAL, m_Input2DStartSignal);
}


BEGIN_MESSAGE_MAP(CDlgScannerSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgScannerSet::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_SCANNER, &CDlgScannerSet::OnCbnSelchangeComboScanner)
	ON_BN_CLICKED(IDC_BUTTON_START_END, &CDlgScannerSet::OnBnClickedButtonStartEnd)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_END, &CDlgScannerSet::OnBnClickedButtonCloseEnd)
	ON_BN_CLICKED(IDC_BUTTON_START_READY, &CDlgScannerSet::OnBnClickedButtonStartReady)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_READY, &CDlgScannerSet::OnBnClickedButtonCloseReady)
	ON_BN_CLICKED(IDC_BUTTON_START_BUSY, &CDlgScannerSet::OnBnClickedButtonStartBusy)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_BUSY, &CDlgScannerSet::OnBnClickedButtonCloseBusy)
	ON_BN_CLICKED(IDC_BUTTON_START_ENABLE2D, &CDlgScannerSet::OnBnClickedButtonStartEnable2d)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_ENABLE2D, &CDlgScannerSet::OnBnClickedButtonCloseEnable2d)
	ON_BN_CLICKED(IDC_BUTTON_START_2D_READY, &CDlgScannerSet::OnBnClickedButtonStart2dReady)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_2D_READY, &CDlgScannerSet::OnBnClickedButtonClose2dReady)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgScannerSet::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_START_READ_OK, &CDlgScannerSet::OnBnClickedButtonStartReadOk)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_READ_OK, &CDlgScannerSet::OnBnClickedButtonCloseReadOk)
	ON_BN_CLICKED(IDC_BUTTON_START_READ_NG, &CDlgScannerSet::OnBnClickedButtonStartReadNg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_READ_NG, &CDlgScannerSet::OnBnClickedButtonCloseReadNg)
	ON_BN_CLICKED(IDC_BUTTON_START_INPUT, &CDlgScannerSet::OnBnClickedButtonStartInput)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_INPUT, &CDlgScannerSet::OnBnClickedButtonCloseInput)
	ON_BN_CLICKED(IDC_BUTTON_START_PWS_ENABLE, &CDlgScannerSet::OnBnClickedButtonStartPwsEnable)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_PWS_ENABLE, &CDlgScannerSet::OnBnClickedButtonClosePwsEnable)
	ON_BN_CLICKED(IDC_BUTTON_START_PWS_OK, &CDlgScannerSet::OnBnClickedButtonStartPwsOk)
	ON_BN_CLICKED(IDC_BUTTON_START_PWS_NG, &CDlgScannerSet::OnBnClickedButtonStartPwsNg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_PWS_FAIL, &CDlgScannerSet::OnBnClickedButtonClosePwsFail)

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgScannerSet 消息处理程序


void CDlgScannerSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!Apply())
		return;

	CDialogEx::OnOK();
}


BOOL CDlgScannerSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comboScanner.AddString(_T("不启用扫描枪"));
	m_comboScanner.AddString(_T("网口扫描枪1"));
	m_comboScanner.AddString(_T("网口扫描枪2"));
	m_comboScanner.AddString(_T("启用两个网口扫描枪"));
	m_comboScanner.SetCurSel(m_pFactoryConfig->m_emUseScan);
	OnCbnSelchangeComboScanner();
	//扫描枪
	SetDlgItemText(IDC_EDIT_IP_1, m_pFactoryConfig->m_sScannerIP[0]);
	SetDlgItemInt(IDC_EDIT_PORT_1, m_pFactoryConfig->m_nScannerPort[0]);
	SetDlgItemText(IDC_EDIT_IP_2, m_pFactoryConfig->m_sScannerIP[1]);
	SetDlgItemInt(IDC_EDIT_PORT_2, m_pFactoryConfig->m_nScannerPort[1]);
	SetDlgItemInt(IDC_EDIT_ENABLE_2D, m_pFactoryConfig->m_nScanUse2DPort);
	SetDlgItemInt(IDC_EDIT_READY_2D, m_pFactoryConfig->m_nScannReadySingle);
	SetDlgItemInt(IDC_EDIT_SCAN_OK, m_pFactoryConfig->m_nScannOKSignal);
	SetDlgItemInt(IDC_EDIT_SCAN_NG, m_pFactoryConfig->m_nScannNGSignal);
	SetDlgItemInt(IDC_EDIT_2D_TRIGGER, m_pFactoryConfig->m_nScannStartSignal[0]);
	SetDlgItemInt(IDC_EDIT_2D_MANUAL_INPUT_TIMEOUT, m_pFactoryConfig->m_nScanInputTimeOutPort);
	SetDlgItemInt(IDC_EDIT_2D_MANUAL_INPUT_TIMEOUT, m_pFactoryConfig->m_nScanInputTimeOutPort);
	SetDlgItemInt(IDC_EDIT_ENABLE_PWS, m_pFactoryConfig->m_nScanEnablePWS);
	SetDlgItemInt(IDC_EDIT_PWS_OK, m_pFactoryConfig->m_nScanPWSOK);
	SetDlgItemInt(IDC_EDIT_PWS_NG, m_pFactoryConfig->m_nScanPWSNG);
	//PLC
	CString str;
	str.Format(_T("%d"), m_pFactoryConfig->m_nReadyPort);
	SetDlgItemText(IDC_EDIT_PLC_READY, str);
	str.Format(_T("%d"), m_pFactoryConfig->m_nStartPort);
	SetDlgItemText(IDC_EDIT_PLC_START, str);
	str.Format(_T("%d"), m_pFactoryConfig->m_nBusyPort);
	SetDlgItemText(IDC_EDIT_PLC_BUSY, str);
	str.Format(_T("%d"), m_pFactoryConfig->m_nEndPort);
	SetDlgItemText(IDC_EDIT_PLC_END, str);
	//网络IO
	SetDlgItemText(IDC_EDIT_COMMIO_IP, m_pFactoryConfig->m_nCommIP);
	str.Format(_T("%d"), m_pFactoryConfig->m_nCommPort);
	SetDlgItemText(IDC_EDIT_COMMIO_PORT, str);

	SetTimer(1, 200, nullptr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgScannerSet::OnCbnSelchangeComboScanner()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_comboScanner.GetCurSel();
	if (-1 == iSel)
		return;

	if (emScannerType_None == iSel) {
		GetDlgItem(IDC_EDIT_IP_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IP_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT_2)->EnableWindow(FALSE);
	}
	else if (emScannerType_Client1 == iSel) {
		GetDlgItem(IDC_EDIT_IP_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IP_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT_2)->EnableWindow(FALSE);
	}
	else if (emScannerType_Client2 == iSel) {
		GetDlgItem(IDC_EDIT_IP_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IP_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT_2)->EnableWindow(TRUE);
	}
	else if (emScannerType_Client_All) {
		GetDlgItem(IDC_EDIT_IP_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IP_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT_2)->EnableWindow(TRUE);
	}
}


void CDlgScannerSet::OnBnClickedButtonStartEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	IMarkDriver* pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	pMarkDriver->WriteExtPortPin(CFactoryConfig::Instance()->m_nEndPort, IO_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonCloseEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	IMarkDriver* pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	pMarkDriver->WriteExtPortPin(CFactoryConfig::Instance()->m_nEndPort, IO_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStartReady()
{
	// TODO: 在此添加控件通知处理程序代码
	IMarkDriver* pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	pMarkDriver->WriteExtPortPin(CFactoryConfig::Instance()->m_nReadyPort, IO_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonCloseReady()
{
	// TODO: 在此添加控件通知处理程序代码
	IMarkDriver* pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	pMarkDriver->WriteExtPortPin(CFactoryConfig::Instance()->m_nReadyPort, IO_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStartBusy()
{
	// TODO: 在此添加控件通知处理程序代码
	IMarkDriver* pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	pMarkDriver->WriteExtPortPin(CFactoryConfig::Instance()->m_nBusyPort, IO_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonCloseBusy()
{
	// TODO: 在此添加控件通知处理程序代码
	IMarkDriver* pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
	pMarkDriver->WriteExtPortPin(CFactoryConfig::Instance()->m_nBusyPort, IO_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStartEnable2d()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanUse2DPort, IO_2D_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonCloseEnable2d()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanUse2DPort, IO_2D_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStart2dReady()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScannReadySingle, IO_2D_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonClose2dReady()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScannReadySingle, IO_2D_OUT_INVALID);
}

BOOL CDlgScannerSet::Apply()
{
	int iSel = m_comboScanner.GetCurSel();
	if (-1 == iSel) {
		AfxMessageBox(_T("没有选中扫描枪类型！"));
		return FALSE;
	}
	bool bCheckScanner1 = false, bCheckScanner2 = false;
	if (emScannerType_Client1 == iSel)
		bCheckScanner1 = true;
	else if (emScannerType_Client2 == iSel)
		bCheckScanner2 = true;
	else if (emScannerType_Client_All == iSel)
		bCheckScanner1 = bCheckScanner2;

	CString s;
	int port;
	if (bCheckScanner1) {
		GetDlgItemText(IDC_EDIT_IP_1, s);
		if (s.IsEmpty()) {
			AfxMessageBox(_T("1号扫描枪IP为空！"));
			return FALSE;
		}
		port = GetDlgItemInt(IDC_EDIT_PORT_1);
		if (port <= 0) {
			AfxMessageBox(_T("1号扫描枪Port小于等于0！"));
			return FALSE;
		}
	}
	if (bCheckScanner2) {
		GetDlgItemText(IDC_EDIT_IP_2, s);
		if (s.IsEmpty()) {
			AfxMessageBox(_T("2号扫描枪IP为空！"));
			return FALSE;
		}
		port = GetDlgItemInt(IDC_EDIT_PORT_2);
		if (port <= 0) {
			AfxMessageBox(_T("2号扫描枪Port小于等于0！"));
			return FALSE;
		}
	}

	m_pFactoryConfig->m_emUseScan = (emScannerType_t)iSel;
	GetDlgItemText(IDC_EDIT_IP_1, m_pFactoryConfig->m_sScannerIP[0]);
	m_pFactoryConfig->m_nScannerPort[0] = GetDlgItemInt(IDC_EDIT_PORT_1);
	GetDlgItemText(IDC_EDIT_IP_2, m_pFactoryConfig->m_sScannerIP[1]);
	m_pFactoryConfig->m_nScannerPort[1] = GetDlgItemInt(IDC_EDIT_PORT_2);

	m_pFactoryConfig->m_nScanUse2DPort = GetDlgItemInt(IDC_EDIT_ENABLE_2D);
	m_pFactoryConfig->m_nScannReadySingle = GetDlgItemInt(IDC_EDIT_READY_2D);
	m_pFactoryConfig->m_nScannOKSignal = GetDlgItemInt(IDC_EDIT_SCAN_OK);
	m_pFactoryConfig->m_nScannNGSignal = GetDlgItemInt(IDC_EDIT_SCAN_NG);
	m_pFactoryConfig->m_nScannStartSignal[0] = GetDlgItemInt(IDC_EDIT_2D_TRIGGER);
	m_pFactoryConfig->m_nScanInputTimeOutPort = GetDlgItemInt(IDC_EDIT_2D_MANUAL_INPUT_TIMEOUT);
	m_pFactoryConfig->m_nScanEnablePWS = GetDlgItemInt(IDC_EDIT_ENABLE_PWS);
	m_pFactoryConfig->m_nScanPWSOK = GetDlgItemInt(IDC_EDIT_PWS_OK);
	m_pFactoryConfig->m_nScanPWSNG = GetDlgItemInt(IDC_EDIT_PWS_NG);


	//PLC
	CString tmp = _T("");
	GetDlgItemText(IDC_EDIT_PLC_READY, tmp);
	if (tmp.IsEmpty()) {
		AfxMessageBox(_T("PLC设置：Ready为空！"));
		return FALSE;
	}
	m_pFactoryConfig->m_nReadyPort = _ttoi(tmp);

	GetDlgItemText(IDC_EDIT_PLC_START, tmp);
	if (tmp.IsEmpty()) {
		AfxMessageBox(_T("PLC设置：Start为空！"));
		return FALSE;
	}
	m_pFactoryConfig->m_nStartPort = _ttoi(tmp);

	GetDlgItemText(IDC_EDIT_PLC_BUSY, tmp);
	if (tmp.IsEmpty()) {
		AfxMessageBox(_T("PLC设置：Busy为空！"));
		return FALSE;
	}
	m_pFactoryConfig->m_nBusyPort = _ttoi(tmp);

	GetDlgItemText(IDC_EDIT_PLC_END, tmp);
	if (tmp.IsEmpty()) {
		AfxMessageBox(_T("PLC设置：End为空！"));
		return FALSE;
	}
	m_pFactoryConfig->m_nEndPort = _ttoi(tmp);

	//网络IO
	GetDlgItemText(IDC_EDIT_COMMIO_IP, tmp);
	if (tmp.IsEmpty()) {
		AfxMessageBox(_T("网络IO口设置：IP为空！"));
		return FALSE;
	}
	m_pFactoryConfig->m_nCommIP = tmp;

	GetDlgItemText(IDC_EDIT_COMMIO_PORT, tmp);
	if (tmp.IsEmpty()) {
		AfxMessageBox(_T("网络IO口设置：端口为空！"));
		return FALSE;
	}
	m_pFactoryConfig->m_nCommPort = _ttoi(tmp);
	return TRUE;
}

void CDlgScannerSet::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	Apply();
}


void CDlgScannerSet::OnBnClickedButtonStartReadOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScannOKSignal, IO_2D_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonCloseReadOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScannOKSignal, IO_2D_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStartReadNg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScannNGSignal, IO_2D_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonCloseReadNg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScannNGSignal, IO_2D_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStartInput()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanInputTimeOutPort, IO_2D_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonCloseInput()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanInputTimeOutPort, IO_2D_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStartPwsEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanEnablePWS, IO_2D_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonClosePwsEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanEnablePWS, IO_2D_OUT_INVALID);
}


void CDlgScannerSet::OnBnClickedButtonStartPwsOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanPWSOK, IO_2D_OUT_VALID);
}

void CDlgScannerSet::OnBnClickedButtonClosePwsOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanPWSOK, IO_2D_OUT_INVALID);
}

void CDlgScannerSet::OnBnClickedButtonStartPwsNg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanPWSNG, IO_2D_OUT_VALID);
}


void CDlgScannerSet::OnBnClickedButtonClosePwsFail()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pFormWorkControl->SetCommIOStatus(CFactoryConfig::Instance()->m_nScanPWSNG, IO_2D_OUT_INVALID);
}





void CDlgScannerSet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
	{
		IMarkDriver* pMarkDriver = CMarkPlatform::Instance()->MarkDriver();
		uint32_t nBit = 0;
		pMarkDriver->ReadExtPortPin(m_pFactoryConfig->m_nStartPort + 16, nBit);
		m_InputStartSignal.SetStatus((IO_IN_VALID == nBit) ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
		
		m_pFormWorkControl->GetCommIOStatus(m_pFactoryConfig->m_nScannStartSignal[0], nBit);
		m_Input2DStartSignal.SetStatus((IO_2D_IN_VALID == nBit) ? CInputStatic::SIGNAL : CInputStatic::NON_SIGNAL);
	}

		break;

	}

	CDialogEx::OnTimer(nIDEvent);
}
