// DlgMapAdjust.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgMapAdjust.h"
#include "afxdialogex.h"
#include "tlString.h"
#include "FactoryConfig.h"
#include "MarkPlatform.h"
#include "MarkThread.h"
#include "Wafer\WaferEngine.h"
#include "FormWorkControl.h"


// CDlgMapAdjust 对话框

IMPLEMENT_DYNAMIC(CDlgMapAdjust, CDialogEx)

CDlgMapAdjust::CDlgMapAdjust(CWaferMapDoc * pMapDoc, const CString & sMarkFileName, 
	CString & sMobFilePath, bool & bAdjust, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAP_ADJUST, pParent)
	, m_dbDeg(0)
	, m_dbOffsetX(0)
	, m_dbOffsetY(0)
	, m_sMobFilePath(sMobFilePath)
	, m_sMarkFileName(sMarkFileName)
	, m_bAdjust(bAdjust)
{
	m_pMapDoc = pMapDoc;
	m_nCurIndex = -1;
	m_fntBig.CreatePointFont(110, _T("宋体"));
	m_iMappingDebugSet = IniFileRead(_T("Global"), _T("m_iMappingDebugSet"), 1, _T(".\\config\\MapAdjust.ini"));
}

CDlgMapAdjust::~CDlgMapAdjust()
{
	m_fntBig.DeleteObject();
}

void CDlgMapAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW_FRAME, m_ViewFrame);
	DDX_Text(pDX, IDC_EDIT_DEG, m_dbDeg);
	DDX_Text(pDX, IDC_EDIT_OFFSETX, m_dbOffsetX);
	DDX_Text(pDX, IDC_EDIT_OFFSETY, m_dbOffsetY);
	DDX_Radio(pDX, IDC_RADIO_MAPPING_NONE, m_iMappingDebugSet);
}


BEGIN_MESSAGE_MAP(CDlgMapAdjust, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SELALL, &CDlgMapAdjust::OnBnClickedButtonSelall)
	ON_BN_CLICKED(IDC_BUTTON_UNSELALL, &CDlgMapAdjust::OnBnClickedButtonUnselall)
	ON_BN_CLICKED(IDC_BUTTON_SELLEFT, &CDlgMapAdjust::OnBnClickedButtonSelleft)
	ON_BN_CLICKED(IDC_BUTTON_SELRIGHT, &CDlgMapAdjust::OnBnClickedButtonSelright)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST, &CDlgMapAdjust::OnBnClickedButtonAdjust)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CDlgMapAdjust::OnBnClickedButtonUndo)
	ON_BN_CLICKED(IDC_BUTTON_PRINTSEL, &CDlgMapAdjust::OnBnClickedButtonPrintsel)
	ON_BN_CLICKED(IDC_BUTTON_UNPRINTSEL, &CDlgMapAdjust::OnBnClickedButtonUnprintsel)
	ON_MESSAGE(WM_TETE_MAPSET_CELLINFO, &CDlgMapAdjust::ShowCellInfo)
	ON_BN_CLICKED(IDC_BUTTON_SEL_ROW, &CDlgMapAdjust::OnBnClickedButtonSelRow)
	ON_BN_CLICKED(IDC_BUTTON_SEL_COL, &CDlgMapAdjust::OnBnClickedButtonSelCol)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_UP, &CDlgMapAdjust::OnBnClickedButtonOffsetUp)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_DOWN, &CDlgMapAdjust::OnBnClickedButtonOffsetDown)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_LEFT, &CDlgMapAdjust::OnBnClickedButtonOffsetLeft)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_RIGHT, &CDlgMapAdjust::OnBnClickedButtonOffsetRight)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CDlgMapAdjust::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CDlgMapAdjust::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CDlgMapAdjust::OnBnClickedButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CDlgMapAdjust::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_ALL, &CDlgMapAdjust::OnBnClickedButtonZoomAll)
	ON_BN_CLICKED(IDC_BUTTON_FORBIT_CANVAS, &CDlgMapAdjust::OnBnClickedButtonForbitCanvs)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, &CDlgMapAdjust::OnBnClickedButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_DOWN, &CDlgMapAdjust::OnBnClickedButtonRotateDown)
	ON_BN_CLICKED(IDC_BUTTON_RED_PREVIEW, &CDlgMapAdjust::OnBnClickedButtonRedPreview)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_ALL, &CDlgMapAdjust::OnBnClickedButtonPrintAll)
	ON_BN_CLICKED(IDC_BUTTON_UNPRINTALL, &CDlgMapAdjust::OnBnClickedButtonUnprintall)

	ON_BN_CLICKED(IDC_RADIO_MAPPING_NONE, &CDlgMapAdjust::OnBnClickedRadioMappingNone)
	ON_BN_CLICKED(IDC_RADIO_MAPPING_COMPARE_NG, &CDlgMapAdjust::OnBnClickedRadioMappingNone)
	ON_BN_CLICKED(IDC_RADIO_MAPPING_PRINT_BIN1, &CDlgMapAdjust::OnBnClickedRadioMappingNone)
	ON_BN_CLICKED(IDC_RADIO_MAPPING_PRINT_ANYBIN, &CDlgMapAdjust::OnBnClickedRadioMappingNone)
	ON_BN_CLICKED(IDC_BUTTON_SET_BIN1, &CDlgMapAdjust::OnBnClickedButtonSetBin1)
	ON_BN_CLICKED(IDC_BUTTON_SET_BIN2, &CDlgMapAdjust::OnBnClickedButtonSetBin1)
	ON_BN_CLICKED(IDC_BUTTON_SET_BIN3, &CDlgMapAdjust::OnBnClickedButtonSetBin1)
	ON_BN_CLICKED(IDC_BUTTON_SET_BIN4, &CDlgMapAdjust::OnBnClickedButtonSetBin1)
	ON_BN_CLICKED(IDC_BUTTON_SET_BIN5, &CDlgMapAdjust::OnBnClickedButtonSetBin1)
	ON_BN_CLICKED(IDC_BUTTON_SET_BIN6, &CDlgMapAdjust::OnBnClickedButtonSetBin1)
