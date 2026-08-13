#include "stdafx.h"
#include "WaferGraphDoc.h"
#include "MarkPlatform.h"
#include "WaferLayer.h"
#include "WaferNode.h"
#include "WaferFile.h"
#include "tlObjectEngine.h"
#include "WaferEngine.h"

#include "pltfile.h"
#include "dxffile.h"

//#include "DlgObjectsArray.h"
//#include "DlgFill.h"
//#include "DlgObjectsSort.h"
//#include "DlgObjectsRotate.h"
//#include "DlgObjectsSizePos.h"

#include "Configure.h"

/////////////////////////////////////////////////////////////////////////////
/// CWaferGraph 定义
CWaferGraph::CWaferGraph(IPlatform *pPlatform)
{
	memset(m_sName, 0, sizeof m_sName);
	m_dbHeight = 5;
	m_dbWidth = 5;

	m_IsMarkable = true;
	m_IsLocked = false;
	m_pPlatform = pPlatform;

	m_theObjects.clear();

	m_IsModify = false;

	m_uCurrent = 0;

	m_iUsedCount = 0;

}

CWaferGraph::~CWaferGraph()
{
	DeleteAllObjects();
}

void CWaferGraph::SetName(const char * s)
{
	memset(m_sName, 0, sizeof m_sName);
	strcpy_s(m_sName, sizeof(m_sName), s);
}

const char * CWaferGraph::GetName(void) const
{
	return m_sName;
}

void CWaferGraph::SetHeight(double h)
{
	m_dbHeight = h;
}

double CWaferGraph::GetHeight(void) const
{
	return m_dbHeight;
}

void CWaferGraph::SetWidth(double w)
{
	m_dbWidth = w;
}

double CWaferGraph::GetWidth(void) const
{
	return m_dbWidth;
}

void CWaferGraph::SetUsed(bool b)
{
	m_iUsedCount = b ? (m_iUsedCount + 1) : (m_iUsedCount - 1);
	m_iUsedCount = (m_iUsedCount < 0) ? 0 : m_iUsedCount;
}

bool CWaferGraph::IsUsed(void) const
{
	//return (m_iUsedCount > 0);
	return (CWaferEngine::Instance()->GetMapDoc()->GetMapObjCount(m_sName) > 0);
}

void CWaferGraph::SetLock(bool lock)
{
	m_IsLocked = lock;
}

bool CWaferGraph::IsLocked(void) const
{
	return m_IsLocked;
}

void CWaferGraph::SetMarkable(bool mark)
{
	m_IsMarkable = mark;
}

bool CWaferGraph::IsMarkable(void) const
{
	return m_IsMarkable;
}

bool CWaferGraph::ShowAttributeWnd(void)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (GetObjSelCount() == 0)
	{
		// 显示文档属性框
	}
	else
	{
		// 显示第一个对象的属性框
		for (auto & item : m_theObjects)
		{
			if (item->IsSelected())
			{
				return item->ShowAttributeWnd();
			}
		}
	}
	return true;
}

void CWaferGraph::ShowArrayWnd(void)
{
#if 0
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());  // 位于DLL中时，需要
	if (GetObjSelCount() == 0) 
		return;
	stArrayData_t ArrayData;
	CDlgObjectsArray dlg(&ArrayData);
	if (IDOK == dlg.DoModal())
	{
		FField_t field = GetSelField();
		double dbPitchX = field.width() + ArrayData.dbColPitch;
		double dbPitchY = field.height() + ArrayData.dbRowPitch;
		double dbOffsetX, dbOffsetY;
		int nSelObjCnt = m_theObjects.size();
		for (uint32_t y = 0; y < ArrayData.nRowCount; y++)//--逐行阵列
		{
			dbOffsetY = -dbPitchY * y;
			for (uint32_t x = 0; x < ArrayData.nColCount; x++)
			{
				dbOffsetX = dbPitchX * x;
				if ((x == 0) && (y == 0))continue;

				IGraphObject *pTheObj = NULL;
				for (int i = 0; i < nSelObjCnt; i++)
				{
					pTheObj = m_theObjects[i];
					if (pTheObj->IsSelected())
					{
						IGraphObject *pNewObj = pTheObj->Clone();
						AppendObject(pNewObj);
						pNewObj->Repos(dbOffsetX, dbOffsetY, 0);//--相对偏移复制
					}
				}
			}
		}
	}
