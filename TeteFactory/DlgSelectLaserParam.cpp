// DlgSelectLaserParam.cpp : 实现文件
//

#include "stdafx.h"
#include "TeteFactory.h"
#include "DlgSelectLaserParam.h"
#include "afxdialogex.h"
#include "MarkPlatform.h"
#include "Wafer\WaferEngine.h"



// CDlgSelectLaserParam 对话框

IMPLEMENT_DYNAMIC(CDlgSelectLaserParam, CDialogEx)

CDlgSelectLaserParam::CDlgSelectLaserParam(std::vector<IMarkPen*>& refvMarkPen, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LASER_SELECT, pParent)
	, m_refvMarkPen(refvMarkPen)
{

}

CDlgSelectLaserParam::~CDlgSelectLaserParam()
{
}

void CDlgSelectLaserParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDlgSelectLaserParam, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &CDlgSelectLaserParam::OnDblclkList1)
END_MESSAGE_MAP()


// CDlgSelectLaserParam 消息处理程序


BOOL CDlgSelectLaserParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	size_t iSize = m_refvMarkPen.size();
	for (size_t i = 0; i < iSize; ++i) {
		m_list.AddString(CString(m_refvMarkPen[i]->sName));
		m_list.SetItemData(i, (DWORD_PTR)m_refvMarkPen[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgSelectLaserParam::OnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_list.GetCurSel();
	if (-1 == iSel)
		return;

	IMarkPen* pMarkPen = (IMarkPen*)m_list.GetItemData(iSel);
	int nPenOptionWndFlag = PenOptionWndFlag_EDIT_PARAM;
	CMarkPlatform::Instance()->MarkDevice()->SetDeviceCtrl(Laser_SetPenOptionWndFlag_I, (void*)&nPenOptionWndFlag);
	if (-1 == CMarkPlatform::Instance()->MarkDevice()->ShowPenOptionWnd(pMarkPen))
	{
		CWaferEngine::Instance()->GetGraphDoc()->UpdateAllLayerFromDB();  // 如果修改失败，应该重新获取本地参数库中的参数，以免出现信息不对等的情况
	}
}