END_MESSAGE_MAP()


// CDlgMapAdjust 消息处理程序


void CDlgMapAdjust::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString sBtnText;
	GetDlgItemText(IDC_BUTTON_RED_PREVIEW, sBtnText);
	if (_T("停止红光预览") == sBtnText)
	{
		OnBnClickedButtonRedPreview();
	}

	if (m_nCurIndex >= 0 && m_nCurIndex < 10)
	{
		// 保存修正文件
		// 客户要求保存最近的三个阵列位置文件，
		// 因此如果已经存在三个文件，删除最早的文件，然后再另外保存，并将文件名更新
		FindNewMobFile();

		CStdioFile file;
		if (!file.Open(m_sMobFilePath, CFile::modeWrite | CFile::modeCreate))
		{
			AfxMessageBox(_T("阵列修正文件保存失败!"));
			return;
		}

		// 自定义格式：index,x,y,a
		CString s;
		// 写入汇总后的偏移值
		size_t nWaferSize = m_pMapDoc->GetGroup(0)->vWaferMetas.size();
		for (int i = 0; i < nWaferSize; i++)
		{
			s.Format(_T("%d,%.4f,%.4f,%.4f\n"), i, m_DefalutOffsetValue[i].dx + m_ppOffsetValue[m_nCurIndex][i].dx
				, m_DefalutOffsetValue[i].dy + m_ppOffsetValue[m_nCurIndex][i].dy
				, m_DefalutOffsetValue[i].deg + m_ppOffsetValue[m_nCurIndex][i].deg);
			file.WriteString(s);
		}
		file.Close();
	}
	
	delete[] m_DefalutOffsetValue;
	for (int i = 0; i < 10; i++)
	{
		delete[] m_ppOffsetValue[i];
	}

	// 取消所有cell的选中状态
	CWaferGroup * pGroup = m_pMapDoc->GetGroup(0);
	if (nullptr == pGroup)
		return;
	int nWaferSize = pGroup->vWaferMetas.size();
	for (int i = 0; i < nWaferSize; i++)
	{
		pGroup->vWaferMetas[i].EnableSel(false);
	}

	// 保存手动设置的Mapping信息
	if (/*1 == CFactoryConfig::Instance()->m_iMappingDebugSet
		|| */3 == CFactoryConfig::Instance()->m_iMappingDebugSet)
	{
		for (int i = 0; i < nWaferSize; i++)
		{
			CFactoryConfig::Instance()->m_iManualBinMap.push_back(pGroup->vWaferMetas[i].GetWaferID());
		}
	}

	CDialogEx::OnClose();
}