#endif
}
void CWaferGraph::ShowSortWnd(void)
{
#if 0 
	/*AFX_MANAGE_STATE(AfxGetStaticModuleState());*/
	if (m_theObjects.size() == 0) return;

	//更新对象ID
	for (int i = 0, nCount = m_theObjects.size(); i < nCount; i++)
	{
		m_theObjects[i]->SetID(i);
	}
	CDlgObjectSort dlg(this);
	dlg.DoModal();
#endif
}
void CWaferGraph::ShowHatchWnd(void)
{
#if 0
	/*AFX_MANAGE_STATE(AfxGetStaticModuleState());*/
	if (GetObjSelCount() == 0) 
		return;
	std::vector<stHatch_t> vstHatch;
	int nMaxCount = 0;
	IGraphObject * pGraphObj = NULL;
	for (auto & item : m_theObjects)
	{
		if (item->IsSelected())  
		{
			// 多个选中对象时，按填充对象多的来定
			int nSize = item->GetHatchCount();
			if (nSize > nMaxCount)
			{
				pGraphObj = item;
				nMaxCount = nSize;
			}
		}
	}
	if (NULL != pGraphObj)
	{
		int nCount = pGraphObj->GetHatchCount();
		for (int i = 0; i < nCount; i++)
		{
			stHatch_t stHatch;
			if(pGraphObj->GetHatch(i, stHatch))
				vstHatch.push_back(stHatch);
		}
	}
	bool bShowContour = pGraphObj->IsMarkFrame();
	CDlgFill dlg(vstHatch, bShowContour);
	if (IDOK == dlg.DoModal())
	{
		for (auto & item : m_theObjects)
		{
			if (item->IsSelected())
			{
				item->ClearHatch();
				for (auto & itHatch : vstHatch)
				{
					item->AddHatch(itHatch);
				}
				item->SetMarkFrame(bShowContour);
				item->Update(false);
			}
		}
	}
#endif 
}
void CWaferGraph::ShowRotateWnd(void)
{
#if 0
	/*AFX_MANAGE_STATE(AfxGetStaticModuleState());*/
	if (GetObjSelCount() == 0) return;
	CDlgObjectsRotate dlg(this);
	dlg.DoModal();
#endif
}
void CWaferGraph::ShowSizeSelWnd(void)
{
#if 0
	/*AFX_MANAGE_STATE(AfxGetStaticModuleState());*/
	if (GetObjSelCount() == 0) return;
	CDlgSizePos dlg(this);
	dlg.DoModal();
#endif
}
void CWaferGraph::Draw(IGraphRender *pTheRender)
{
	IGraphObject *pTheObj = NULL;
	size_t size = m_theObjects.size();

	pTheRender->MoveTo(0, 0);
	for (size_t i = 0;i<size;i++)
	{
		pTheObj = m_theObjects[i];
        if (NULL == pTheObj)
            break;
		if (!pTheObj->IsVisible())
			continue;
		int nPenStyle = PS_SOLID;
		int nWidth = 1;
		COLORREF crColor;
		
		int nLayerID = pTheObj->GetLayer();
		CWaferGraphLayer * pLayer =  CWaferEngine::Instance()->GetGraphDoc()->GetLayer(nLayerID);
		
		if(NULL == pLayer)
			crColor = CWaferEngine::Instance()->GetGraphDoc()->GetLayer(0)->GetPen()->dwColor;
		else
		    crColor = pLayer->GetPen()->dwColor;

		if (!pTheObj->IsMarkable())
		{
			nPenStyle = PS_DOT;
			crColor = RGB(150, 150, 150);
		}
		if (pTheObj->IsSelected())
			crColor = RGB(255, 0, 0);

		pTheRender->BeginPaint(nPenStyle, nWidth, crColor);
		pTheObj->SetConfigInfo(Object_ShowTextFrame_B, (void*)&CConfigure::Instance()->m_bShowTextFrame);
		pTheObj->Draw(pTheRender);
		pTheRender->EndPaint();
	}
	pTheRender->MoveTo(0, 0);
}
int32_t CWaferGraph::Mark(IMarkEngine *pTheEngine, int32_t nMarkLayer, uint32_t nMarkFlag)
{
    IGraphObject *pTheObj = NULL;
    size_t size = m_theObjects.size();
    int iError(0);
    for (size_t i = 0;i<size;i++)
    {
        pTheObj = m_theObjects[i];
        if (!pTheObj->IsMarkable())
            continue;

		// 判断层ID是否一致，如果不一致，则不标记
		int nLayerID = pTheObj->GetLayer();
		if(-1 != nMarkLayer && nLayerID != nMarkLayer)
			continue;

		// 当标记所有层（nMarkLayer=-1）时，只有设置了更新激光参数标志时，才会更新激光参数
		if (-1 == nMarkLayer && (MARKFLAG_UPDATEPEN & nMarkFlag))
		{
			CWaferGraphLayer * pLayer = CWaferEngine::Instance()->GetGraphDoc()->GetLayer(nLayerID);
			if (nullptr == pLayer)
			{
				CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_ERROR, "图形对象对应的激光参数不存在！");
				CMarkPlatform::Instance()->Message(emMSG_TYPE_ERROR, "图形对象对应的激光参数不存在！");
				return -1;
			}
			IMarkPen * pPen = pLayer->GetPen();
			if (!pTheEngine->UpdatePen(pPen))
			{
				CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_ERROR, "激光参数更新失败！");
				CMarkPlatform::Instance()->Message(emMSG_TYPE_ERROR, "激光参数更新失败！");
				return -1;
			}
		}

		// 判断是否为变量或2D Barcode数据
		auto ObjIsVariable = [&]() {
			bool b2DBarcode = false;
			int iTextObjectType = TEXT_OBJECT_NORMAL;
			pTheObj->GetConfigInfo(Object_Is2DBarcode_B, (void*)&b2DBarcode);
			pTheObj->GetConfigInfo(Object_Text_Object_Type_I, (void*)&iTextObjectType);
			return b2DBarcode || pTheObj->IsVariable() || (TEXT_OBJECT_INDEX == iTextObjectType);
		};
		if (MARKFLAG_ONLY_MARK_VAR & nMarkFlag) {
			if (!ObjIsVariable()) continue;
		}
		else if (MARKFLAG_ONLY_MARK_NON_VAR & nMarkFlag) {
			if (ObjIsVariable()) continue;
		}

		// 日期对象要在编辑之前更新为当前日期
		int nType = pTheObj->GetType();
		if (emObjType_TIME == nType)
		{
			pTheObj->Update(true);
			SetModified(true, false);
		}

        iError = pTheObj->Mark(pTheEngine);
        if (0 != iError)
            break;

		if (pTheObj->IsVariable())
		{
			// 日期是在标记前才要更新
			if (emObjType_TIME != nType)
			{
				pTheObj->Update(true);
				SetModified(true, false);
			}
		}

		m_pPlatform->SystemIdle();
    }

    return iError;
}
bool CWaferGraph::LoadFile(IFileReader *pTheReader)
{
	if (NULL == pTheReader) 
		return false;

	DeleteAllObjects();

	m_mapAlign.clear();
	IGraphObject * pBaseObject = NULL;
	stTextAlign_t stTextAlign;

	bool bRet = true;
	uint32_t code;
	while (CN_DOC_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_DOC_NAME:
				memset(m_sName, 0, sizeof m_sName);
				pTheReader->GetString(m_sName);
				break;
			case CN_DOC_MARKABLE:
				m_IsMarkable = pTheReader->GetBool();
				break;
			case CN_DOC_LOCKED:
				m_IsLocked = pTheReader->GetBool();
				break;
			case CN_DOC_MARKCORNER:
				m_iMarkStartCorner = pTheReader->GetInt32();
				break;
			case CN_DOC_MARKSORT:
				m_iMarkSortType = pTheReader->GetInt32();
				break;
			case CN_DOC_MARKDIRT:
				m_iMarkDirtType = pTheReader->GetInt32();
				break;
			case CN_DOC_WIDTH:
				m_dbWidth = pTheReader->GetDouble();
				break;
			case CN_DOC_HEIGHT:
				m_dbHeight = pTheReader->GetDouble();
				break;
			case CN_DOC_ALIGN_BASEOBJ:
			{
				char sName[MAX_TXT_SIZE] = {0};
				pTheReader->GetString(sName);
				pBaseObject = GetObject(sName);
				break;
			}
			case CN_DOC_ALIGN_CHILDOBJ:
			{
				char sName[MAX_TXT_SIZE] = { 0 };
				pTheReader->GetString(sName);
				stTextAlign.pObject = GetObject(sName);
				break;
			}
			case CN_DOC_ALIGN_STYLE:
			{
				stTextAlign.uStyle = pTheReader->GetInt8();
				m_mapAlign.insert(make_pair(pBaseObject, stTextAlign));
				break;
			}
			case CN_DOC_OBJTYPE:
			{
				int32_t iObjType = pTheReader->GetInt32();
				IGraphObject * pTheObject = AppendObject(iObjType);
				if (NULL != pTheObject)
				{
					if (!pTheObject->LoadFile(pTheReader))
						bRet = false;
					if (!pTheObject->Update(false))
						bRet = false;
				}
				break;
			}
			default: break;
			}
		}
	}
	return bRet;
}
bool CWaferGraph::SaveFile(IFileWriter *pTheWriter)
{
	if (NULL == pTheWriter) return false;

	pTheWriter->WriteBegin(CN_DOC_BASE);    //Doc开始标志

	pTheWriter->WriteString(CN_DOC_NAME, m_sName);

	pTheWriter->WriteBool(CN_DOC_MARKABLE, m_IsMarkable);
	pTheWriter->WriteBool(CN_DOC_LOCKED, m_IsLocked);

	pTheWriter->WriteInt32(CN_DOC_MARKCORNER, m_iMarkStartCorner);
	pTheWriter->WriteInt32(CN_DOC_MARKSORT, m_iMarkSortType);
	pTheWriter->WriteInt32(CN_DOC_MARKDIRT, m_iMarkDirtType);

	pTheWriter->WriteDouble(CN_DOC_WIDTH, m_dbWidth);
	pTheWriter->WriteDouble(CN_DOC_HEIGHT, m_dbHeight);

	// 图形对象
	for (auto & item : m_theObjects)
	{
		pTheWriter->WriteInt32(CN_DOC_OBJTYPE, item->GetType());
		item->SaveFile(pTheWriter);
	}

	// 保存对齐参数, 必须在对象后面保存
	for (auto & item : m_mapAlign)
	{
		pTheWriter->WriteString(CN_DOC_ALIGN_BASEOBJ, item.first->GetName());
		pTheWriter->WriteString(CN_DOC_ALIGN_CHILDOBJ, item.second.pObject->GetName());
		pTheWriter->WriteInt8(CN_DOC_ALIGN_STYLE, item.second.uStyle);
	}

	pTheWriter->WriteEnd(CN_DOC_BASE);
	return true;
}
bool CWaferGraph::ImportFile(const char *sName)
{
	CStringA s = sName;
	s = s.Right(3).MakeLower();
	if (s == "plt")
	{
		CPltFile plt;
		if (0 != plt.LoadFile(sName, this))
			return false;
	}
	else if(s == "dxf")
	{
		CDxfFile dxf;
		IGraphObject * pObject = AppendObject(emObjType_SHAPE);
		if (!dxf.LoadFile(sName, pObject))
		{
			DeleteObject(pObject);  // 加载失败，删除对象
			m_pPlatform->Message(emMSG_TYPE_ERROR, "dxf文档加载失败！");
			m_pPlatform->PrintLog(emLOG_LEVEL_ERROR, "dxf文档加载失败！");
			return false;
		}
		if (pObject->GetMetaCount() <= 0)
		{
			DeleteObject(pObject);    // 对象中不包含图元，删除对象
			m_pPlatform->Message(emMSG_TYPE_WARN, "未识别dxf中的图形！");
			m_pPlatform->PrintLog(emLOG_LEVEL_WARN, "未识别dxf中的图形！");
			return false;
		}

		// 将对象居中显示
		FField_t field = pObject->GetField();
		pObject->Repos(-field.CenterX(), -field.CenterY(), 0);
	}
	return true;
}
bool CWaferGraph::ExportFile(const char *sName)
{
	return true;
}
bool CWaferGraph::CopyToClipboard()
{
	CStringA sFileName = ".\\His\\Clipboard\\Clipboard.cbd";

	if (!PathIsDirectoryA(sFileName.GetString()))
	{
		if (!MakeSureDirectoryPathExists(sFileName.GetString()))  //创建多层目录
		{
			return false;
		}
	}

	CFileWriter *pTheWriter = new CFileWriter();
	if (!pTheWriter->Open(sFileName.GetString()))
	{
		delete pTheWriter;
		pTheWriter = NULL;
		return false;
	}

	pTheWriter->WriteBegin(CN_DOC_BASE);    //Doc开始标志

	for (auto & item : m_theObjects)
	{
		if (item->IsSelected())
		{
			pTheWriter->WriteInt32(CN_OBJECT_TYPE, item->GetType());
			item->SaveFile(pTheWriter);
		}
	}

	pTheWriter->WriteEnd(CN_DOC_BASE);

	pTheWriter->Close();
	delete pTheWriter;
	pTheWriter = NULL;

	return true;
}
bool CWaferGraph::CopyFromClipboard()
{
	std::string sFileName = ".\\His\\Clipboard\\Clipboard.cbd";
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(sFileName.c_str()))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	uint32_t code(0);
	int32_t iObjType(0);

	IGraphObject * pBaseObject = NULL;
	stTextAlign_t stTextAlign;

	while (pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else if (CN_OBJECT_TYPE == code)
		{
			switch (code)
			{
			case CN_OBJECT_TYPE:
			{
				int32_t iObjType = pTheReader->GetInt32();
				IGraphObject * pTheObject = AppendObject(iObjType);
				if (NULL != pTheObject)
				{
					pTheObject->LoadFile(pTheReader);
					pTheObject->Update(false);
					// 修改复制对象的名称，遍历所有对象的名称
					ChangeCopyObjectName(pTheObject);

					pTheObject->SetSelected(true);
				}
				break;
			}
			}
		}
	}
	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;

	return true;
}

bool CWaferGraph::GetModified() const
{
	// 如果文档本身被修改，则不用获取对象是否被修改，直接返回
	if (m_IsModify) return true;

	bool bModified = false;
	for (auto & item : m_theObjects)
	{
		bModified = item->GetModified();
		if (bModified) break;
	}
	return bModified;
}
void CWaferGraph::AddUndoList()
{
	SaveTmpLayer();
	m_vuUndo.push_back(m_uCurrent++);
	m_vuRedo.clear();
}
void CWaferGraph::SetModified(bool modify, bool bAddUndo)
{
	m_IsModify = modify;
	
	// 如果设置为已改动，则需要将文档保存到撤销队列文档中去
	if (modify && bAddUndo)
	{
		AddUndoList();
	}
}
bool CWaferGraph::Redo()
{
	if (!CanRedo()) return false;

	int32_t uNext = m_vuRedo[m_vuRedo.size() - 1];
	m_vuRedo.pop_back();

	OpenTmpLayer(uNext);
	m_vuUndo.push_back(uNext);

	return true;
}
bool CWaferGraph::Undo()
{
	if (!CanUndo()) return false;

	int32_t uCurrent = m_vuUndo[m_vuUndo.size() - 1];
	int32_t uPre = m_vuUndo[m_vuUndo.size() - 2];
	m_vuUndo.pop_back();

	OpenTmpLayer(uPre);
	m_vuRedo.push_back(uCurrent);

	return true;
}

int32_t CWaferGraph::GetObjCount(void) const
{
	return m_theObjects.size();
}

int32_t CWaferGraph::GetObjSelCount(void) const
{
	int iSelCount = 0;

	IGraphObject *pTheObj = NULL;
	for (size_t i = 0;i<m_theObjects.size();i++)
	{
		pTheObj = m_theObjects[i];
		if (pTheObj->IsSelected())
			iSelCount++;
	}
	return iSelCount;
}
IGraphObject *CWaferGraph::AppendObject(int iObjType)
{
	if (NULL == m_pPlatform)
		return NULL;

	IGraphObject *pTheObj = m_pPlatform->ObjectEngine()->Create(iObjType);
	
	pTheObj->SetID(m_theObjects.size());
	m_theObjects.push_back(pTheObj);

	uint32_t nActiveLayerID = CWaferEngine::Instance()->GetGraphDoc()->ActiveLayerID();
	pTheObj->SetLayer(nActiveLayerID);

	return pTheObj;
}
void CWaferGraph::AppendObject(IGraphObject *pTheObj)
{
	pTheObj->SetID(m_theObjects.size());
	m_theObjects.push_back(pTheObj);

	pTheObj->SetSelected(true); // 选中

	//pTheObj->SetLayer(0);
	if (0 != m_theObjects.size())
	{
		m_stFieldAll = m_stFieldAll + pTheObj->GetField();
	}
	else
	{
		m_stFieldAll = pTheObj->GetField();
	}

}

bool CWaferGraph::DeleteObject(int32_t iObjID)
{
	int32_t iObjIndex = 0;
	for (auto itt = m_theObjects.begin(); itt != m_theObjects.end();)
	{
		IGraphObject *pTheObject = (IGraphObject *)*itt;
		if (iObjIndex == iObjID)
		{
			itt = m_theObjects.erase(itt);
			deleteTextAlign(pTheObject);   // 删除对齐关系,此处一定要注意
			delete pTheObject;
			pTheObject = NULL;
			break;
		}
		else
			itt++;
		iObjIndex++;
	}

	// 更新对象ID
	UpdateObjID();

	// 更新对象区域
	updateRange();
	
	return true;
}

bool CWaferGraph::DeleteSelObjects(void)
{
	for (auto itt = m_theObjects.begin(); itt != m_theObjects.end();)
	{
		IGraphObject *pTheObject = (IGraphObject *)*itt;
		if (pTheObject->IsSelected())
		{
			itt = m_theObjects.erase(itt);
			deleteTextAlign(pTheObject);   // 删除对齐关系，此处一定要注意
			delete pTheObject;
			pTheObject = nullptr;
		}
		else
			itt++;
	}

	// 更新对象ID
	UpdateObjID();

	// 更新对象区域
	updateRange();

	return true;
}
bool CWaferGraph::DeleteAllObjects(void)
{
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		IGraphObject *pTheObject = m_theObjects[i];
		deleteTextAlign(pTheObject);   // 删除对齐关系，此处一定要注意
		delete pTheObject;
		pTheObject = NULL;
	}
	m_theObjects.clear();

	m_stFieldAll.Left = 0.0;
	m_stFieldAll.Top = 0.0;
	m_stFieldAll.Right = 0.0;
	m_stFieldAll.Bottom = 0.0;
	return true;
}
IGraphObject * CWaferGraph::GetObject(int32_t iObjIndex)
{
	int32_t iObjCnt = m_theObjects.size();
	if ((iObjIndex < 0) || (iObjIndex >= iObjCnt))
		return NULL;
	return m_theObjects[iObjIndex];
}

IGraphObject * CWaferGraph::GetObject(const char * sName)
{
	if (NULL == sName)
		return NULL;
	
	for (auto & item : m_theObjects)
	{
		if (!strcmp(sName, item->GetName()))
		{
			return item;
		}
	}
	return NULL;
}
void CWaferGraph::UpdateAllObjects(void)
{
	IGraphObject *pTheObject = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObject = m_theObjects[i];
		pTheObject->Update();
	}
}
void CWaferGraph::UpdateSelObjects(void)
{
	IGraphObject *pTheObject = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObject = m_theObjects[i];
		if (pTheObject->IsSelected())
			pTheObject->Update();
	}
}
/**
* @功能 合并选中对象，不同图层的对象，如果要合并，则全部改成同一个图层对象
*/
void CWaferGraph::MergeSelObjects(void)
{
	if (GetObjSelCount() < 2) return;
	int32_t nFirstLayerID = -1;   //第一个选中对象的图层号
	
	// 合并的对象只能是基本图形对象
	IGraphObject * pNewObj = m_pPlatform->ObjectEngine()->Create(emObjType_SHAPE);

	for (auto & item : m_theObjects)
	{
		if (item->IsSelected())
		{
			if (-1 == nFirstLayerID)
				nFirstLayerID = item->GetLayer();
			
			while (item->GetMetaCount() > 0)
			{
				IGraphMeta * pMeta = item->GetMeta(0);
				if (emMETA_TAG_USELESS == pMeta->MetaTag())
				{
					item->DeleteMeta(0);
					continue;
				}					
				pMeta = item->DetachMeta(0);   // 分离图元
				if (NULL != pMeta)
				{
					pMeta->SetMetaTag(emMETA_TAG_FRAME);
					pNewObj->AppendMeta(pMeta);   // 如果有填充属性的图元，合并之后，改变其属性为外框
				}
			}
		}
	}
	// 删除所有选中的对象
	DeleteSelObjects();

	// 将新对象添加到文档中
	AppendObject(pNewObj);
	pNewObj->SetLayer(nFirstLayerID);
}

void CWaferGraph::SplitSelObjects(void)
{
	if (GetObjSelCount() == 0) return;

	vector<int32_t> vnLayerID;   //所有选中对象的图层号
	vector<IGraphObject*> vNewObj;   // 新对象
	IObjectEngine * pObjectFactory = m_pPlatform->ObjectEngine();

	for (auto & item : m_theObjects)
	{
		if (item->IsSelected())
		{
			int nLayerID = item->GetLayer();

			while (item->GetMetaCount() > 0)
			{
				IGraphMeta * pMeta = item->GetMeta(0);
				if (emMETA_TAG_USELESS == pMeta->MetaTag())
				{
					item->DeleteMeta(0);
					continue;
				}
				pMeta = item->DetachMeta(0);   // 分离图元
				if (NULL != pMeta)
				{
					IGraphObject * pNewObj = pObjectFactory->Create(emObjType_SHAPE);
					pMeta->SetMetaTag(emMETA_TAG_FRAME);
					pNewObj->AppendMeta(pMeta);
					vNewObj.push_back(pNewObj);
					vnLayerID.push_back(nLayerID);
				}
			}
		}
	}

	// 删除所有选中的对象
	DeleteSelObjects();

	// 将新对象添加到文档中
	for (int i = 0, len = vNewObj.size(); i < len; i++)
	{
		AppendObject(vNewObj[i]);
		vNewObj[i]->SetLayer(vnLayerID[i]);
	}
}