BOOL CDlgMapAdjust::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ViewFrame.ModifyStyle(WS_BORDER, 0);
	CRect rcMapView;
	m_ViewFrame.GetClientRect(&rcMapView);
	m_pMapView = new CWaferMapView;
	m_pMapView->Create(WS_VISIBLE | WS_CHILD, rcMapView, &m_ViewFrame, ID_WAFER_MAP_VIEW, NULL);
	m_pMapView->OnChangeDoc(m_pMapDoc);

	SetDlgItemText(IDC_EDIT_CS_OFFSET, _T("0"));
	SetDlgItemText(IDC_EDIT_DEG, _T("0"));
	SetDlgItemInt(IDC_EDIT_SEL_ROW_START, 1);
	SetDlgItemInt(IDC_EDIT_SEL_ROW_END, 1);
	SetDlgItemInt(IDC_EDIT_SEL_COL_START, 1);
	SetDlgItemInt(IDC_EDIT_SEL_COL_END, 1);
	GetDlgItem(IDC_BUTTON_ROTATE)->SetFont(&m_fntBig);
	GetDlgItem(IDC_BUTTON_ROTATE_DOWN)->SetFont(&m_fntBig);
	// 分配内存
	int nCellCount = m_pMapDoc->GetGroup(0)->vWaferMetas.size();
	for (int i = 0; i < 10; i++)
	{
		m_ppOffsetValue[i] = new stOffsetValue_t[nCellCount]();
	}
	m_DefalutOffsetValue = new stOffsetValue_t[nCellCount]();
	// 读取默认偏移数据
	CStdioFile file;
	if (!file.Open(m_sMobFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
	{
		AfxMessageBox(_T("阵列位置修正文件打开失败!"));
		return FALSE;
	}

	// 自定义格式：index,x,y,a
	CString s;
	if (0 == file.GetLength())
	{
		// 写入默认值
		for (int i = 0; i < nCellCount; i++)
		{
			s.Format(_T("%d,%.4f,%.4f,%.4f\n"), i, m_DefalutOffsetValue[i].dx, m_DefalutOffsetValue[i].dy, m_DefalutOffsetValue[i].deg);
			file.WriteString(s);
		}
		file.Close();
		return TRUE;
	}

	while (file.ReadString(s))
	{
		int index;
		std::vector<TString> vs;
		chParseString(s.GetString(), vs, _T(','));
		if (vs.size() != 4)
			continue;
		index = _ttoi(vs[0].c_str());
		if (index < nCellCount && index >= 0)
		{
			m_DefalutOffsetValue[index].dx = _ttof(vs[1].c_str());
			m_DefalutOffsetValue[index].dy = _ttof(vs[2].c_str());
			m_DefalutOffsetValue[index].deg = _ttof(vs[3].c_str());
		}
	}
	file.Close();

	// MAPPING模式下，禁止设置打印状态
	if (CFactoryConfig::Instance()->m_bCheckStripMap
		&& !CFactoryConfig::Instance()->m_bEnableDummy)
	{	
		GetDlgItem(IDC_BUTTON_PRINTSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNPRINTSEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRINT_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNPRINTALL)->EnableWindow(FALSE);
	}

	// Dummy模式下，开放Mapping调试设置
	GetDlgItem(IDC_STATIC_MAPPING)->ShowWindow(CFactoryConfig::Instance()->m_bEnableDummy);
	GetDlgItem(IDC_RADIO_MAPPING_NONE)->ShowWindow(CFactoryConfig::Instance()->m_bEnableDummy);
	GetDlgItem(IDC_RADIO_MAPPING_COMPARE_NG)->ShowWindow(CFactoryConfig::Instance()->m_bEnableDummy);
	GetDlgItem(IDC_RADIO_MAPPING_PRINT_BIN1)->ShowWindow(CFactoryConfig::Instance()->m_bEnableDummy);
	GetDlgItem(IDC_RADIO_MAPPING_PRINT_ANYBIN)->ShowWindow(CFactoryConfig::Instance()->m_bEnableDummy);

	m_iMappingDebugSet = IniFileRead(_T("Global"), _T("m_iMappingDebugSet"), 0, _T(".\\config\\MapAdjust.ini"));  
	CFactoryConfig::Instance()->m_iMappingDebugSet = m_iMappingDebugSet;

	CFactoryConfig::Instance()->m_iManualBinMap.clear();

	// Add by hhhuang 20241023
	// Dummy模式下，所有的单颗，需要被设置为不打印状态
	if (CFactoryConfig::Instance()->m_bEnableDummy)
	{
		m_pMapView->SetAllPrint(false);
	}
	// END: Add by hhhuang 20241023



	ShowBinSetButton();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgMapAdjust::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && VK_ESCAPE == pMsg->wParam)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgMapAdjust::OnBnClickedButtonSelall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	m_pMapView->doSelectAll();
}


void CDlgMapAdjust::OnBnClickedButtonUnselall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	m_pMapView->UnSelectAllObjects();
	m_pMapView->Invalidate();
}


void CDlgMapAdjust::OnBnClickedButtonSelleft()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	m_pMapView->SelectLeftHead();
	m_pMapView->Invalidate();
}


void CDlgMapAdjust::OnBnClickedButtonSelright()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	m_pMapView->SelectRightHead();
	m_pMapView->Invalidate();
}

void CDlgMapAdjust::OnBnClickedButtonAdjust()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapDoc)
		return;
	CWaferGroup * pGroup = m_pMapDoc->GetGroup(0);
	if (nullptr == pGroup)
		return;
	if (m_pMapDoc->GetMapObjSelCount() == 0)
		return;

	//UpdateData(TRUE);
	double dbOffsetX = 0, dbOffsetY = 0, dbDeg = 0;
	CString s;
	GetDlgItemText(IDC_EDIT_OFFSETX, s);
	dbOffsetX = _ttof(s);
	GetDlgItemText(IDC_EDIT_OFFSETY, s);
	dbOffsetY = _ttof(s);
	if (D_EQUAL(dbOffsetX, 0) && D_EQUAL(dbOffsetY, 0) && D_EQUAL(dbDeg, 0))
		return;

	int nPreIndex = (m_nCurIndex == -1) ? 0 : m_nCurIndex;
	m_nCurIndex++;
	if (10 == m_nCurIndex)
		m_nCurIndex = 0;
	size_t nWaferSize = pGroup->vWaferMetas.size();
	for (int i = 0; i < nWaferSize; i++)
	{
		if (pGroup->vWaferMetas[i].IsSel())
		{
			m_ppOffsetValue[m_nCurIndex][i].dx = m_ppOffsetValue[nPreIndex][i].dx + dbOffsetX;
			m_ppOffsetValue[m_nCurIndex][i].dy = m_ppOffsetValue[nPreIndex][i].dy + dbOffsetY;
			m_ppOffsetValue[m_nCurIndex][i].deg = m_ppOffsetValue[nPreIndex][i].deg + dbDeg;
		}
		else
		{
			m_ppOffsetValue[m_nCurIndex][i].dx = m_ppOffsetValue[nPreIndex][i].dx;
			m_ppOffsetValue[m_nCurIndex][i].dy = m_ppOffsetValue[nPreIndex][i].dy;
			m_ppOffsetValue[m_nCurIndex][i].deg = m_ppOffsetValue[nPreIndex][i].deg;
		}
	}

	// 修改阵列
	for (int i = 0; i < nWaferSize; i++)
	{
		if (pGroup->vWaferMetas[i].IsSel())
		{
			if (!(D_EQUAL(dbOffsetX, 0)
				&& D_EQUAL(dbOffsetY, 0)))
				pGroup->vWaferMetas[i].xDim.Repos(dbOffsetX, dbOffsetY);
			if (!D_EQUAL(dbDeg, 0))
				pGroup->vWaferMetas[i].xDim.Rotate(dbDeg);
		}
		pGroup->vWaferMetas[i].EnableSel(false);
	}
	m_pMapView->Invalidate();

	dbOffsetX = 0;
	dbOffsetY = 0;
	dbDeg = 0;
	SetDlgItemText(IDC_EDIT_OFFSETX, _T("0"));
	SetDlgItemText(IDC_EDIT_OFFSETY, _T("0"));
	m_bAdjust = true;  // 阵列以及被修改过
	//UpdateData(FALSE);
}

void CDlgMapAdjust::OnBnClickedButtonUndo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (-1 == m_nCurIndex)
		return;

	if (nullptr == m_pMapDoc)
		return;
	CWaferGroup * pGroup = m_pMapDoc->GetGroup(0);
	if (nullptr == pGroup)
		return;

	int nPreIndex = (m_nCurIndex == 0) ? 9 : m_nCurIndex-1;
	size_t nWaferSize = pGroup->vWaferMetas.size();
	stOffsetValue_t *stOffsetValue = new stOffsetValue_t[nWaferSize]();
	for (int i = 0; i < nWaferSize; i++)
	{
			stOffsetValue[i].dx = m_ppOffsetValue[nPreIndex][i].dx - m_ppOffsetValue[m_nCurIndex][i].dx;
			stOffsetValue[i].dy = m_ppOffsetValue[nPreIndex][i].dy - m_ppOffsetValue[m_nCurIndex][i].dy;
			stOffsetValue[i].deg = m_ppOffsetValue[nPreIndex][i].deg - m_ppOffsetValue[m_nCurIndex][i].deg;
	}
	// 清空当前偏移记录
	for (int i = 0; i < nWaferSize; i++)
	{
		m_ppOffsetValue[m_nCurIndex][i].dx = 0;
		m_ppOffsetValue[m_nCurIndex][i].dy = 0;
		m_ppOffsetValue[m_nCurIndex][i].deg= 0;
	}
	m_nCurIndex = nPreIndex;

	// 修改阵列
	for (int i = 0; i < nWaferSize; i++)
	{
		if (!(D_EQUAL(stOffsetValue[i].dx, 0)
			&& D_EQUAL(stOffsetValue[i].dy, 0)))
			pGroup->vWaferMetas[i].xDim.Repos(stOffsetValue[i].dx, stOffsetValue[i].dy);
		if (!D_EQUAL(stOffsetValue[i].deg, 0))
			pGroup->vWaferMetas[i].xDim.Rotate(stOffsetValue[i].deg);
	}
	m_pMapView->Invalidate();

	delete[] stOffsetValue;
}

void CDlgMapAdjust::OnBnClickedButtonPrintsel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	// Add by hhhuang 20260323
	// Dummy模式下，技术员不能设置超过2颗的产品，进行打印
	if (CFactoryConfig::Instance()->m_bEnableDummy) {
		if (USERMNG->GetCurUser() >= USER_A_TECH)
		{
			int iMark = m_pMapView->GetObjMarkCount();
			int iSel = m_pMapView->GetObjSelCount();
			if ((iSel + iMark) > 2) {
				AfxMessageBox(_T("技术员权限选择不能超过2颗产品！"));
				return;
			}
		}
	}
	// END: Add by hhhuang 20260323



	m_bAdjust = true;  // 阵列以及被修改过
	m_pMapView->SetSelPrint(true);
	m_pMapView->Invalidate();
}


void CDlgMapAdjust::OnBnClickedButtonUnprintsel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	m_bAdjust = true;  // 阵列以及被修改过
	m_pMapView->SetSelPrint(false);
	m_pMapView->Invalidate();
}


void CDlgMapAdjust::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
}