bool CWaferGraph::SelectAllObjects(void)
{
	IGraphObject *pTheObject = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObject = m_theObjects[i];
		pTheObject->SetSelected(true);
	}
	updateSelRange();
	return true;
}
bool CWaferGraph::UnSelectAllObjects(void)
{
	IGraphObject *pTheObject = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObject = m_theObjects[i];
		pTheObject->SetSelected(false);
	}
	updateSelRange();
	return true;
}
void CWaferGraph::GetSelObj(std::vector<IGraphObject*>& vpTheObj)
{
	vpTheObj.clear();
	IGraphObject *pTheObj = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (pTheObj->IsSelected())
			vpTheObj.push_back(pTheObj);
	}
}
//---------------------------------------------------------------------------
void CWaferGraph::Resise(double dbLeft, double dbTop, double dbRight, double dbBottom, int iAbs)
{
	IGraphObject *pTheObj = NULL;

	if (0 == iAbs)
	{
		dbLeft = dbLeft - m_stFieldAll.Left;
		dbTop = dbTop - m_stFieldAll.Top;
		dbRight = dbRight - m_stFieldAll.Right;
		dbBottom = dbBottom - m_stFieldAll.Bottom;
	}

	double dbZoomX, dbZoomY;
	dbZoomX = (dbRight - dbLeft) / m_stFieldAll.width();
	dbZoomY = (dbTop - dbBottom) / m_stFieldAll.height();

	double dx, dy;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		dx = (pTheObj->GetField().Left - m_stFieldAll.Left)*dbZoomX;
		dy = (pTheObj->GetField().Top - m_stFieldAll.Top)*dbZoomY;

		pTheObj->Resize(dbZoomX, dbZoomY);
		pTheObj->Repos(dbLeft, dbTop, emPOS_ABS);
		pTheObj->Repos(dx, dy, emPOS_REL);
	}
	updateRange();
}
void CWaferGraph::Repos(double px, double py, int iAbs)
{
	IGraphObject *pTheObj = NULL;
	double dx, dy;
	if (emPOS_ABS == iAbs)
	{
		dx = px - m_stFieldAll.Left;
		dy = py - m_stFieldAll.Top;
	}
	else
	{
		dx = px;
		dy = py;
	}

	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];

		pTheObj->Repos(dx, dy, 0);
	}
	updateRange();
}
void CWaferGraph::Rotate(double cx, double cy, double angle, int iAbs)
{
	IGraphObject *pTheObj = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];

		pTheObj->Rotate(cx, cy, angle);
	}

	updateRange();
}
void CWaferGraph::ResiseSel(double dbLeft, double dbTop, double dbRight, double dbBottom)
{
	IGraphObject *pTheObj = NULL;

	//--计算缩放比例
	double dbZoomX, dbZoomY;
	if (D_EQUAL(m_stFieldSel.width(), 0))
		dbZoomX = 1;
	else
		dbZoomX = (dbRight - dbLeft) / m_stFieldSel.width();
	if (D_EQUAL(m_stFieldSel.height(), 0))
		dbZoomY = 1;
	else
		dbZoomY = (dbTop - dbBottom) / m_stFieldSel.height();

	double dx, dy;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (!pTheObj->IsSelected())
			continue;
		dx = (pTheObj->GetField().Left - m_stFieldSel.Left)*dbZoomX;
		dy = (pTheObj->GetField().Top - m_stFieldSel.Top)*dbZoomY;

		pTheObj->Resize(dbZoomX, dbZoomY);
		pTheObj->Repos(dbLeft, dbTop, emPOS_ABS);
		pTheObj->Repos(dx, dy, emPOS_REL);
	}
	updateRange();
}
void CWaferGraph::ReposSel(double px, double py, int iAbs)
{
	IGraphObject *pTheObj = NULL;
	double dx, dy;
	if (emPOS_ABS == iAbs)
	{
		dx = px - m_stFieldSel.Left;
		dy = py - m_stFieldSel.Top;
	}
	else
	{
		dx = px;
		dy = py;
	}

	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (pTheObj->IsSelected())
			pTheObj->Repos(dx, dy, 0);
	}
	updateRange();
}
void CWaferGraph::RotateSel(double cx, double cy, double angle, int iAbs)
{
	IGraphObject *pTheObj = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (pTheObj->IsSelected())
			pTheObj->Rotate(cx, cy, angle);
	}

	updateRange();
}
bool CWaferGraph::AlignHorLeft(void)
{
	if (m_stFieldSel.isEmpty()) return false;
	double dbLeft, dbTop;
	dbLeft = m_stFieldSel.Left;

	IGraphObject *pTheObj = NULL;

	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (NULL != pTheObj && pTheObj->IsSelected())
		{
			dbTop = pTheObj->GetField().Top;
			pTheObj->Repos(dbLeft, dbTop, 1);
		}
	}
	updateSelRange();

	return true;
}
bool CWaferGraph::AlignHorCenter(void)
{
	if (m_stFieldSel.isEmpty()) return false;
	double dbLeft, dbTop, dbBase;
	dbBase = (m_stFieldSel.Left + m_stFieldSel.Right) / 2;

	IGraphObject *pTheObj = NULL;

	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (NULL != pTheObj && pTheObj->IsSelected())
		{
			dbTop = pTheObj->GetField().Top;
			dbLeft = dbBase - pTheObj->GetField().width() / 2;
			pTheObj->Repos(dbLeft, dbTop, 1);
		}
	}
	updateSelRange();

	return true;
}
bool CWaferGraph::AlignHorRight(void)
{
	if (m_stFieldSel.isEmpty()) return false;
	double dbLeft, dbTop, dbBase;
	dbBase = m_stFieldSel.Right;

	IGraphObject *pTheObj = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (NULL != pTheObj && pTheObj->IsSelected())
		{
			dbTop = pTheObj->GetField().Top;
			dbLeft = dbBase - pTheObj->GetField().width();
			pTheObj->Repos(dbLeft, dbTop, 1);
		}
	}
	updateSelRange();

	return true;
}
bool CWaferGraph::AlignVerTop(void)
{
	if (m_stFieldSel.isEmpty()) return false;
	double dbLeft, dbTop, dbBase;
	dbBase = m_stFieldSel.Top;

	IGraphObject *pTheObj = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (NULL != pTheObj && pTheObj->IsSelected())
		{
			dbLeft = pTheObj->GetField().Left;
			dbTop = dbBase;
			pTheObj->Repos(dbLeft, dbTop, 1);
		}
	}
	updateSelRange();
	return true;
}
bool CWaferGraph::AlignVerCenter(void)
{
	if (m_stFieldSel.isEmpty()) return false;
	double dbLeft, dbTop, dbBase;
	dbBase = (m_stFieldSel.Top + m_stFieldSel.Bottom) / 2;

	IGraphObject *pTheObj = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (NULL != pTheObj && pTheObj->IsSelected())
		{
			dbLeft = pTheObj->GetField().Left;
			dbTop = dbBase + pTheObj->GetField().height() / 2;
			pTheObj->Repos(dbLeft, dbTop, 1);
		}
	}
	updateSelRange();

	return true;
}
bool CWaferGraph::AlignVerBottom(void)
{
	if (m_stFieldSel.isEmpty()) return false;
	double dbLeft, dbTop, dbBase;
	dbBase = m_stFieldSel.Bottom;

	IGraphObject *pTheObj = NULL;
	size_t iObjCnt = m_theObjects.size();
	for (size_t i = 0; i<iObjCnt;i++)
	{
		pTheObj = m_theObjects[i];
		if (NULL != pTheObj && pTheObj->IsSelected())
		{
			dbLeft = pTheObj->GetField().Left;
			dbTop = dbBase + pTheObj->GetField().height();
			pTheObj->Repos(dbLeft, dbTop, 1);
		}
	}
	updateSelRange();

	return true;
}
bool CWaferGraph::AlignCenter(void)
{
	ReposSel(-1 * m_stFieldSel.width() / 2, m_stFieldSel.height() / 2, 1);
	return true;
}

bool CWaferGraph::AlignHorAvgSpace(void)
{

	return true;
}

bool CWaferGraph::AlignVerAvgSpace(void)
{
	return true;
}

bool CWaferGraph::TextAlign(void)
{
	if (0 == m_mapAlign.size())
		return true;

	IGraphObject * pBase, * pChild;
	for (auto & item : m_mapAlign)
	{
		pBase = item.first;
		pChild = item.second.pObject;
		if (NULL == pBase || NULL == pChild)
			continue;

		FField_t fBase = pBase->GetField();
		FField_t fChild = pChild->GetField();
		switch (item.second.uStyle)
		{
			case TEXTALIGN_LEFT:
				pChild->Repos(fBase.Left - fChild.Left, 0, emPOS_REL);
				break;
			case TEXTALIGN_CENTER:
				pChild->Repos(fBase.CenterX() - fChild.CenterX(), 0, emPOS_REL);
				break;
			case TEXTALIGN_RIGHT:
				pChild->Repos(fBase.Right - fChild.Right, 0, emPOS_REL);
				break;
			case TEXTALIGN_JUSTIFY:
			{
				// 两端对齐，修改文本对象长度类型为：固定长度，并将文本长度设置为基准对象的长度
				// 更新对象，就是调用Update, 重新生成文本
				// 然后将对象与基准对象左对齐，这样就能达到两端对齐
				double dbFixedLen = fBase.width();
				int nLenStyle = 0;  // 设置为固定长度
				pChild->SetConfigInfo(Object_TextLenStyle, (void*)&nLenStyle);
				pChild->SetConfigInfo(Object_TextFixedLen, (void*)&dbFixedLen);

				// 更新对象
				if (false == pChild->Update(true))
					return false;

				// 更新后，恢复长度类型和固定长度
				dbFixedLen = 0;
				nLenStyle = 1;  // 设置为可变长度
				pChild->SetConfigInfo(Object_TextLenStyle, (void*)&nLenStyle);
				pChild->SetConfigInfo(Object_TextFixedLen, (void*)&dbFixedLen);

				// 与基准对象向左对齐
				pChild->Repos(fBase.Left - fChild.Left, 0, emPOS_REL);

				break;
			}
		}
	}
	return true;
}

void CWaferGraph::updateRange(void)
{
	size_t iObjCount = m_theObjects.size();
	if (0 == iObjCount)return;

	IGraphObject *pTheObj = NULL;
	pTheObj = m_theObjects[0];
	m_stFieldAll = pTheObj->GetField();
	if (iObjCount > 1)
	{
		for (size_t i = 1; i < iObjCount; i++)
		{
			pTheObj = m_theObjects[i];
			m_stFieldAll = m_stFieldAll + pTheObj->GetField();
		}
	}
}
void CWaferGraph::updateSelRange(void)
{
	size_t iObjCount = m_theObjects.size();
	if (0 == iObjCount)return;

	IGraphObject *pTheObj = NULL;
	pTheObj = m_theObjects[0];
	FField_t field;
	for (size_t i = 0; i < iObjCount; i++)
	{
		pTheObj = m_theObjects[i];
		if (pTheObj->IsSelected())
		{
			if (field.isEmpty())
				field = pTheObj->GetField();
			else
				field = field + pTheObj->GetField();
		}
	}
	m_stFieldSel = field;
}

void CWaferGraph::UpdateObjID()
{
	size_t nSize = m_theObjects.size();
	for (size_t i = 0; i < nSize; i++)
	{
		m_theObjects[i]->SetID(i);
	}
}