LRESULT CDlgMapAdjust::ShowCellInfo(WPARAM wParam, LPARAM lParam)
{
	if (nullptr == m_pMapDoc)
		return 0;
	int iGroup = wParam;
	int index = lParam;
	CWaferGroup* pWaferGroup = m_pMapDoc->GetGroup(iGroup);
	CString s;
	double x, y, a;
	if (-1 != m_nCurIndex)
	{
		x = m_DefalutOffsetValue[index].dx + m_ppOffsetValue[m_nCurIndex][index].dx;
		y = m_DefalutOffsetValue[index].dy + m_ppOffsetValue[m_nCurIndex][index].dy;
		a = m_DefalutOffsetValue[index].deg + m_ppOffsetValue[m_nCurIndex][index].deg;
	}
	else
	{
		x = m_DefalutOffsetValue[index].dx;
		y = m_DefalutOffsetValue[index].dy;
		a = m_DefalutOffsetValue[index].deg;
	}
	s.Format(_T("行：%d，列：%d，索引：%d，偏移X=%.4f，偏移Y=%.4f，角度A=%.4f")
		, pWaferGroup->GetRowCount() - pWaferGroup->vWaferMetas[index].nRow + 1 
		, pWaferGroup->vWaferMetas[index].nCol
		,index, x, y, a);
	SetDlgItemText(IDC_STATIC_DETAIL_INFO, s);
		
	return 0;
}

BOOL CDlgMapAdjust::FindNewMobFile()
{
	// 在指定文件夹中，找到最旧的阵列修正文件，
	// 阵列修正文件格式：指定路径 + 打印图档名 + "_" + 日期（年月日时分秒 + 修正文件后缀（.mob）
	WIN32_FIND_DATA filedata;
	CString sFileFilter, sOldFile;
	FILETIME filetime;
	int nFileCount = 0;  // 文件数量
	sFileFilter.Format(_T("%s\\%s*.mob"), CFactoryConfig::Instance()->m_sMobFilePath, m_sMarkFileName);
	HANDLE hFile = FindFirstFile(sFileFilter, &filedata);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		// 未找到相关文件，分配一个初始文件名，但是不创建文件，等到后面打开的时候再创建
		goto TestError;
	}

	nFileCount++;
	filetime = filedata.ftLastWriteTime;
	sOldFile = filedata.cFileName;
	while (FindNextFile(hFile, &filedata))
	{
		nFileCount++;
		if (1 == CompareFileTime(&filetime, &filedata.ftLastWriteTime))
		{
			// filetime 比 filedata.ftLastWriteTime晚
			filetime = filedata.ftLastWriteTime;
			sOldFile = filedata.cFileName;
		}
	}
	FindClose(hFile);

	if (!sOldFile.IsEmpty() && nFileCount >= 3)
	{
		sOldFile.Format(_T("%s\\%s"), CFactoryConfig::Instance()->m_sMobFilePath, sOldFile);
		DeleteFile(sOldFile);
	}

TestError:
	CTime currentTime = CTime::GetCurrentTime();
	CString timeYmdHMS = currentTime.Format(_T("_%Y_%m_%d_%H%M%S"));
	m_sMobFilePath = CFactoryConfig::Instance()->m_sMobFilePath +
		_T("\\") + m_sMarkFileName + timeYmdHMS + g_sMapOffsetfile;
	return TRUE;
}

void CDlgMapAdjust::OnBnClickedButtonSelRow()
{
	// TODO: 在此添加控件通知处理程序代码
	int iStart = GetDlgItemInt(IDC_EDIT_SEL_ROW_START);
	int iEnd = GetDlgItemInt(IDC_EDIT_SEL_ROW_END);
	if (0 < iStart && 0 < iEnd) {
		if (nullptr == m_pMapView)
			return;
		if (iStart > iEnd)  iEnd = iStart;    // 单独选中某一行，可以只需要输入起始位置即可，
		m_pMapView->SelectRow(iStart, iEnd);
		m_pMapView->Invalidate();
	}
	else {
		AfxMessageBox(_T("参数错误！"));
	}
}


void CDlgMapAdjust::OnBnClickedButtonSelCol()
{
	// TODO: 在此添加控件通知处理程序代码
	int iStart = GetDlgItemInt(IDC_EDIT_SEL_COL_START);
	int iEnd = GetDlgItemInt(IDC_EDIT_SEL_COL_END);
	if (0 < iStart && 0 < iEnd) {
		if (nullptr == m_pMapView)
			return;
		if (iStart > iEnd)  iEnd = iStart;    // 单独选中某一行，可以只需要输入起始位置即可，
		m_pMapView->SelectCol(iStart, iEnd);
		m_pMapView->Invalidate();
	}
	else {
		AfxMessageBox(_T("参数错误！"));
	}
}
void CDlgMapAdjust::OnBnClickedButtonOffsetUp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItemText(IDC_EDIT_CS_OFFSET, s);
	double dbO = _ttof(s);
	AdjustPos(0, dbO);

	// 偏移值清空，防止连续作用
	SetDlgItemText(IDC_EDIT_CS_OFFSET, _T("0"));

}


void CDlgMapAdjust::OnBnClickedButtonOffsetDown()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItemText(IDC_EDIT_CS_OFFSET, s);
	double dbO = _ttof(s);
	AdjustPos(0, -dbO);

	// 偏移值清空，防止连续作用
	SetDlgItemText(IDC_EDIT_CS_OFFSET, _T("0"));
}


void CDlgMapAdjust::OnBnClickedButtonOffsetLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItemText(IDC_EDIT_CS_OFFSET, s);
	double dbO = _ttof(s);
	AdjustPos(-dbO, 0);

	// 偏移值清空，防止连续作用
	SetDlgItemText(IDC_EDIT_CS_OFFSET, _T("0"));
}