void CWaferGraph::ChangeCopyObjectName(IGraphObject * pCopyObj)
{
	if (nullptr == pCopyObj)
		return;
	
	std::string sObjName = pCopyObj->GetName();
	std::string sObjCopyName = sObjName + "-";
	std::vector<std::string> vsCopyName;
	for (auto & item : m_theObjects)
	{
		if (nullptr == item)
			continue;
		
		std::string s = item->GetName();
		if (nullptr != strstr(s.c_str(), sObjCopyName.c_str()))
		{
			vsCopyName.push_back(s);
		}
	}
	char buff[1024] = { 0 };
	int i = 0;
	for (int i = 0; i < 10; i++)
	{
		bool bFind = false;
		memset(buff, 0, 1024);
		sprintf(buff, "%s(%d)", sObjCopyName.c_str(), i+1);
		for (auto & item : vsCopyName)
		{
			if (!strcmp(item.c_str(), buff))
			{
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			pCopyObj->SetName(buff);
			return;
		}
	}
	pCopyObj->SetName(buff);
}

bool CWaferGraph::OpenTmpLayer(int Current)
{
	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	sprintf_s(buff, ".\\His\\ur\\wafer\\%s\\His%d.hisb", m_sName, Current);
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(buff))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	int nActiveLayer = CWaferEngine::Instance()->GetGraphDoc()->ActiveLayerID();
	int nOldLayerID = nActiveLayer;   // 解决“撤销后，图层设定为最后一个图层”问题

	LoadFile(pTheReader);

	nActiveLayer = nOldLayerID;      // 解决“撤销后，图层设定为最后一个图层”问题

	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;
	return true;
}
bool CWaferGraph::SaveTmpLayer()
{
	CStringA sFileName(".\\His\\ur\\wafer\\");
	sFileName += m_sName ;  
	sFileName += "\\";// MakeSureDirectoryPathExists必须以\结尾
	if (!PathIsDirectoryA(sFileName.GetString()))
	{
		if (!MakeSureDirectoryPathExists(sFileName.GetString()))  //创建多层目录
		{
			return false;
		}
	}

	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	sprintf_s(buff, "%sHis%d.hisb", sFileName, m_uCurrent);
	CFileWriter *pTheWriter = new CFileWriter();
	if (!pTheWriter->Open(buff))
	{
		delete pTheWriter;
		pTheWriter = NULL;
		return false;
	}
	bool bRet = SaveFile(pTheWriter);

	pTheWriter->Close();
	delete pTheWriter;
	pTheWriter = NULL;
	return bRet;
}
bool CWaferGraph::DeleteAllTempFile()
{
	HANDLE hFirstFile = NULL;
	WIN32_FIND_DATAA FindData;

	string sFilePath(".\\His\\ur\\wafer\\");
	sFilePath += m_sName;

	hFirstFile = ::FindFirstFileA((sFilePath + "\\*.*").c_str(), &FindData);
	if (hFirstFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL bRes = true;
	string sFileName;

	while (bRes)
	{
		bRes = ::FindNextFileA(hFirstFile, &FindData);

		if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //发现文件
		{
			sFileName = sFilePath + "\\" + FindData.cFileName;
			::DeleteFileA(sFileName.c_str());
		}
	}
	::FindClose(hFirstFile);

	return true;
}
void CWaferGraph::updateDoc(bool IsActive)
{
	/*CMarkLayer *pTheLayer = NULL;
	for(size_t i=0;i<m_theMarkLayers.size();i++)
	{
	pTheLayer = m_theMarkLayers[i];
	//--pTheLayer->UpdateLayers();
	} */
}

//全局IGraphObject对象升序排序函数
bool lessmark(IGraphObject*  m1, IGraphObject* m2)
{
	return m1->GetID() < m2->GetID();
}
//将文档中的对象按ID的升序排序
void CWaferGraph::SortByIDAsc(void)
{
	//按照ID升序排序
	std::sort(m_theObjects.begin(), m_theObjects.end(), lessmark);
}

bool CWaferGraph::IsLayerMarkable(int32_t iLayer)
{
	CWaferGraphLayer * pLayer = CWaferEngine::Instance()->GetGraphDoc()->GetLayer(iLayer);
	if (NULL == iLayer)
		return false;

	if (!pLayer->IsMarkable())return false;

	bool IsMarkable = false;
	IGraphObject *pTheObj = NULL;
	size_t size = m_theObjects.size();
	for (size_t j = 0;j<size;j++)
	{
		pTheObj = m_theObjects[j];
		if (pTheObj->IsMarkable() && (pTheObj->GetLayer() == iLayer))
		{
			IsMarkable = true;
			break;
		}
	}
	return IsMarkable;
}

bool CWaferGraph::SetObjectContent(const char * sObjName, void * pContent)
{
	if (NULL == sObjName
		|| NULL == pContent)
		return false;
	
	bool bRet = false;
	IGraphObject *pTheObj = NULL;
	size_t size = m_theObjects.size();
	for (size_t j = 0; j<size; j++)
	{
		pTheObj = m_theObjects[j];
		if (!strcmp(sObjName, pTheObj->GetName()))
		{
			if (0 != pTheObj->SetConfigInfo(Object_TextContent, pContent))
				return false;
			bRet = true;
		}
		else {
			bool bDouble;
			if (0 != pTheObj->GetConfigInfo(Object_IsDouble_B, (void*)&bDouble))
				return false;
			if (bDouble) {
				// 双变量设置， 双变量的目的：两个变量位于一行，且长度会变化，变化后整行字符需要居中，或与其他行对齐
				char szDoubleName[512] = { 0 };
				if (0 != pTheObj->GetConfigInfo(Object_Double_TextName, szDoubleName))
					return false;
				if (!strcmp(sObjName, szDoubleName)) {
					if (0 != pTheObj->SetConfigInfo(Object_Double_TextContent, pContent))
						return false;
					bRet = true;
				}
			}
		}
	}
	return bRet;
}

bool CWaferGraph::GetObjectContent(const char * sObjName, void * pContent)
{
	if (NULL == sObjName
		|| NULL == pContent)
		return false;

	bool bRet = false;
	IGraphObject *pTheObj = NULL;
	for (size_t j = 0, size = m_theObjects.size(); j<size; j++)
	{
		pTheObj = m_theObjects[j];
		if (!strcmp(sObjName, pTheObj->GetName()))
		{
			if (0 != pTheObj->GetConfigInfo(Object_TextContent, pContent))
				return false;
			bRet = true;
		}
		else {
			bool bDouble;
			if (0 != pTheObj->GetConfigInfo(Object_IsDouble_B, (void*)&bDouble))
				return false;
			if (bDouble) {
				// 双变量设置， 双变量的目的：两个变量位于一行，且长度会变化，变化后整行字符需要居中，或与其他行对齐
				char szDoubleName[512] = { 0 };
				if (0 != pTheObj->GetConfigInfo(Object_Double_TextName, szDoubleName))
					return false;
				if (!strcmp(sObjName, szDoubleName)) {
					if (0 != pTheObj->GetConfigInfo(Object_Double_TextContent, pContent))
						return false;
					bRet = true;
				}
			}
		}
	}
	return bRet;

}

// 获取图档中所有文本对象的名称/内容/对象
void CWaferGraph::GetAllTextNameAndContent(std::vector<std::string>& vsrefName
	, std::vector<std::string>& vsrefContent, std::vector<std::string>& fontName)
{
	for (size_t j = 0, size = m_theObjects.size(); j < size; j++)
	{
		IGraphObject *pTheObj = m_theObjects[j];
		int nType = pTheObj->GetType();
		if(!(emObjType_TEXT == nType/*
			|| emObjType_AUTO == nType
			|| emObjType_TIME == nType*/))
			 continue;

		char szText[512] = { 0 };
		char chFontName[MAX_TEXT_SIZE] = { 0 };
		if (0 != pTheObj->GetConfigInfo(Object_TextContent, (void*)szText))
			return;
		if (0 != pTheObj->GetConfigInfo(Object_TextFontName, (void*)chFontName))
			return;
		vsrefName.push_back(pTheObj->GetName());
		vsrefContent.push_back(szText);
		fontName.push_back(chFontName);

		// 如果该对象存在双变量，则需要添加到缓冲区
		bool bDouble;
		if (0 != pTheObj->GetConfigInfo(Object_IsDouble_B, (void*)&bDouble))
			return;
		if (bDouble) {
			memset(szText, 0, sizeof szText);
			if (0 != pTheObj->GetConfigInfo(Object_Double_TextContent, szText))
				return;

			char szDoubleName[512] = { 0 };
			if (0 != pTheObj->GetConfigInfo(Object_Double_TextName, szDoubleName))
				return;

			vsrefName.push_back(szDoubleName);
			vsrefContent.push_back(szText);
			fontName.push_back(chFontName);
		}	
	}
}


void CWaferGraph::GetAllTextFontName(std::vector<std::string>& fontName)
{
	for (size_t j = 0, size = m_theObjects.size(); j < size; j++)
	{
		IGraphObject *pTheObj = m_theObjects[j];
		//if (!pTheObj->IsMarkable()) continue;
		int nType = pTheObj->GetType();
		if (!(emObjType_TEXT == nType
			|| emObjType_AUTO == nType
			|| emObjType_TIME == nType))
			continue;

		char chFontName[MAX_TEXT_SIZE] = { 0 };
		if (0 != pTheObj->GetConfigInfo(Object_TextFontName, (void*)chFontName))
			return;

		string tmpName = pTheObj->GetName();
		if (string::npos != tmpName.find("TEXT")) {   //判断是否为变量（名称带有TEXT）
			fontName.push_back(chFontName);
		}
	}
}

bool CWaferGraph::IsExistVarObj()
{
	for (auto & item : m_theObjects)
	{
		if(item->IsVariable())
		{
			return true;
		}
	}
	return false;
}

bool CWaferGraph::IsExist2DBarcode()
{
	for (auto & item : m_theObjects)
	{
		bool b2DBarcode = false;
		item->GetConfigInfo(Object_Is2DBarcode_B, (void*)&b2DBarcode);
		if (b2DBarcode)
			return true;
	}
	return false;
}

bool CWaferGraph::IsExistTwo2DBarcode()
{
	for (auto & item : m_theObjects)
	{
		bool b2DBarcode = false;
		item->GetConfigInfo(Object_Is2DBarcode_B, (void*)&b2DBarcode);
		if (b2DBarcode)
		{
			int i2DBarcodeIndex = 0;
			item->GetConfigInfo(Object_2DBarcode_Index_I, (void*)&i2DBarcodeIndex);
			if (2 == i2DBarcodeIndex) return true;
		}
	}
	return false;
}

bool CWaferGraph::IsExistFirst2DBarcode()
{
	for (auto & item : m_theObjects)
	{
		bool b2DBarcode = false;
		item->GetConfigInfo(Object_Is2DBarcode_B, (void*)&b2DBarcode);
		if (b2DBarcode)
		{
			int i2DBarcodeIndex = 0;
			item->GetConfigInfo(Object_2DBarcode_Index_I, (void*)&i2DBarcodeIndex);
			if (1 == i2DBarcodeIndex) return true;
		}
	}
	return false;
}


bool CWaferGraph::IsExistBarcode()
{
	for (auto & item : m_theObjects)
	{
		if (item->GetType() == emObjType_D2CODE)
			return true;
	}
	return false;
}

bool CWaferGraph::IsExistTextIndex()
{
	for (auto & item : m_theObjects)
	{
		int iTextObjectType = TEXT_OBJECT_NORMAL;
		item->GetConfigInfo(Object_Text_Object_Type_I, (void*)&iTextObjectType);
		if (TEXT_OBJECT_INDEX == iTextObjectType) {
			return true;
		}
	}
	return false;
}

bool CWaferGraph::Replace2DBarcodeData(int index, const char* pContent)
{
	if (nullptr == pContent)
		return false;

	for (auto & item : m_theObjects)
	{
		bool b2DBarcode = false;
		item->GetConfigInfo(Object_Is2DBarcode_B, (void*)&b2DBarcode);
		if (b2DBarcode) {
			int i2DBarcodeIndex = 1;
			item->GetConfigInfo(Object_2DBarcode_Index_I, (void*)&i2DBarcodeIndex);
			if (index == i2DBarcodeIndex)
			{
				if (0 != item->SetConfigInfo(Object_TextContent, (void*)pContent))
					return false;
			}
		}
	}

	return true;
}

bool CWaferGraph::ReplaceTextIndexData(int index)
{
	for (auto & item : m_theObjects)
	{
		if (emObjType_SHAPE != item->GetType())
		{
			int iTextObjectType = TEXT_OBJECT_NORMAL;
			item->GetConfigInfo(Object_Text_Object_Type_I, (void*)&iTextObjectType);
			if (TEXT_OBJECT_INDEX == iTextObjectType) {
				char pContent[256];
				memset(pContent, 0, sizeof pContent);
				sprintf_s(pContent, "%d", index);
				if (0 != item->SetConfigInfo(Object_TextContent, (void*)pContent))
					return false;
			}
		}
	}

	return true;
}

bool CWaferGraph::DeleteObject(IGraphObject * pObject)
{
	if (NULL == pObject)
		return false;

	IGraphObject *p = NULL;
	std::vector<IGraphObject *>::iterator itt;
	for (itt = m_theObjects.begin(); itt != m_theObjects.end();)
	{
		p = *itt;
		if (p == pObject)
		{
			itt = m_theObjects.erase(itt);
			deleteTextAlign(p);   // 删除对齐关系
			delete p;
			p = NULL;
			break;
		}
		else
			itt++;
	}

	updateRange();

	return true;
}

void CWaferGraph::DeleteObjectByLayer(int32_t iLayerID)
{
	if (iLayerID < 0)
		return;

	IGraphObject *pTheObject = NULL;
	for (auto itt = m_theObjects.begin(); itt != m_theObjects.end();)
	{
		pTheObject = (IGraphObject *)*itt;
		if (pTheObject->GetLayer() == iLayerID)
		{
			itt = m_theObjects.erase(itt);
			deleteTextAlign(pTheObject);   // 删除对齐关系
			delete pTheObject;
			pTheObject = NULL;
		}
		else
			itt++;
	}

	updateRange(); 
}

void CWaferGraph::deleteTextAlign(IGraphObject * pObject)
{
	if (NULL == pObject)
		return;
	
	IGraphObject * pBase = NULL, *pAlign = NULL;
	for (auto itt = m_mapAlign.begin(); itt != m_mapAlign.end();)
	{
		pBase = itt->first;
		pAlign = itt->second.pObject;
		// 只要基准对象和对齐对象任何一个与pObject相等，都要被删除
		if (pBase == pObject || pAlign == pObject)
			itt = m_mapAlign.erase(itt);
		else
			itt++;
	}

}

CWaferGraph* CWaferGraph::ChangeWaferDirAndClone(int iDeg)
{
	CWaferGraph* pWaferGraph = new CWaferGraph(m_pPlatform);
	for (auto& item : m_theObjects)
	{
		IGraphObject* pCloneObject = item->Clone();
		pWaferGraph->AppendObject(pCloneObject);
	}
	pWaferGraph->UnSelectAllObjects();
	// 宽和高
	if (90 == iDeg || 270 == iDeg)
	{
		pWaferGraph->SetHeight(GetWidth());
		pWaferGraph->SetWidth(GetHeight());
	}
	else
	{
		pWaferGraph->SetHeight(GetHeight());
		pWaferGraph->SetWidth(GetWidth());
	}
	// 旋转所有的对象
	pWaferGraph->Rotate(0, 0, DEG2RAD(iDeg), 0);
	return pWaferGraph;
}

/////////////////////////////////////////////////////////////////////////////
/// CWaferGraphDoc 定义
CWaferGraphDoc::CWaferGraphDoc() :
	m_nActiveWafer(-1)
{
    m_pOwnerWnd = NULL;
	//AppendLayer();
	//SetActiveLayer(0);
}


CWaferGraphDoc::~CWaferGraphDoc()
{
	DeleteAllLayer();
	RemoveAllWafer();
}

void CWaferGraphDoc::SetName(const char * s)
{
	memset(m_sName, 0, sizeof m_sName);
	strcpy_s(m_sName, sizeof(m_sName), s);
}

const char * CWaferGraphDoc::GetName(void) const
{
	return m_sName;
}

bool CWaferGraphDoc::LoadFile(IFileReader *pTheReader)
{
	// 清除所有的层和文档
	DeleteAllLayer();
	RemoveAllWafer();
	SetStripMap(false);

	char sActivePenName[MAX_TXT_SIZE] = {0};
	bool bRet = true;
	int iDocCount = 0;
	CWaferGraph *pWafer = NULL;
	uint32_t code;
	while (CN_WAFER_DOC_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_WAFERDOC_ACTIVELAYER_NAME:
				pTheReader->GetString(sActivePenName);
				break;
			case CN_WAFERDOC_LAYER:
			{
				CWaferGraphLayer * pTheLayer = AppendLayer();
				if (!pTheLayer->LoadFile(pTheReader))
					bRet = false;

				if (!LoadParamFromDataBase(pTheLayer->GetPen())) {
					bRet = false;
					//CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_ERROR, "获取激光参数库中的激光参数[%s]失败！", pTheLayer->GetPen()->sName);
				}	
			}
			break;
			case CN_WAFERDOC_DOCCNT:
				iDocCount = pTheReader->GetInt32();
				break;
			case CN_WAFERDOC_DOC:
			{				
				pWafer = AppendWafer("");
				if (!pWafer->LoadFile(pTheReader))
					bRet = false;
				pWafer->DeleteAllTempFile();  // 清除该文档对应所有历史文件
				pWafer->AddUndoList();   // 将初始档案添加到撤销队列中
				break;
			}
			case CN_WAFERDOC_STRIP_MAP:
				SetStripMap(pTheReader->GetBool());
				break;
			default:
				break;
			}
		}
	}

	if (NULL == SetActiveLayer(sActivePenName))
		SetActiveLayer(0);

	return bRet;
}