void CDlgMapAdjust::OnBnClickedButtonOffsetRight()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItemText(IDC_EDIT_CS_OFFSET, s);
	double dbO = _ttof(s);
	AdjustPos(dbO, 0);

	// 偏移值清空，防止连续作用
	SetDlgItemText(IDC_EDIT_CS_OFFSET, _T("0"));
}

void CDlgMapAdjust::AdjustPos(double dbOffsetX, double dbOffsetY)
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapDoc)
		return;
	if (m_pMapDoc->GetGroupCount() <= 0)
		return;
	if (m_pMapDoc->GetMapObjSelCount() == 0)
		return;

	if (D_EQUAL(dbOffsetX, 0) && D_EQUAL(dbOffsetY, 0))
		return;

	int nPreIndex = (m_nCurIndex == -1) ? 0 : m_nCurIndex;
	m_nCurIndex++;
	if (10 == m_nCurIndex)
		m_nCurIndex = 0;

	CWaferGroup * pGroup = m_pMapDoc->GetGroup(0);
	if (nullptr == pGroup)
		return;

	size_t nWaferSize = pGroup->vWaferMetas.size();
	for (int j = 0; j < nWaferSize; j++)
	{
		if (pGroup->vWaferMetas[j].IsSel())
		{
			m_ppOffsetValue[m_nCurIndex][j].dx = m_ppOffsetValue[nPreIndex][j].dx + dbOffsetX;
			m_ppOffsetValue[m_nCurIndex][j].dy = m_ppOffsetValue[nPreIndex][j].dy + dbOffsetY;
			m_ppOffsetValue[m_nCurIndex][j].deg = m_ppOffsetValue[nPreIndex][j].deg + 0;
		}
		else
		{
			m_ppOffsetValue[m_nCurIndex][j].dx = m_ppOffsetValue[nPreIndex][j].dx;
			m_ppOffsetValue[m_nCurIndex][j].dy = m_ppOffsetValue[nPreIndex][j].dy;
			m_ppOffsetValue[m_nCurIndex][j].deg = m_ppOffsetValue[nPreIndex][j].deg;
		}
	}

	// 修改阵列
	for (int j = 0; j < nWaferSize; j++)
	{
		if (pGroup->vWaferMetas[j].IsSel())
		{
			if (!(D_EQUAL(dbOffsetX, 0)
				&& D_EQUAL(dbOffsetY, 0)))
				pGroup->vWaferMetas[j].xDim.Repos(dbOffsetX, dbOffsetY);
		}
		pGroup->vWaferMetas[j].EnableSel(false);
	}
	
	m_pMapView->Invalidate();

	m_bAdjust = true;  // 阵列以及被修改过
}

void CDlgMapAdjust::RotateSel(double dbDeg)
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapDoc)
		return;
	if (m_pMapDoc->GetGroupCount() <= 0)
		return;
	if (m_pMapDoc->GetMapObjSelCount() == 0)
		return;

	if (D_EQUAL(dbDeg, 0))
		return;

	int nPreIndex = (m_nCurIndex == -1) ? 0 : m_nCurIndex;
	m_nCurIndex++;
	if (10 == m_nCurIndex)
		m_nCurIndex = 0;

	CWaferGroup * pGroup = m_pMapDoc->GetGroup(0);
	if (nullptr == pGroup)
		return;

	size_t nWaferSize = pGroup->vWaferMetas.size();
	// 记录选中对象的中心点的初始值
	vector<DPoint_t> vOldPointCenter;
	for (int j = 0; j < nWaferSize; j++)
	{
		if (pGroup->vWaferMetas[j].IsSel())
		{
			vOldPointCenter.push_back(DPoint_t(pGroup->vWaferMetas[j].xDim.GetCenterX(), pGroup->vWaferMetas[j].xDim.GetCenterY()));
		}
	}

	// 整体旋转
	FField_t fSel;
	fSel = m_pMapDoc->GetSelField();
	double cx = fSel.CenterX();
	double cy = fSel.CenterY();
	m_pMapDoc->RotateSel(cx, cy, dbDeg);  // 对象旋转后，其中心点也会发生偏移

	// 记录偏移值
	int iSelIndex = 0;
	for (int j = 0; j < nWaferSize; j++)
	{
		if (pGroup->vWaferMetas[j].IsSel())
		{
			double dbOffsetX = 0, dbOffsetY = 0;
			dbOffsetX = pGroup->vWaferMetas[j].xDim.GetCenterX() - vOldPointCenter[iSelIndex].x;
			dbOffsetY = pGroup->vWaferMetas[j].xDim.GetCenterY() - vOldPointCenter[iSelIndex].y;
			m_ppOffsetValue[m_nCurIndex][j].dx = m_ppOffsetValue[nPreIndex][j].dx + dbOffsetX;
			m_ppOffsetValue[m_nCurIndex][j].dy = m_ppOffsetValue[nPreIndex][j].dy + dbOffsetY;
			m_ppOffsetValue[m_nCurIndex][j].deg = m_ppOffsetValue[nPreIndex][j].deg + dbDeg;
			iSelIndex++;
		}
		else
		{
			m_ppOffsetValue[m_nCurIndex][j].dx = m_ppOffsetValue[nPreIndex][j].dx;
			m_ppOffsetValue[m_nCurIndex][j].dy = m_ppOffsetValue[nPreIndex][j].dy;
			m_ppOffsetValue[m_nCurIndex][j].deg = m_ppOffsetValue[nPreIndex][j].deg;
		}
	}

	// 所有阵列取消选中
	for (int j = 0; j < nWaferSize; j++)
	{
		pGroup->vWaferMetas[j].EnableSel(false);
	}

	m_pMapView->Invalidate();

	m_bAdjust = true;  // 阵列以及被修改过
}