bool CWaferGraphDoc::LoadFileWithoutPen(IFileReader *pTheReader, std::vector<string> & vsPenName)
{
	// 清除所有的文档
	RemoveAllWafer();
	SetStripMap(false);

	char sActivePenName[MAX_TXT_SIZE] = { 0 };
	bool bRet = true;
	int iDocCount = 0;
	CWaferGraph *pWafer = NULL;
	uint32_t code;
	while (CN_WAFER_DOC_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_WAFERDOC_ACTIVELAYER_NAME:
			{
				pTheReader->GetString(sActivePenName);
				vsPenName.push_back(sActivePenName);   // 多个激光参数
				break;
			}
			case CN_WAFERDOC_LAYER:   
			{
				// 层信息加载后，删除
				CWaferGraphLayer * pTheLayer = AppendLayer();
				pTheLayer->LoadFile(pTheReader);
				DeleteLayer(pTheLayer);
				break;
			}
			case CN_WAFERDOC_DOCCNT:
				iDocCount = pTheReader->GetInt32();
				break;
			case CN_WAFERDOC_DOC:
			{
				pWafer = AppendWafer("");
				if (!pWafer->LoadFile(pTheReader))
					bRet = false;
				pWafer->DeleteAllTempFile();  // 清除该文档对应所有历史文件
				pWafer->AddUndoList();        // 将初始档案添加到撤销队列中
				break;
			}
			case CN_WAFERDOC_STRIP_MAP:
				SetStripMap(pTheReader->GetBool());
				break;
			default:
				break;
			}
		}
	}
	return bRet;
}

bool CWaferGraphDoc::SaveFile(IFileWriter *pTheWriter)
{
	if (NULL == pTheWriter)
		return false;

	pTheWriter->WriteBegin(CN_WAFER_DOC_BASE);
	
	// 保存所有层对象
	for (auto item : m_theLayers)
	{
		pTheWriter->WriteString(CN_WAFERDOC_ACTIVELAYER_NAME, item->GetPen()->sName);
	}

	for (auto & item : m_theLayers)
	{
		pTheWriter->WriteInt32(CN_WAFERDOC_LAYER, 0);
		item->SaveFile(pTheWriter);
	}

	int iDocCount = GetWaferCount();
	pTheWriter->WriteInt32(CN_WAFERDOC_DOCCNT, iDocCount);
	// 保存文档内容
	CWaferGraph *pWafer = NULL;
	for (int i = 0; i < iDocCount; i++)
	{
		pWafer = GetWafer(i);
		if (NULL == pWafer)
			return false;
		
		pTheWriter->WriteInt32(CN_WAFERDOC_DOC, 0);
		pWafer->SaveFile(pTheWriter);
	}

	pTheWriter->WriteBool(CN_WAFERDOC_STRIP_MAP, GetStripMap());

	pTheWriter->WriteEnd(CN_WAFER_DOC_BASE);
	
	return true;

}

CWaferGraph * CWaferGraphDoc::AppendWafer(const char *sDocName)
{
	if (NULL == sDocName)
		return NULL;
	CMarkPlatform * pPlatform = CMarkPlatform::Instance();
	CWaferGraph * pWafer = new CWaferGraph(pPlatform);
	pWafer->SetName(sDocName);
	
	pWafer->DeleteAllTempFile();  // 清除该文档对应所有历史文件
	

	m_vWafterGraphs.push_back(pWafer);

	return pWafer;
}

int CWaferGraphDoc::GetWaferCount(void) const
{
	return m_vWafterGraphs.size();
}

CWaferGraph * CWaferGraphDoc::GetActiveWafer(void)
{
	if (m_nActiveWafer < 0 || m_nActiveWafer > m_vWafterGraphs.size() - 1)
		return NULL;

	return m_vWafterGraphs[m_nActiveWafer];
}

CWaferGraph * CWaferGraphDoc::GetWafer(uint32_t nID)
{
	if (nID < 0 || nID >= m_vWafterGraphs.size())
		return NULL;

	return m_vWafterGraphs[nID];
}

CWaferGraph *  CWaferGraphDoc::SetActiveWafer(int nID)
{
	if (nID < 0 || nID >= m_vWafterGraphs.size())
	{
		m_nActiveWafer = -1;
		return  NULL;
	}

	m_nActiveWafer = nID;

	return m_vWafterGraphs[nID];
}

void CWaferGraphDoc::RemoveWafer(uint32_t iDocID)
{
	int iDocCount = m_vWafterGraphs.size();
	if ((iDocID < 0) || (iDocID >= iDocCount))
		return;

	CWaferGraph *pTheDoc = NULL;
	std::vector<CWaferGraph *>::iterator itt;
	int32_t iDocIndex = 0;
	for (itt = m_vWafterGraphs.begin(); itt != m_vWafterGraphs.end();)
	{
		pTheDoc = (CWaferGraph *)*itt;
		if (iDocIndex == iDocID)
		{
			itt = m_vWafterGraphs.erase(itt);
			delete pTheDoc;
			pTheDoc = NULL;
			break;
		}
		else
			itt++;
		iDocIndex++;
	}
}

void CWaferGraphDoc::RemoveWafer(const char *sDocName)
{
	int iDocCount = m_vWafterGraphs.size();
	if (iDocCount < 1)
		return;

	CWaferGraph *pTheDoc = NULL;
	std::vector<CWaferGraph *>::iterator itt;
	int32_t iDocIndex = 0;
	const char *sName = NULL;
	for (itt = m_vWafterGraphs.begin(); itt != m_vWafterGraphs.end();)
	{
		pTheDoc = (CWaferGraph *)*itt;
		if (0 == strcmp(pTheDoc->GetName(), sDocName))
		{
			itt = m_vWafterGraphs.erase(itt);
			delete pTheDoc;
			pTheDoc = NULL;
			break;
		}
		else
			itt++;
		iDocIndex++;
	}
}

void CWaferGraphDoc::RemoveAllWafer(void)
{
	int iDocCount = m_vWafterGraphs.size();
	if (iDocCount < 1)
		return;

	CWaferGraph *pTheDoc = NULL;
	std::vector<CWaferGraph *>::iterator itt;
	for (itt = m_vWafterGraphs.begin(); itt != m_vWafterGraphs.end(); itt++)
	{
		pTheDoc = (CWaferGraph *)*itt;
		delete pTheDoc;
		pTheDoc = NULL;
	}
	m_vWafterGraphs.clear();
}

/////////////////////////////////////////////////////
///图层操作
bool CWaferGraphDoc::GetModified() const
{
	bool bModified = false;
	for (auto & item : m_vWafterGraphs)
	{
		bModified = item->GetModified();
		if (bModified) break;
	}

	return bModified || m_IsModified;
}

void CWaferGraphDoc::SetModified(bool fModify)
{
	m_IsModified = fModify;

	// 设置每个芯片模块的标志信息
	for (auto & item : m_vWafterGraphs)
	{
		item->SetModified(fModify, false);
	}
}

int32_t CWaferGraphDoc::GetLayerCount() const
{
	return m_theLayers.size();
}

CWaferGraphLayer *CWaferGraphDoc::AppendLayer(void)
{
	CMarkPlatform * pPlatform = CMarkPlatform::Instance();

	CWaferGraphLayer * pTheLayer = new CWaferGraphLayer();

	int iLayerID = m_theLayers.size();
	char sName[256];
	sprintf_s(sName, "layer%d", iLayerID);
	pTheLayer->SetName(sName);
	IMarkPen *pNewPen = pPlatform->MarkDevice()->CreatePen();
	sprintf_s(pNewPen->sName, 255, "pen_%d", iLayerID);
	pNewPen->dwColor = g_clLayerColor[iLayerID % 7];
	pTheLayer->SetPen(pNewPen);
	m_theLayers.push_back(pTheLayer);

	return pTheLayer;
}

bool CWaferGraphDoc::DeleteLayer(int32_t iLayerID)
{
	int32_t iLayerIndex = 0;
	for (auto itt = m_theLayers.begin(); itt != m_theLayers.end();)
	{
		if (iLayerIndex == iLayerID)
		{
			CWaferGraphLayer * pTheLayer = (CWaferGraphLayer *)*itt;
			itt = m_theLayers.erase(itt);
			delete pTheLayer;
			break;
		}
		else
			itt++;
		iLayerIndex++;
	}

	// 重新设置活动层
	if (0 == m_theLayers.size())
	{
		//AppendLayer();
		//m_nActiveLayer = 0;
		m_nActiveLayer = -1;
	}
	else
	{
		m_nActiveLayer--;
		if (m_nActiveLayer < 0)
			m_nActiveLayer = 0;
	}

#if 0  // 只支持单层
	// 将所有对象设置为活动层
	UpdateObjectLayerID();
#else  // 支持多层
	// 更新对象的层ID, 并删除对应层ID的对象
	UpdateObjectLayerID(iLayerID);
#endif

	return true;
}

bool CWaferGraphDoc::DeleteLayer(CWaferGraphLayer * pLayer)
{
	if (nullptr == pLayer)
		return false;

	int nDelLayerID = 0;
	for (auto itt = m_theLayers.begin(); itt != m_theLayers.end();)
	{
		if (*itt == pLayer)
		{
			CWaferGraphLayer * pTheLayer = (CWaferGraphLayer *)*itt;
			itt = m_theLayers.erase(itt);
			delete pTheLayer;
			break;
		}
		else
			itt++;

		nDelLayerID++;
	}

	// 重新设置活动层
	if (0 == m_theLayers.size())
	{
		//AppendLayer();
		//m_nActiveLayer = 0;
		m_nActiveLayer = -1;
	}
	else
	{
		if (m_nActiveLayer >= nDelLayerID)
			m_nActiveLayer--;
	}

#if 0  // 只支持单层
	// 将所有对象设置为活动层
	UpdateObjectLayerID();
#else  // 支持多层
	// 更新对象的层ID, 并删除对应层ID的对象
	UpdateObjectLayerID(nDelLayerID);
#endif

	return true;
}

void CWaferGraphDoc::DeleteAllLayer(void)
{
	for (auto & item : m_theLayers)
	{
		delete item;
	}
	m_theLayers.clear();
	m_nActiveLayer = -1;
}

int32_t CWaferGraphDoc::ActiveLayerID(void) const
{
	return m_nActiveLayer;
}

CWaferGraphLayer *CWaferGraphDoc::GetActiveLayer(void)
{
	int32_t iLayerCount = m_theLayers.size();
	if (m_nActiveLayer < 0 || m_nActiveLayer >= iLayerCount)
		return NULL;

	CWaferGraphLayer * pTheLayer = m_theLayers[m_nActiveLayer];

	return pTheLayer;
}
CWaferGraphLayer *CWaferGraphDoc::GetLayer(int32_t iLayerID)
{
	int32_t iLayerCount = m_theLayers.size();
	if (iLayerID < 0 || iLayerID >= iLayerCount)
		return NULL;

	CWaferGraphLayer * pTheLayer = m_theLayers[iLayerID];
	return pTheLayer;
}
CWaferGraphLayer *CWaferGraphDoc::SetActiveLayer(int iLayerID)
{
	int32_t iLayerCount = m_theLayers.size();
	if (iLayerID < 0 || iLayerID >= iLayerCount)
	{
		m_nActiveLayer = -1;
		return NULL;
	}

	m_nActiveLayer = iLayerID;
	CWaferGraphLayer * pTheLayer = m_theLayers[iLayerID];
#if 0  // 单层
	UpdateObjectLayerID();   // 定制，将所有对象的层，改为活动层
#endif

	return pTheLayer;
}
CWaferGraphLayer *CWaferGraphDoc::SetActiveLayer(const char *sPenName)
{
	size_t iLayerCount = m_theLayers.size();

	CWaferGraphLayer * pTheLayer = NULL;
	const char *sName = NULL;
	for (size_t i = 0; i < iLayerCount; i++)
	{
		pTheLayer = m_theLayers[i];
		sName = pTheLayer->GetPen()->sName;   // 使用笔名比较，注意不是层名
		if (0 == strcmp(sName, sPenName))
		{
			m_nActiveLayer = i;
			break;
		}
		else
			pTheLayer = NULL;
	}

#if 0  // 单层
	UpdateObjectLayerID();   // 定制，将所有对象的层，改为活动层
#endif

	return pTheLayer;
}

CWaferGraphLayer * CWaferGraphDoc::SetActiveLayer(CWaferGraphLayer * pLayer)
{
	if (nullptr == pLayer)
		return nullptr;

	int nLayerID = 0;
	for (auto & item : m_theLayers)
	{
		if (pLayer == item)
		{
			m_nActiveLayer = nLayerID;
#if 0  // 单层
			UpdateObjectLayerID();   // 定制，将所有对象的层，改为活动层
#endif
			return item;
		}
		nLayerID++;
	}

	return nullptr;
}

bool CWaferGraphDoc::IsExistLayer(const char *sPenName, int& irefLayerID)
{
	bool bExist = false;
	size_t iLayerCount = m_theLayers.size();
	CWaferGraphLayer * pTheLayer = NULL;
	const char *sName = NULL;
	for (size_t i = 0; i < iLayerCount; i++)
	{
		pTheLayer = m_theLayers[i];
		sName = pTheLayer->GetPen()->sName;   // 使用笔名比较，注意不是层名
		if (0 == strcmp(sName, sPenName))
		{
			irefLayerID = i;
			bExist = true;
			break;
		}
	}

	return bExist;
}

void CWaferGraphDoc::UpdateObjectLayerID()
{
	if (m_nActiveLayer < 0 || m_nActiveLayer >= m_theLayers.size())
		return;

	for (auto & item : m_vWafterGraphs)
	{
		int32_t nSize = item->GetObjCount();
		for (int32_t i = 0; i < nSize; i++)
		{
			IGraphObject * pObject = item->GetObject(i);
			if (nullptr != pObject)
			{
				pObject->SetLayer(m_nActiveLayer);
			}
		}
	}
}

void CWaferGraphDoc::DeleteObjectByLayer(int32_t iLayerID)
{
	if (iLayerID < 0)
		return;

	for (auto & item : m_vWafterGraphs)
	{
		item->DeleteObjectByLayer(iLayerID);
	}

}

void CWaferGraphDoc::UpdateObjectLayerID(int32_t iLayerID)
{
	if (iLayerID < 0)
		return;

	// 更新比层iLayerID大的对象的层ID，直接减一即可
	for (auto & item : m_vWafterGraphs)
	{
		int32_t nSize = item->GetObjCount();
		for (int32_t i = 0; i < nSize; i++)
		{
			IGraphObject * pObject = item->GetObject(i);
			if (nullptr != pObject)
			{
				int32_t nOldLayer = pObject->GetLayer();
				if (nOldLayer > iLayerID)
					pObject->SetLayer(--nOldLayer);
			}
		}
	}
}

BOOL CWaferGraphDoc::IsLayerUsed(int32_t iLayerID)
{
	for (auto & item : m_vWafterGraphs)
	{
		int32_t nSize = item->GetObjCount();
		for (int32_t i = 0; i < nSize; i++)
		{
			IGraphObject * pObject = item->GetObject(i);
			if (nullptr != pObject)
			{
				int32_t nObjLayer = pObject->GetLayer();
				if (nObjLayer == iLayerID)
					return TRUE;
			}
		}
	}
	return FALSE;
}

bool CWaferGraphDoc::IsExist2DBarcode()
{
	for (auto & item : m_vWafterGraphs){
		if (item->IsExist2DBarcode())
			return true;
	}
	return false;
}

bool CWaferGraphDoc::IsExistTwo2DBarcode()
{
	for (auto & item : m_vWafterGraphs) {
		if (item->IsExistTwo2DBarcode())
			return true;
	}
	return false;
}

bool CWaferGraphDoc::IsExistFirst2DBarcode()
{
	for (auto & item : m_vWafterGraphs) {
		if (item->IsExistFirst2DBarcode())
			return true;
	}
	return false;
}

bool CWaferGraphDoc::IsExistBarcode()
{
	for (auto & item : m_vWafterGraphs) {
		if (item->IsExistBarcode())
			return true;
	}
	return false;
}

bool CWaferGraphDoc::LoadParamFromDataBase(IMarkPen* pMarkPen)
{
	if (nullptr == pMarkPen)
		return false;

	if (0 != CMarkPlatform::Instance()->MarkDevice()->GetDeviceCtrl(Laser_GetPenDatabaseParam_P, (void*)pMarkPen)) {
		return false;
	}
	return true;
}

bool CWaferGraphDoc::UpdateAllLayerFromDB()
{
	for (auto & item : m_theLayers)
	{
		if (!LoadParamFromDataBase(item->GetPen()))
			return false;
	}
	return true;
}

// 获取所有变量文本对象的字体名称
void CWaferGraphDoc::GetAllTextFontName(std::vector<std::string>& fontName)
{
	for (auto & item : m_vWafterGraphs)
	{
		item->GetAllTextFontName(fontName);
	}
}

bool CWaferGraphDoc::GetAllParamValue(std::vector<CString>& sParamName)
{
	int32_t nLayerC = GetLayerCount();
	for (int i = 0; i < nLayerC; i++)
	{
		CWaferGraphLayer * pLayer = GetLayer(i);
		IMarkPen * pMarkPen = pLayer->GetPen();
		if (IsLayerUsed(i))
		{
			sParamName.push_back(CString(pMarkPen->sName));
		}
	}

	return true;
}