void CDlgMapAdjust::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pMapView->OnDrawChange(emGraph_Arrow);
}


void CDlgMapAdjust::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pMapView->OnDrawChange(emGraph_Hand);
}


void CDlgMapAdjust::OnBnClickedButtonZoomOut()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pMapView->doZoomOut(1.1);
}


void CDlgMapAdjust::OnBnClickedButtonZoomIn()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pMapView->doZoomIn(0.9);
}


void CDlgMapAdjust::OnBnClickedButtonZoomAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pMapView->doZoomAll();
}


void CDlgMapAdjust::OnBnClickedButtonForbitCanvs()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItemText(IDC_BUTTON_FORBIT_CANVAS, s);
	bool bFixedCanvas = (_T("固定画布") == s);
 
	m_pMapView->m_bFixedCanvas = bFixedCanvas;
	m_pMapView->Invalidate();
	GetDlgItem(IDC_BUTTON_SELECT)->ShowWindow(!bFixedCanvas);
	GetDlgItem(IDC_BUTTON_MOVE)->ShowWindow(!bFixedCanvas);
	GetDlgItem(IDC_BUTTON_ZOOM_OUT)->ShowWindow(!bFixedCanvas);
	GetDlgItem(IDC_BUTTON_ZOOM_IN)->ShowWindow(!bFixedCanvas);
	GetDlgItem(IDC_BUTTON_ZOOM_ALL)->ShowWindow(!bFixedCanvas);
	SetDlgItemText(IDC_BUTTON_FORBIT_CANVAS, bFixedCanvas ? _T("可变画布") : _T("固定画布"));
}


HBRUSH CDlgMapAdjust::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgMapAdjust::OnBnClickedButtonRotate()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItemText(IDC_EDIT_DEG, s);
	double dbO = _ttof(s);
	RotateSel(dbO);

	// 旋转值清空，防止连续作用
	SetDlgItemText(IDC_EDIT_DEG, _T("0"));

	m_bAdjust = true;  // 阵列以及被修改过
}


void CDlgMapAdjust::OnBnClickedButtonRotateDown()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	GetDlgItemText(IDC_EDIT_DEG, s);
	double dbO = _ttof(s);
	RotateSel(-dbO);

	// 旋转值清空，防止连续作用
	SetDlgItemText(IDC_EDIT_DEG, _T("0"));

	m_bAdjust = true;  // 阵列以及被修改过
}


void CDlgMapAdjust::OnBnClickedButtonRedPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapDoc)
		return;

	IMarkEngine *pTheEngine = CMarkPlatform::Instance()->MarkEngine();
	CString sValue;
	GetDlgItem(IDC_BUTTON_RED_PREVIEW)->GetWindowTextW(sValue);
	if (0 == sValue.Compare(L"停止红光预览"))
	{
		CMarkThread::Instance()->StopThread();
		pTheEngine->StopMark();
		GetDlgItem(IDC_BUTTON_RED_PREVIEW)->SetWindowTextW(L"开始红光预览");
		while (emMARK_STAT_BUSY == pTheEngine->GetMarkStat());
		pTheEngine->MoveTo(0, 0);
		pTheEngine->MarkEnd();
		GetDlgItem(IDC_BUTTON_OFFSET_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_OFFSET_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_OFFSET_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_OFFSET_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ROTATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ROTATE_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OFFSET_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OFFSET_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OFFSET_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OFFSET_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ROTATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ROTATE_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(FALSE);

		IMarkPen *pThePen = CMarkPlatform::Instance()->MarkDevice()->CreatePen();
		pThePen->dbPower = 1;
		pThePen->dwJumpSpeed = 1000; 
		pThePen->dwMarkSpeed = 1000; 
		pThePen->iDelayJump = 0;
		pThePen->iDelayMark = 0;
		pThePen->dwDelayPolygon = 0;
		pThePen->iDelayLaserOff = 0;
		pThePen->iDelayLaserOn = 0;
		pTheEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT, true);
		pTheEngine->MarkBegin();
		pTheEngine->UpdatePen(pThePen);
		CMarkThread::Instance()->StartThread((LPVOID *)this, PreviewProc);
		GetDlgItem(IDC_BUTTON_RED_PREVIEW)->SetWindowTextW(L"停止红光预览");
	}
}


int32_t CDlgMapAdjust::PreviewProc(LPVOID *pParam)
{
	CDlgMapAdjust* pMainDlg = (CDlgMapAdjust*)pParam;
	CMarkPlatform* pMarkPlatform = CMarkPlatform::Instance();

	int iError(0);
	FField_t field;
	CWaferMapDoc* pMapDoc = pMainDlg->m_pMapDoc;
	IMarkEngine *pTheEngine = pMarkPlatform->MarkEngine();

	field = pMapDoc->GetLeftSelField();
	if (!field.isEmpty())
	{
		// 将全局坐标转到振镜坐标系下
		CCoordTransform::Instance()->Base2Head(0, field.Left, field.Top);
		CCoordTransform::Instance()->Base2Head(0, field.Right, field.Bottom);
		pMarkPlatform->MarkDriver()->SetScanHead(1, 0);
		pTheEngine->MarkRectangle(field.Left, field.Top, field.Right, field.Bottom);
	}

	pMarkPlatform->MarkDriver()->SetScanHead(0, 1);
	// 将全局坐标转到振镜坐标系下
	field = pMapDoc->GetRightSelField();
	if (!field.isEmpty())
	{
		CCoordTransform::Instance()->Base2Head(1, field.Left, field.Top);
		CCoordTransform::Instance()->Base2Head(1, field.Right, field.Bottom);
		pTheEngine->MarkRectangle(field.Left, field.Top, field.Right, field.Bottom);
	}

	return 0;
}

void CDlgMapAdjust::OnBnClickedButtonPrintAll()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	// Add by hhhuang 20260323
	// Dummy模式下，技术员不能设置超过2颗的产品，进行打印
	if (CFactoryConfig::Instance()->m_bEnableDummy) {
		if (USERMNG->GetCurUser() >= USER_A_TECH)
		{
			AfxMessageBox(_T("技术员权限选择不能超过2颗产品！"));
			return;
		}
	}
	// END: Add by hhhuang 20260323

	m_bAdjust = true;  // 阵列以及被修改过
	m_pMapView->SetAllPrint(true);
	m_pMapView->Invalidate();
}


void CDlgMapAdjust::OnBnClickedButtonUnprintall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nullptr == m_pMapView)
		return;

	m_bAdjust = true;  // 阵列以及被修改过
	m_pMapView->SetAllPrint(false);
	m_pMapView->Invalidate();
}


void CDlgMapAdjust::OnBnClickedRadioMappingNone()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CFactoryConfig::Instance()->m_iMappingDebugSet = m_iMappingDebugSet;

	switch (m_iMappingDebugSet)
	{
	case 0:   // 无设置
		m_pMapDoc->ChangeWaferID(0);
		m_pMapView->EnablePopMenu(false);
		break;
	case 1:   // 不良品数据对比
		m_pMapDoc->ChangeWaferID(0);
		m_pMapView->EnablePopMenu(true);
		break;
	case 2:   // 全部打印BIN1图档
		m_pMapDoc->ChangeWaferID(1);
		m_pMapView->EnablePopMenu(false);
		break;
	case 3:
		m_pMapDoc->ChangeWaferID(0);
		m_pMapView->EnablePopMenu(true);
		break;
	}

	m_pMapView->Invalidate();

	ShowBinSetButton();

	IniFileWrite(_T("Global"), _T("m_iMappingDebugSet"), m_iMappingDebugSet, _T(".\\config\\MapAdjust.ini"));
}


void CDlgMapAdjust::OnBnClickedButtonSetBin1()
{
	// TODO: 在此添加控件通知处理程序代码
	int iWaferID = 0;
	UINT uID = GetFocus()->GetDlgCtrlID();
	switch (uID)
	{
	case IDC_BUTTON_SET_BIN1:
		iWaferID = 0;
		break;
	case IDC_BUTTON_SET_BIN2:
		iWaferID = 1;
		break;
	case IDC_BUTTON_SET_BIN3:
		iWaferID = 2;
		break;
	case IDC_BUTTON_SET_BIN4:
		iWaferID = 3;
		break;
	case IDC_BUTTON_SET_BIN5:
		iWaferID = 4;
		break;
	case IDC_BUTTON_SET_BIN6:
		iWaferID = 5;
		break;
	}

	m_pMapView->doChangeWafer(iWaferID);
	m_pMapView->Invalidate();
}

void CDlgMapAdjust::ShowBinSetButton()
{
	int iWaferCount = CWaferEngine::Instance()->GetGraphDoc()->GetWaferCount();

	UINT uID[] = { IDC_BUTTON_SET_BIN1, IDC_BUTTON_SET_BIN2, IDC_BUTTON_SET_BIN3, IDC_BUTTON_SET_BIN4, IDC_BUTTON_SET_BIN5, IDC_BUTTON_SET_BIN6};

	for (int i = 0; i < 6; i++)
	{
		bool bShow = iWaferCount > i;
		if (bShow)
		{
			// 根据Mapping调试设置判断：不良品数据对比和打印设置的BIN 两种模式需要显示
			if (CFactoryConfig::Instance()->m_bEnableDummy &&
				(/*(1 == CFactoryConfig::Instance()->m_iMappingDebugSet)
					|| */(3 == CFactoryConfig::Instance()->m_iMappingDebugSet)))
			{
				GetDlgItem(uID[i])->ShowWindow(TRUE);
				GetDlgItem(IDC_STATIC_M1)->ShowWindow(TRUE);
			}
			else
			{
				GetDlgItem(uID[i])->ShowWindow(FALSE);
				GetDlgItem(IDC_STATIC_M1)->ShowWindow(FALSE);
			}
		}
		else
		{
			GetDlgItem(uID[i])->ShowWindow(FALSE);
		}
		
	}


}
