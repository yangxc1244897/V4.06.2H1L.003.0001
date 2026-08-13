#include "stdafx.h"
#include "WaferMapDoc.h"
#include "WaferEngine.h"
#include "WaferFile.h"
#include "MarkPlatform.h"
#include "CoordTransform.h"

//#include "DlgMapPos.h"
// 标记线程
struct stMarkThreadParam_t
{
	bool bPenOrder;
	CWaferGroup * pGroup;
	IMarkEngine * pMarkEngine;
	vector<xWorkItem_t> vWorkItem;
	const CString* p2DData;
	const CString* p2DData2;
};
unsigned int __stdcall MarkThread(void *);

void _WaferMeta_t::Draw(IGraphRender *pTheRender, bool bDrawPin, bool bRenderBK, bool bMapMode)
{
	// 绘制芯片模块主框架
	int nPenStyle = PS_SOLID; 
	int nWidth = 1;
	COLORREF crColor = pWaferGroup->dwColor;
	CBrush brush(RGB(120, 120, 120));
	if (bMapMode) {
		// 根据Bin设置背景色
		//if (CWaferEngine::Instance()->GetGraphDoc()->GetWaferCount() > 1) 
		{
			if (0 != m_nWaferID) {  // 一般Bin0为正常内容，其余为不标记内容
				static COLORREF clrBinInfo[] = { RGB(0, 0, 0), RGB(0, 0, 255), RGB(0, 255, 255),
					RGB(255, 0, 255), RGB(255, 165, 0), RGB(0, 0, 255), RGB(0, 255, 255), RGB(0, 0, 0) };
				brush.DeleteObject();
				brush.CreateSolidBrush(clrBinInfo[m_nWaferID]);
				bRenderBK = true;
			}
		}
	}
	if (IsSel())
	{
		brush.DeleteObject();
		brush.CreateSolidBrush(RGB(255, 0, 0));
		bRenderBK = true;
	}
	bool bMark = IsMarkable();
	if (!bMark)
	{
		if (IsSel())
			crColor = RGB(255, 0, 0);
		nPenStyle = PS_DOT;
		brush.DeleteObject();
		brush.CreateStockObject(NULL_BRUSH);
	}
	pTheRender->BeginPaint(nPenStyle, nWidth, crColor);
	pTheRender->SetBrush(&brush);
	DPoint_t dp[5];
	dp[4] = dp[0] = DPoint_t(xDim.dimPoints1[0].x, xDim.dimPoints1[0].y);  // 第一点和第五点，左上
	dp[1] = DPoint_t(xDim.dimPoints1[2].x, xDim.dimPoints1[2].y);     // 第二点，右上
	dp[2] = DPoint_t(xDim.dimPoints1[2].x - xDim.dimPoints1[0].x + xDim.dimPoints1[1].x, 
		xDim.dimPoints1[2].y - xDim.dimPoints1[0].y + xDim.dimPoints1[1].y);   // 第三点，右下
	dp[3] = DPoint_t(xDim.dimPoints1[1].x, xDim.dimPoints1[1].y);    // 第四点， 左下
	double dbR = min(xDim.dimPoints1[2].x - xDim.dimPoints1[0].x, 
		xDim.dimPoints1[0].y - xDim.dimPoints1[1].y);  //定位孔半径默认值
	pTheRender->DrawPolygon(dp, 5, bRenderBK ? bMark : false);   // 填充时，无法绘制边
	
	pTheRender->EndPaint();

	if (bDrawPin)
	{
		// 绘制芯片的所属头，和方位圆
		int nHead = GetHead();
		COLORREF clrHead;
		if (0 == nHead)   // 未分配
		{
			clrHead = RGB(255, 255, 255);
		}
		else if (1 == nHead)   // 主头
		{
			clrHead = RGB(255, 0, 0);
		}
		else if (2 == nHead)  //从头
		{
			//clrHead = RGB(30, 144, 255);
			clrHead = RGB(0, 0, 255);
		}
		else    // 双头
		{
			clrHead = RGB(255, 0, 255);
		}
		pTheRender->BeginPaint(PS_SOLID, 3, clrHead);
		CBrush brushHead(RGB(220, 220, 220));
		pTheRender->SetBrush(&brushHead);
		// 定位孔半径，采用芯片高和宽中最小长度的十分之一
		dbR = min(pWaferGroup->GetCellHeight(), pWaferGroup->GetCellWidth()) / 10;
		pTheRender->DrawCircle(xDim.dimPoints1[3].x, xDim.dimPoints1[3].y, dbR);
		pTheRender->EndPaint();
	}

	// 绘制BIN号
	if(bMapMode) DrawBinInfo(pTheRender);
}

void _WaferMeta_t::DrawBinInfo(IGraphRender* pGraphRender)
{
	if (CWaferEngine::Instance()->GetGraphDoc()->GetWaferCount() > 1) {
		if (0 != m_nWaferID) {  // Bin0为正常内容，其余为不标记内容
			static COLORREF clrBinInfo[] = { RGB(128, 0, 0), RGB(0, 128, 0), RGB(0, 0, 128),
				RGB(128, 128, 0), RGB(128, 0, 128), RGB(0, 128, 128), RGB(128, 128, 255), RGB(0, 0, 0) };
			pGraphRender->BeginPaint(PS_SOLID, 1, RGB(0, 0, 0));
			pGraphRender->SetTextStyle(RGB(255, 255, 255));
			FField_t field;
			xDim.GetField(field);
			CStringA s;
			s.Format("%d", m_nWaferID);
			pGraphRender->DrawText(s.GetString(), field, DT_CENTER);
			pGraphRender->EndPaint();
		}
	}
}

/*********************************************************************/
void CWaferGroup::SetArrayInfo(const xArray_t & xArray)
{
	m_ArrayInfo = xArray;
}

void CWaferGroup::GetArrayInfo(xArray_t & xArray)
{
	xArray = m_ArrayInfo;
}

bool CWaferGroup::LoadFile(IFileReader *pTheReader)
{
    if (NULL == pTheReader)
        return false;

    uint32_t code;
    char chTmp[MAX_TXT_SIZE];
    int iFlag = 0;
    xWaferMeta_t WaferMetasTmp;
    while (CN_WAFERGROUP_BASE == pTheReader->ReadData(&code))
    {
        if (CODED_VERIFY(code))
        {
            break;
        }
        else
        {
            switch (code)
            {
            case CN_WAFERGROUP_NAME:
                memset(sName, 0, sizeof sName);
                pTheReader->GetString(sName);
                break;
            case CN_WAFERGROUP_COLOR:
                dwColor = pTheReader->GetInt32();
                break;
			case CN_WAFERGROUP_CELL_H:
				m_ArrayInfo.cell.dCellHight = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_CELL_W:
				m_ArrayInfo.cell.dCellWidth = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_CELL_ROWCOUNT:
				m_ArrayInfo.cell.nRowCount = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_CELL_COLCOUNT:
				m_ArrayInfo.cell.nColCount = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_CELL_ARRAYSTYLE:
				m_ArrayInfo.cell.nStyle = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_CELL_DEG:
				m_ArrayInfo.cell.nWaferDeg = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_CELL_ROWPITCH:
				m_ArrayInfo.cell.dbRowPitch = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_CELL_COLPITCH:
				m_ArrayInfo.cell.dbColPitch = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_CELL_COLPITCH_CS:
				m_ArrayInfo.cell.dbColPitchCS = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_BASE_H:
				m_ArrayInfo.group.dbGroupH = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_BASE_W:
				m_ArrayInfo.group.dbGroupW = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_COUNT:
				m_ArrayInfo.group.nGroupCount = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_DEG:
				m_ArrayInfo.group.dbGroupDeg = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_CX:
				m_ArrayInfo.group.dbCenterX = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_CY:
				m_ArrayInfo.group.dbCenterY = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_GROUP_PITCH:
				m_ArrayInfo.group.dbGroupPitch = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_GROUP_IL_COUNT_X:
				m_ArrayInfo.group.nIterleaveCountX = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_GROUP_IL_INDENT_X:
				m_ArrayInfo.group.dbIterleaveIndentX = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_GROUP_IL_DEG_X:
				m_ArrayInfo.group.nIterleaveDegX = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_GROUP_IL_COUNT_Y:
				m_ArrayInfo.group.nIterleaveCountY = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_GROUP_IL_INDENT_Y:
				m_ArrayInfo.group.dbIterleaveIndentY = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_GROUP_IL_DEG_Y:
				m_ArrayInfo.group.nIterleaveDegY = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_GROUP_IL_STYLE:
				m_ArrayInfo.group.nIndentStyle = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_GROUP_COUNT_VER:
				m_ArrayInfo.group.nGroupCountVer = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_GROUP_PITCH_VER:
				m_ArrayInfo.group.dbGroupPitchVer = pTheReader->GetDouble();
				break;
			case CN_WAFERGROUP_WAFERID:
				WaferMetasTmp.m_nWaferID = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_NFLAG:
				WaferMetasTmp.nFlag = pTheReader->GetInt32();
				WaferMetasTmp.EnableSel(false);  // 取消选中
				break;
			case CN_WAFERGROUP_COL:
				WaferMetasTmp.nCol = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_ROW:
				WaferMetasTmp.nRow = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_BLOCKID:
				WaferMetasTmp.m_nBlockID = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_BLOCKID_VER:
				WaferMetasTmp.m_nBlockIDVer = pTheReader->GetInt32();
				break;
			case CN_WAFERGROUP_POINTX:
				if (iFlag < 3)
				{
					WaferMetasTmp.xDim.dimPoints0[iFlag].x = pTheReader->GetDouble();
				}
				else
				{
					WaferMetasTmp.xDim.dimPoints1[iFlag - 3].x = pTheReader->GetDouble();
				}
				break;
			case CN_WAFERGROUP_POINTY:
				if (iFlag < 3)
				{
					WaferMetasTmp.xDim.dimPoints0[iFlag].y = pTheReader->GetDouble();
					iFlag += 1;
				}
				else
				{
					WaferMetasTmp.xDim.dimPoints1[iFlag - 3].y = pTheReader->GetDouble();
					iFlag += 1;
					if (iFlag == 7)
					{
						WaferMetasTmp.pWaferGroup = this;
						vWaferMetas.push_back(WaferMetasTmp);
						iFlag = 0;
					}
				}
				break;
			default: break;
			}
		}
	}
	return true;
}
bool CWaferGroup::SaveFile(IFileWriter *pTheWriter)
{
	if (NULL == pTheWriter) return false;

	pTheWriter->WriteBegin(CN_WAFERGROUP_BASE);    //Doc开始标志

	// Group属性
	pTheWriter->WriteString(CN_WAFERGROUP_NAME, sName);
	pTheWriter->WriteDouble(CN_WAFERGROUP_CELL_H, m_ArrayInfo.cell.dCellHight);
	pTheWriter->WriteDouble(CN_WAFERGROUP_CELL_W, m_ArrayInfo.cell.dCellWidth);
	pTheWriter->WriteInt32(CN_WAFERGROUP_CELL_ROWCOUNT, m_ArrayInfo.cell.nRowCount);
	pTheWriter->WriteInt32(CN_WAFERGROUP_CELL_COLCOUNT, m_ArrayInfo.cell.nColCount);
	pTheWriter->WriteInt32(CN_WAFERGROUP_CELL_ARRAYSTYLE, m_ArrayInfo.cell.nStyle);
	pTheWriter->WriteInt32(CN_WAFERGROUP_CELL_DEG, m_ArrayInfo.cell.nWaferDeg);
	pTheWriter->WriteDouble(CN_WAFERGROUP_CELL_ROWPITCH, m_ArrayInfo.cell.dbRowPitch);
	pTheWriter->WriteDouble(CN_WAFERGROUP_CELL_COLPITCH, m_ArrayInfo.cell.dbColPitch);
	pTheWriter->WriteDouble(CN_WAFERGROUP_CELL_COLPITCH_CS, m_ArrayInfo.cell.dbColPitchCS);

	pTheWriter->WriteInt32(CN_WAFERGROUP_COLOR, dwColor);
	pTheWriter->WriteDouble(CN_WAFERGROUP_BASE_H, m_ArrayInfo.group.dbGroupH);
	pTheWriter->WriteDouble(CN_WAFERGROUP_BASE_W, m_ArrayInfo.group.dbGroupW);
	pTheWriter->WriteInt32(CN_WAFERGROUP_COUNT, m_ArrayInfo.group.nGroupCount);
	pTheWriter->WriteDouble(CN_WAFERGROUP_DEG, m_ArrayInfo.group.dbGroupDeg);
	pTheWriter->WriteDouble(CN_WAFERGROUP_CX, m_ArrayInfo.group.dbCenterX);
	pTheWriter->WriteDouble(CN_WAFERGROUP_CY, m_ArrayInfo.group.dbCenterY);
	pTheWriter->WriteDouble(CN_WAFERGROUP_GROUP_PITCH, m_ArrayInfo.group.dbGroupPitch);
	pTheWriter->WriteInt32(CN_WAFERGROUP_GROUP_IL_COUNT_X, m_ArrayInfo.group.nIterleaveCountX);
	pTheWriter->WriteDouble(CN_WAFERGROUP_GROUP_IL_INDENT_X, m_ArrayInfo.group.dbIterleaveIndentX);
	pTheWriter->WriteInt32(CN_WAFERGROUP_GROUP_IL_DEG_X, m_ArrayInfo.group.nIterleaveDegX);
	pTheWriter->WriteInt32(CN_WAFERGROUP_GROUP_IL_COUNT_Y, m_ArrayInfo.group.nIterleaveCountY);
	pTheWriter->WriteDouble(CN_WAFERGROUP_GROUP_IL_INDENT_Y, m_ArrayInfo.group.dbIterleaveIndentY);
	pTheWriter->WriteInt32(CN_WAFERGROUP_GROUP_IL_DEG_Y, m_ArrayInfo.group.nIterleaveDegY);
	pTheWriter->WriteInt32(CN_WAFERGROUP_GROUP_IL_STYLE, m_ArrayInfo.group.nIndentStyle);
	pTheWriter->WriteInt32(CN_WAFERGROUP_GROUP_COUNT_VER, m_ArrayInfo.group.nGroupCountVer);
	pTheWriter->WriteDouble(CN_WAFERGROUP_GROUP_PITCH_VER, m_ArrayInfo.group.dbGroupPitchVer);


	// Meta属性
	for (size_t i = 0; i < vWaferMetas.size(); i++)
	{
		pTheWriter->WriteInt32(CN_WAFERGROUP_WAFERID, vWaferMetas[i].m_nWaferID);
		pTheWriter->WriteInt32(CN_WAFERGROUP_NFLAG, vWaferMetas[i].nFlag);
		pTheWriter->WriteInt32(CN_WAFERGROUP_ROW, vWaferMetas[i].nRow);
		pTheWriter->WriteInt32(CN_WAFERGROUP_COL, vWaferMetas[i].nCol);
		pTheWriter->WriteInt32(CN_WAFERGROUP_BLOCKID, vWaferMetas[i].m_nBlockID);
		pTheWriter->WriteInt32(CN_WAFERGROUP_BLOCKID_VER, vWaferMetas[i].m_nBlockIDVer);
		for (int j = 0; j < 3; j++)
		{
			pTheWriter->WriteDouble(CN_WAFERGROUP_POINTX, vWaferMetas[i].xDim.dimPoints0[j].x);
			pTheWriter->WriteDouble(CN_WAFERGROUP_POINTY, vWaferMetas[i].xDim.dimPoints0[j].y);
		}
		for (int k = 0; k < 4; k++)
		{
			pTheWriter->WriteDouble(CN_WAFERGROUP_POINTX, vWaferMetas[i].xDim.dimPoints1[k].x);
			pTheWriter->WriteDouble(CN_WAFERGROUP_POINTY, vWaferMetas[i].xDim.dimPoints1[k].y);
		}
	}
	pTheWriter->WriteEnd(CN_WAFERGROUP_BASE);
	return true;
}
void CWaferGroup::Draw(IGraphRender *pTheRender, const FField_t & fClient, bool bDrawPin)
{
	// 绘制底板
	pTheRender->BeginPaint(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brush(RGB(135, 206, 235));
	pTheRender->SetBrush(&brush);
	DPoint_t dp[5];

	double dbHalfW = m_ArrayInfo.group.dbGroupW / 2;
	double dbHalfH = m_ArrayInfo.group.dbGroupH / 2;
	double cx = m_ArrayInfo.group.dbCenterX;
	double cy = m_ArrayInfo.group.dbCenterY;
	dp[4] = dp[0] = DPoint_t(-dbHalfW + cx, dbHalfH + cy);
	dp[1] = DPoint_t(dbHalfW + cx, dbHalfH + cy);
	dp[2] = DPoint_t(dbHalfW + cx, -dbHalfH + cy);
	dp[3] = DPoint_t(-dbHalfW + cx, -dbHalfH + cy);
	for (auto & item : dp)
	{
		item.rotate(cx, cy, DEG2RAD(m_ArrayInfo.group.dbGroupDeg));
	}
	pTheRender->DrawPolygon(dp, 5, true);   // 填充时，无法绘制边框

	pTheRender->EndPaint();

	// 绘制芯片
	size_t nWaferSize = vWaferMetas.size();
	if (nWaferSize < 1)
	{
		return;
	}
	FField_t fWafer;
	bool bRenderBK = nWaferSize > 2000 ? false : true;
	for (size_t j = 0; j < nWaferSize; j++)
	{
		vWaferMetas[j].xDim.GetField(fWafer);
		if (fWafer.Left > fClient.Right
			|| fWafer.Right < fClient.Left
			|| fWafer.Top < fClient.Bottom
			|| fWafer.Bottom > fClient.Top)
			continue;
		vWaferMetas[j].Draw(pTheRender, bDrawPin, bRenderBK, m_bMapMode);
	}
}

/**
* @brief 单卡双头分光标记线程
* @return 测试成功返回1，测试失败返回0。
*/
unsigned int __stdcall MarkThread(void * p)
{
	int32_t iError(0);
	bool bFirstMark = true;
	stMarkThreadParam_t * pMarkParam = (stMarkThreadParam_t*)p;
	CWaferGroup * pGroup = pMarkParam->pGroup;
	IMarkEngine * pMarkEngine = pMarkParam->pMarkEngine;
	vector<xWorkItem_t> vWorkItems = pMarkParam->vWorkItem;
	bool bPenOrder = pMarkParam->bPenOrder;
	IPlatform * pPlatform = CMarkPlatform::Instance();
	double dbSafePos[2] = { 0 , 0 };
	if (nullptr == pGroup || nullptr == pMarkEngine)
		return 0;

	pMarkEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT, false);

	size_t nWorkSize = vWorkItems.size();
	CWaferGraphDoc * pWaferGraphDoc = CWaferEngine::Instance()->GetGraphDoc();

	int nCycleCount = 1;  // 循环次数，如果有n个参数，且采用“按激光参数顺序标记”模式，则nCycleCount = n
	bool bUpdatePenPerObj = false;  // 是否每次标记对象前都更新激光参数
	int nActiveLayerID = 0;   // 只有一个激光参数被使用时，对象使用的参数ID
	int nLaserPenC = pGroup->GetUsedLaserPenCount(nActiveLayerID);
	if (nLaserPenC <= 0) {
		pPlatform->PrintLog(emLOG_LEVEL_ERROR, "印章中不存在内容");
		return 0;
	}
	if (1 == nLaserPenC) {   // 只有一个激光参数，每次标记前更新一次参数即可
		bUpdatePenPerObj = false;
		nCycleCount = 1;
	}
	else {
		if (bPenOrder) {// 多个激光参数，按激光参数顺序标记，循环标记，每次将所有芯片的相同参数内容打完
			bUpdatePenPerObj = false;
			nCycleCount = nLaserPenC;
		}
		else { // 多个激光参数，按芯片顺序标记，标记每个对象前，更新激光参数
			bUpdatePenPerObj = true;
			nCycleCount = 1;
		}
	}
	for (int nLayerIndex = 0; nLayerIndex < nCycleCount; nLayerIndex++)
	{
		if (false == bUpdatePenPerObj)
		{
			pMarkEngine->MarkBegin();
			// 设置激光参数
			CWaferGraphLayer * pLayer = CWaferEngine::Instance()->GetGraphDoc()->GetLayer((1 == nCycleCount) ? nActiveLayerID : nLayerIndex);
			if (NULL == pLayer) {
				pPlatform->PrintLog(emLOG_LEVEL_ERROR, "激光参数更新失败！");
				pPlatform->Message(emMSG_TYPE_ERROR, "激光参数更新失败！");
				return 0;
			}
			IMarkPen * pPen = pLayer->GetPen();
			if (!pMarkEngine->UpdatePen(pPen)) {
				pPlatform->PrintLog(emLOG_LEVEL_ERROR, "激光参数更新失败！");
				pPlatform->Message(emMSG_TYPE_ERROR, "激光参数更新失败！");
				return 0;
			}
			pMarkEngine->MarkEnd();
		}

		if (0 == CWaferEngine::Instance()->GetMapDoc()->m_iMarkOrder)
		{
			pPlatform->PrintLog(emLOG_LEVEL_INFO, "先打双头，再打单头！");
			// 双头同时标记
			CMarkPlatform::Instance()->MarkDriver()->SetScanHead(1, 1);
			pMarkEngine->MarkBegin();

			for (size_t i = 0; i < nWorkSize; i++)
			{
				if ((vWorkItems[i].nMasterID >= 0) && (vWorkItems[i].nSlaverID >= 0)) //--主从
				{
					//PrintHeadID(vWorkItems[i].nMasterID, vWorkItems[i].nSlaverID);

					//--计算从相对于主的位置
					xDim_t xDimM, xDimS, xDim;
					xDimM = pGroup->vWaferMetas[vWorkItems[i].nMasterID].xDim;
					xDimS = pGroup->vWaferMetas[vWorkItems[i].nSlaverID].xDim;
					CCoordTransform::Instance()->Base2Head(0, xDimM.dimPoints1[0].x, xDimM.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(0, xDimM.dimPoints1[1].x, xDimM.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(0, xDimM.dimPoints1[2].x, xDimM.dimPoints1[2].y);
					CCoordTransform::Instance()->Base2Head(1, xDimS.dimPoints1[0].x, xDimS.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(1, xDimS.dimPoints1[1].x, xDimS.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(1, xDimS.dimPoints1[2].x, xDimS.dimPoints1[2].y);
					xDim.dimPoints0[0] = xDimM.dimPoints1[0];
					xDim.dimPoints0[1] = xDimM.dimPoints1[1];
					xDim.dimPoints0[2] = xDimM.dimPoints1[2];
					xDim.dimPoints1[0] = xDimS.dimPoints1[0];
					xDim.dimPoints1[1] = xDimS.dimPoints1[1];
					xDim.dimPoints1[2] = xDimS.dimPoints1[2];
					xHeadTurn_t xHeadTurn;
					xHeadTurn.dx = 0.0;
					xHeadTurn.dy = 0.0;
					xHeadTurn.cx = 0.0;
					xHeadTurn.cy = 0.0;
					xHeadTurn.angle = 0.0;
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_MASTER_TURN, &xHeadTurn);
					xHeadTurn.dx = xDim.GetOffsetX();
					xHeadTurn.dy = xDim.GetOffsetY();
					xHeadTurn.cx = xDim.dimPoints1[0].x;
					xHeadTurn.cy = xDim.dimPoints1[0].y;
					xHeadTurn.angle = xDim.GetAffineAngle();
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_SLAVER_TURN, &xHeadTurn);

					//--将设计图移至芯片处标记
					xMarkTurn_t xMarkTurn;
					xMarkTurn.dx = xDimM.GetCenterX();
					xMarkTurn.dy = xDimM.GetCenterY();
					xMarkTurn.cx = xMarkTurn.dx;
					xMarkTurn.cy = xMarkTurn.dy;
					xMarkTurn.angle = xDimM.GetRotateAngle();
					pMarkEngine->SetEngineCtrl(emMECMD_MARK_TURN, &xMarkTurn);

					// 获取芯片的模组指针
					int nWaferID = pGroup->vWaferMetas[vWorkItems[i].nMasterID].m_nWaferID;
					CWaferGraph * pWafer = pWaferGraphDoc->GetWafer(nWaferID);
					if (NULL == pWafer) continue;
					iError = pWafer->Mark(pMarkEngine
						, (1 == nCycleCount) ? -1 : nLayerIndex
						, (bUpdatePenPerObj ? MARKFLAG_UPDATEPEN : MARKFLAG_NONUPDATE) | MARKFLAG_ONLY_MARK_NON_VAR);
					if (0 != iError)break;
				}
			}
			if (0 != pMarkEngine->MarkEnd())
				iError = -1;
			if (0 != iError)  goto GO_RESET_PRO;


			// 仅主头标记
			bFirstMark = true;
			for (size_t i = 0; i < nWorkSize; i++)
			{
				if ((vWorkItems[i].nMasterID >= 0) && (vWorkItems[i].nSlaverID < 0))//--仅主
				{
					if (bFirstMark) {
						bFirstMark = false;
						CMarkPlatform::Instance()->MarkDriver()->SetScanHead(1, 0);
						pMarkEngine->MarkBegin();
					}
					xHeadTurn_t xHeadTurn;
					xHeadTurn.dx = 0.0;
					xHeadTurn.dy = 0.0;
					xHeadTurn.cx = 0.0;
					xHeadTurn.cy = 0.0;
					xHeadTurn.angle = 0.0;
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_MASTER_TURN, &xHeadTurn);

					//--计算位置
					xDim_t xDim;
					xDim = pGroup->vWaferMetas[vWorkItems[i].nMasterID].xDim;
					CCoordTransform::Instance()->Base2Head(0, xDim.dimPoints1[0].x, xDim.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(0, xDim.dimPoints1[1].x, xDim.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(0, xDim.dimPoints1[2].x, xDim.dimPoints1[2].y);
					//--将设计图移至芯片处标记
					xMarkTurn_t xMarkTurn;
					xMarkTurn.dx = xDim.GetCenterX();
					xMarkTurn.dy = xDim.GetCenterY();
					xMarkTurn.cx = xMarkTurn.dx;
					xMarkTurn.cy = xMarkTurn.dy;
					xMarkTurn.angle = xDim.GetRotateAngle();
					pMarkEngine->SetEngineCtrl(emMECMD_MARK_TURN, &xMarkTurn);
					// 获取芯片的模组指针
					int nWaferID = pGroup->vWaferMetas[vWorkItems[i].nMasterID].m_nWaferID;
					CWaferGraph * pWafer = pWaferGraphDoc->GetWafer(nWaferID);
					if (NULL == pWafer) continue;
					// 替换2D数据
					if (nullptr != pMarkParam->p2DData) {
						if (!pWafer->Replace2DBarcodeData(1, CStringA(pMarkParam->p2DData[vWorkItems[i].nMasterID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "2D数据替换失败!");
							break;
						}
					}
					// 替换第二组2D数据
					if (nullptr != pMarkParam->p2DData2) {
						if (!pWafer->Replace2DBarcodeData(2, CStringA(pMarkParam->p2DData2[vWorkItems[i].nMasterID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "第二组2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "第二组2D数据替换失败!");
							break;
						}
					}
					// 替换印章中的序号
					if (!pWafer->ReplaceTextIndexData(vWorkItems[i].nMasterID))
					{
						pPlatform->PrintLog(emLOG_LEVEL_ERROR, "序号数据替换失败!");
						pPlatform->Message(emMSG_TYPE_ERROR, "序号数据替换失败!");
						break;
					}
					iError = pWafer->Mark(pMarkEngine
						, (1 == nCycleCount) ? -1 : nLayerIndex
						, (bUpdatePenPerObj ? MARKFLAG_UPDATEPEN : MARKFLAG_NONUPDATE) | vWorkItems[i].uMarkFlag);
					if (0 != iError)break;
				}
			}
			if (!bFirstMark) {
				if (0 != pMarkEngine->MarkEnd())
					iError = -1;
				if (0 != iError)  goto GO_RESET_PRO;
			}

			//仅从头标记
			bFirstMark = true;
			for (size_t i = 0; i < nWorkSize; i++)
			{
				if ((vWorkItems[i].nMasterID < 0) && (vWorkItems[i].nSlaverID >= 0))//--仅从
				{
					if (bFirstMark) {
						bFirstMark = false;
						CMarkPlatform::Instance()->MarkDriver()->SetScanHead(0, 1);
						pMarkEngine->MarkBegin();
					}
					xHeadTurn_t xHeadTurn;
					xHeadTurn.dx = 0.0;
					xHeadTurn.dy = 0.0;
					xHeadTurn.cx = 0.0;
					xHeadTurn.cy = 0.0;
					xHeadTurn.angle = 0.0;
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_SLAVER_TURN, &xHeadTurn);

					//--计算位置
					xDim_t xDim;
					xDim = pGroup->vWaferMetas[vWorkItems[i].nSlaverID].xDim;
					CCoordTransform::Instance()->Base2Head(1, xDim.dimPoints1[0].x, xDim.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(1, xDim.dimPoints1[1].x, xDim.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(1, xDim.dimPoints1[2].x, xDim.dimPoints1[2].y);
					//--将设计图移至芯片处标记
					xMarkTurn_t xMarkTurn;
					xMarkTurn.dx = xDim.GetCenterX();
					xMarkTurn.dy = xDim.GetCenterY();
					xMarkTurn.cx = xMarkTurn.dx;
					xMarkTurn.cy = xMarkTurn.dy;
					xMarkTurn.angle = xDim.GetRotateAngle();
					pMarkEngine->SetEngineCtrl(emMECMD_MARK_TURN, &xMarkTurn);
					// 获取芯片的模组指针
					int nWaferID = pGroup->vWaferMetas[vWorkItems[i].nSlaverID].m_nWaferID;
					CWaferGraph * pWafer = pWaferGraphDoc->GetWafer(nWaferID);
					if (NULL == pWafer) continue;
					// 替换2D数据
					if (nullptr != pMarkParam->p2DData) {
						if (!pWafer->Replace2DBarcodeData(1, CStringA(pMarkParam->p2DData[vWorkItems[i].nSlaverID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "2D数据替换失败!");
							break;
						}
					}
					// 替换第二组2D数据
					if (nullptr != pMarkParam->p2DData2) {
						if (!pWafer->Replace2DBarcodeData(2, CStringA(pMarkParam->p2DData2[vWorkItems[i].nSlaverID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "第二组2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "第二组2D数据替换失败!");
							break;
						}
					}
					// 替换印章中的序号
					if (!pWafer->ReplaceTextIndexData(vWorkItems[i].nSlaverID))
					{
						pPlatform->PrintLog(emLOG_LEVEL_ERROR, "序号数据替换失败!");
						pPlatform->Message(emMSG_TYPE_ERROR, "序号数据替换失败!");
						break;
					}
					iError = pWafer->Mark(pMarkEngine
						, (1 == nCycleCount) ? -1 : nLayerIndex
						, bUpdatePenPerObj ? MARKFLAG_UPDATEPEN : MARKFLAG_NONUPDATE | vWorkItems[i].uMarkFlag);
					if (0 != iError)break;
				}
			}
			if (!bFirstMark) {
				if (0 != pMarkEngine->MarkEnd())
					iError = -1;
				if (0 != iError)  goto GO_RESET_PRO;
			}
		}
		else if (1 == CWaferEngine::Instance()->GetMapDoc()->m_iMarkOrder)
		{
			pPlatform->PrintLog(emLOG_LEVEL_INFO, "先打单头，再打双头！");

			// 仅主头标记
			bFirstMark = true;
			for (size_t i = 0; i < nWorkSize; i++)
			{
				if ((vWorkItems[i].nMasterID >= 0) && (vWorkItems[i].nSlaverID < 0))//--仅主
				{
					if (bFirstMark) {
						bFirstMark = false;
						CMarkPlatform::Instance()->MarkDriver()->SetScanHead(1, 0);
						pMarkEngine->MarkBegin();
					}
					xHeadTurn_t xHeadTurn;
					xHeadTurn.dx = 0.0;
					xHeadTurn.dy = 0.0;
					xHeadTurn.cx = 0.0;
					xHeadTurn.cy = 0.0;
					xHeadTurn.angle = 0.0;
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_MASTER_TURN, &xHeadTurn);

					//--计算位置
					xDim_t xDim;
					xDim = pGroup->vWaferMetas[vWorkItems[i].nMasterID].xDim;
					CCoordTransform::Instance()->Base2Head(0, xDim.dimPoints1[0].x, xDim.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(0, xDim.dimPoints1[1].x, xDim.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(0, xDim.dimPoints1[2].x, xDim.dimPoints1[2].y);
					//--将设计图移至芯片处标记
					xMarkTurn_t xMarkTurn;
					xMarkTurn.dx = xDim.GetCenterX();
					xMarkTurn.dy = xDim.GetCenterY();
					xMarkTurn.cx = xMarkTurn.dx;
					xMarkTurn.cy = xMarkTurn.dy;
					xMarkTurn.angle = xDim.GetRotateAngle();
					pMarkEngine->SetEngineCtrl(emMECMD_MARK_TURN, &xMarkTurn);
					// 获取芯片的模组指针
					int nWaferID = pGroup->vWaferMetas[vWorkItems[i].nMasterID].m_nWaferID;
					CWaferGraph * pWafer = pWaferGraphDoc->GetWafer(nWaferID);
					if (NULL == pWafer) continue;
					// 替换2D数据
					if (nullptr != pMarkParam->p2DData) {
						if (!pWafer->Replace2DBarcodeData(1, CStringA(pMarkParam->p2DData[vWorkItems[i].nMasterID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "2D数据替换失败!");
							break;
						}
					}
					// 替换第二组2D数据
					if (nullptr != pMarkParam->p2DData2) {
						if (!pWafer->Replace2DBarcodeData(2, CStringA(pMarkParam->p2DData2[vWorkItems[i].nMasterID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "第二组2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "第二组2D数据替换失败!");
							break;
						}
					}
					// 替换印章中的序号
					if (!pWafer->ReplaceTextIndexData(vWorkItems[i].nMasterID))
					{
						pPlatform->PrintLog(emLOG_LEVEL_ERROR, "序号数据替换失败!");
						pPlatform->Message(emMSG_TYPE_ERROR, "序号数据替换失败!");
						break;
					}
					iError = pWafer->Mark(pMarkEngine
						, (1 == nCycleCount) ? -1 : nLayerIndex
						, (bUpdatePenPerObj ? MARKFLAG_UPDATEPEN : MARKFLAG_NONUPDATE) | vWorkItems[i].uMarkFlag);
					if (0 != iError)break;
				}
			}
			if (!bFirstMark) {
				if (0 != pMarkEngine->MarkEnd())
					iError = -1;
				if (0 != iError)  goto GO_RESET_PRO;
			}

			//仅从头标记
			bFirstMark = true;
			for (size_t i = 0; i < nWorkSize; i++)
			{
				if ((vWorkItems[i].nMasterID < 0) && (vWorkItems[i].nSlaverID >= 0))//--仅从
				{
					if (bFirstMark) {
						bFirstMark = false;
						CMarkPlatform::Instance()->MarkDriver()->SetScanHead(0, 1);
						pMarkEngine->MarkBegin();
					}
					xHeadTurn_t xHeadTurn;
					xHeadTurn.dx = 0.0;
					xHeadTurn.dy = 0.0;
					xHeadTurn.cx = 0.0;
					xHeadTurn.cy = 0.0;
					xHeadTurn.angle = 0.0;
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_SLAVER_TURN, &xHeadTurn);

					//--计算位置
					xDim_t xDim;
					xDim = pGroup->vWaferMetas[vWorkItems[i].nSlaverID].xDim;
					CCoordTransform::Instance()->Base2Head(1, xDim.dimPoints1[0].x, xDim.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(1, xDim.dimPoints1[1].x, xDim.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(1, xDim.dimPoints1[2].x, xDim.dimPoints1[2].y);
					//--将设计图移至芯片处标记
					xMarkTurn_t xMarkTurn;
					xMarkTurn.dx = xDim.GetCenterX();
					xMarkTurn.dy = xDim.GetCenterY();
					xMarkTurn.cx = xMarkTurn.dx;
					xMarkTurn.cy = xMarkTurn.dy;
					xMarkTurn.angle = xDim.GetRotateAngle();
					pMarkEngine->SetEngineCtrl(emMECMD_MARK_TURN, &xMarkTurn);
					// 获取芯片的模组指针
					int nWaferID = pGroup->vWaferMetas[vWorkItems[i].nSlaverID].m_nWaferID;
					CWaferGraph * pWafer = pWaferGraphDoc->GetWafer(nWaferID);
					if (NULL == pWafer) continue;
					// 替换2D数据
					if (nullptr != pMarkParam->p2DData) {
						if (!pWafer->Replace2DBarcodeData(1, CStringA(pMarkParam->p2DData[vWorkItems[i].nSlaverID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "2D数据替换失败!");
							break;
						}
					}
					// 替换第二组2D数据
					if (nullptr != pMarkParam->p2DData2) {
						if (!pWafer->Replace2DBarcodeData(2, CStringA(pMarkParam->p2DData2[vWorkItems[i].nSlaverID]).GetString())) {
							pPlatform->PrintLog(emLOG_LEVEL_ERROR, "第二组2D数据替换失败!");
							pPlatform->Message(emMSG_TYPE_ERROR, "第二组2D数据替换失败!");
							break;
						}
					}
					// 替换印章中的序号
					if (!pWafer->ReplaceTextIndexData(vWorkItems[i].nSlaverID))
					{
						pPlatform->PrintLog(emLOG_LEVEL_ERROR, "序号数据替换失败!");
						pPlatform->Message(emMSG_TYPE_ERROR, "序号数据替换失败!");
						break;
					}
					iError = pWafer->Mark(pMarkEngine
						, (1 == nCycleCount) ? -1 : nLayerIndex
						, bUpdatePenPerObj ? MARKFLAG_UPDATEPEN : MARKFLAG_NONUPDATE | vWorkItems[i].uMarkFlag);
					if (0 != iError)break;
				}
			}
			if (!bFirstMark) {
				if (0 != pMarkEngine->MarkEnd())
					iError = -1;
				if (0 != iError)  goto GO_RESET_PRO;
			}

			// 双头同时标记
			CMarkPlatform::Instance()->MarkDriver()->SetScanHead(1, 1);
			pMarkEngine->MarkBegin();
			for (size_t i = 0; i < nWorkSize; i++)
			{
				if ((vWorkItems[i].nMasterID >= 0) && (vWorkItems[i].nSlaverID >= 0)) //--主从
				{
					//PrintHeadID(vWorkItems[i].nMasterID, vWorkItems[i].nSlaverID);

					//--计算从相对于主的位置
					xDim_t xDimM, xDimS, xDim;
					xDimM = pGroup->vWaferMetas[vWorkItems[i].nMasterID].xDim;
					xDimS = pGroup->vWaferMetas[vWorkItems[i].nSlaverID].xDim;
					CCoordTransform::Instance()->Base2Head(0, xDimM.dimPoints1[0].x, xDimM.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(0, xDimM.dimPoints1[1].x, xDimM.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(0, xDimM.dimPoints1[2].x, xDimM.dimPoints1[2].y);
					CCoordTransform::Instance()->Base2Head(1, xDimS.dimPoints1[0].x, xDimS.dimPoints1[0].y);
					CCoordTransform::Instance()->Base2Head(1, xDimS.dimPoints1[1].x, xDimS.dimPoints1[1].y);
					CCoordTransform::Instance()->Base2Head(1, xDimS.dimPoints1[2].x, xDimS.dimPoints1[2].y);
					xDim.dimPoints0[0] = xDimM.dimPoints1[0];
					xDim.dimPoints0[1] = xDimM.dimPoints1[1];
					xDim.dimPoints0[2] = xDimM.dimPoints1[2];
					xDim.dimPoints1[0] = xDimS.dimPoints1[0];
					xDim.dimPoints1[1] = xDimS.dimPoints1[1];
					xDim.dimPoints1[2] = xDimS.dimPoints1[2];
					xHeadTurn_t xHeadTurn;
					xHeadTurn.dx = 0.0;
					xHeadTurn.dy = 0.0;
					xHeadTurn.cx = 0.0;
					xHeadTurn.cy = 0.0;
					xHeadTurn.angle = 0.0;
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_MASTER_TURN, &xHeadTurn);
					xHeadTurn.dx = xDim.GetOffsetX();
					xHeadTurn.dy = xDim.GetOffsetY();
					xHeadTurn.cx = xDim.dimPoints1[0].x;
					xHeadTurn.cy = xDim.dimPoints1[0].y;
					xHeadTurn.angle = xDim.GetAffineAngle();
					CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_SLAVER_TURN, &xHeadTurn);

					//--将设计图移至芯片处标记
					xMarkTurn_t xMarkTurn;
					xMarkTurn.dx = xDimM.GetCenterX();
					xMarkTurn.dy = xDimM.GetCenterY();
					xMarkTurn.cx = xMarkTurn.dx;
					xMarkTurn.cy = xMarkTurn.dy;
					xMarkTurn.angle = xDimM.GetRotateAngle();
					pMarkEngine->SetEngineCtrl(emMECMD_MARK_TURN, &xMarkTurn);

					// 获取芯片的模组指针
					int nWaferID = pGroup->vWaferMetas[vWorkItems[i].nMasterID].m_nWaferID;
					CWaferGraph * pWafer = pWaferGraphDoc->GetWafer(nWaferID);
					if (NULL == pWafer) continue;
					iError = pWafer->Mark(pMarkEngine
						, (1 == nCycleCount) ? -1 : nLayerIndex
						, (bUpdatePenPerObj ? MARKFLAG_UPDATEPEN : MARKFLAG_NONUPDATE) | MARKFLAG_ONLY_MARK_NON_VAR);
					if (0 != iError)break;
				}
			}
			if (0 != pMarkEngine->MarkEnd())
				iError = -1;
			if (0 != iError)  goto GO_RESET_PRO;
		}
	}
GO_RESET_PRO:
	xHeadTurn_t xHeadTurn;
	xHeadTurn.dx = 0.0;
	xHeadTurn.dy = 0.0;
	xHeadTurn.cx = 0.0;
	xHeadTurn.cy = 0.0;
	xHeadTurn.angle = 0.0;
	CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_MASTER_TURN, &xHeadTurn);
	CMarkPlatform::Instance()->MarkDriver()->SetDriverCtrl(CMD_SLAVER_TURN, &xHeadTurn);
	xMarkTurn_t xMarkTurn;
	xMarkTurn.dx = 0.0;
	xMarkTurn.dy = 0.0;
	xMarkTurn.cx = 0.0;
	xMarkTurn.cy = 0.0;
	xMarkTurn.angle = 0.0;
	pMarkEngine->SetEngineCtrl(emMECMD_MARK_TURN, &xMarkTurn);
	pMarkEngine->ActiveOutputSign(emSIGN_OUT_REDLIGHT, true);

	if (0 != iError)
		return 0;
	return 1;
}

int32_t CWaferGroup::Mark(IPlatform *pPlatform, const vector<CString>& v2DBarcodeData, const vector<CString>& v2DBarcodeData2, bool bPenOrder/* = false*/, bool bUp)
{
	int32_t iError(0);
    size_t nWaferSize = vWaferMetas.size();
    if (nWaferSize < 1)return -1;
    if (NULL == pPlatform)return -1;

	// 统计所有的标记顺序
    vector<xWorkItem_t> vWorkItems;
    if(GetWorkTableNew(vWorkItems, bUp)<1)return -1;

	// 启动标记线程
	stMarkThreadParam_t stMTParam;
	stMTParam.bPenOrder = bPenOrder;
	stMTParam.pGroup = this;
	stMTParam.vWorkItem = vWorkItems;
	stMTParam.pMarkEngine = CMarkPlatform::Instance()->MarkEngine();
	stMTParam.p2DData = (v2DBarcodeData.size() == 0) ? nullptr : v2DBarcodeData.data();
	stMTParam.p2DData2 = (v2DBarcodeData2.size() == 0) ? nullptr : v2DBarcodeData2.data();

	HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, MarkThread, (void*)&stMTParam, 0, nullptr);
	if (0 == handle)
	{
		pPlatform->PrintLog(emLOG_LEVEL_ERROR, "MarkThread create error!");
		return  -1;
	}

	CMarkPlatform::Instance()->MsgWaitForThread(&handle);

	// 获取线程返回值
	DWORD dwThreadRet = 0;
	GetExitCodeThread(handle, &dwThreadRet);

	CloseHandle(handle);

	if (1 != dwThreadRet)
		return -1;
	return 0;
}
int32_t CWaferGroup::GetWorkTable(vector<xWorkItem_t> &vWorkItems)
{
    vector<int32_t> vMasterID;
    vector<int32_t> vSlaverID;
	xWaferMeta_t WaferMetas;
	size_t nWaferSize = vWaferMetas.size();
    int iHead;
    for (size_t i = 0; i < nWaferSize; i++)
    {
		WaferMetas = vWaferMetas[i];
        if (!WaferMetas.IsMarkable())continue;
        iHead = WaferMetas.GetHead(); 
        if (0x01 == iHead)
            vMasterID.push_back(i);
        else if (0x02 == iHead)
            vSlaverID.push_back(i);
        else if (0x03 == iHead)
            vMasterID.push_back(i);
    }
  
	/**
	* 将主头和从头一一匹配，以主头为基准
	* 
	*/
	CWaferGraph * pWafer = NULL;
	CWaferGraphDoc * pWaferDoc = CWaferEngine::Instance()->GetGraphDoc();
	xWorkItem_t xItem;
	int nMWaferID = -1, nSWaferID = -1;
    size_t nMasterSize = vMasterID.size();
    size_t nSlaverSize = vSlaverID.size();
	int nMetasCount = vWaferMetas.size();
	for (size_t i = 0; i < nMasterSize; i++)
	{
		if (vMasterID[i] < 0 || vMasterID[i] >= nMetasCount)
			continue;

		nMWaferID = vWaferMetas[vMasterID[i]].m_nWaferID;
		xItem.nMasterID = vMasterID[i];
		xItem.nSlaverID = -1;

		// 如果芯片模块中存在变量，则相同的芯片模块也要分头打
		pWafer = pWaferDoc->GetWafer(nMWaferID);
		if (NULL != pWafer && pWafer->IsExistVarObj())
		{
			vWorkItems.push_back(xItem);
			continue;
		}

		for (size_t j = 0; j < nSlaverSize; j++)
		{
			if (vSlaverID[j] < 0 || vSlaverID[j] >= nMetasCount)
				continue;

			nSWaferID = vWaferMetas[vSlaverID[j]].m_nWaferID;
			if (nMWaferID == nSWaferID)
			{
				xItem.nSlaverID = vSlaverID[j];
				vSlaverID[j] = -1;
				break;
			}
		}
		vWorkItems.push_back(xItem);
	}
	for (size_t i = 0; i < nSlaverSize; i++)
	{
		if (-1 != vSlaverID[i])
		{
			xItem.nMasterID = -1;
			xItem.nSlaverID = vSlaverID[i];
			vWorkItems.push_back(xItem);
		}
	}

    return vWorkItems.size();
}

/**
* 印章标记顺序说明:
* 第一步，先找到主从头印章模板匹配的ID；
* 第二步，如果匹配列表中存在印章模板包含变量，则需要在标记顺序列表中再添加两个元素（只标记主头变量内容，只标记从头变量内容）；
* 第三步，分别找到主头和从头印章模板不匹配的ID；
*/
int32_t CWaferGroup::GetWorkTableNew(vector<xWorkItem_t> &vWorkItems, bool bUp)
{
	vector<int32_t> vMasterID;
	vector<int32_t> vSlaverID;
	xWaferMeta_t WaferMetas;
	for (int col = 0; col < m_ArrayInfo.cell.nColCount; col++) {
		int row = bUp ? 0 : m_ArrayInfo.cell.nRowCount - 1;
		while (row >= 0 && row < m_ArrayInfo.cell.nRowCount) {
			int index = GetWaferMetaID(m_ArrayInfo.cell.nRowCount - row, col + 1);
			if (!vWaferMetas[index].IsMarkable()) {
				bUp ? row++ : row--;
				continue;
			}
			int iHead = vWaferMetas[index].GetHead();
			if (0x01 == iHead)
				vMasterID.push_back(index);
			else if (0x02 == iHead)
				vSlaverID.push_back(index);
			else if (0x03 == iHead)
				vMasterID.push_back(index);

			bUp ? row++ : row--;
		}
	}

//#define ODD_COL_MID_LAST_MARK
	// 计算列数是否为奇数列，如果为奇数列，中间一列需要最后标记
#ifdef ODD_COL_MID_LAST_MARK
	int uMidCol = -1;
	if (0 != m_ArrayInfo.cell.nColCount % 2)
		uMidCol = m_ArrayInfo.cell.nColCount / 2 + 1;
#endif

	/**
	* 将主头和从头一一匹配，以主头为基准
	*
	*/
	CWaferGraph * pWafer = NULL;
	CWaferGraphDoc * pWaferDoc = CWaferEngine::Instance()->GetGraphDoc();
	xWorkItem_t xItem;
	int nMWaferID = -1, nSWaferID = -1;
	size_t nMasterSize = vMasterID.size();
	size_t nSlaverSize = vSlaverID.size();
	int nMetasCount = vWaferMetas.size();
	int iWaferCount = pWaferDoc->GetWaferCount();
	for (int iWCnt = 0; iWCnt < iWaferCount; iWCnt++) {   // 按照BIN的顺序
		for (size_t i = 0; i < nMasterSize; i++)
		{
			if (vMasterID[i] < 0 || vMasterID[i] >= nMetasCount)
				continue;

			nMWaferID = vWaferMetas[vMasterID[i]].m_nWaferID;
			if (iWCnt != nMWaferID)
				continue;
#ifdef ODD_COL_MID_LAST_MARK
			// 如果是奇数列，则中间一列最后标记
			if (uMidCol == vWaferMetas[vMasterID[i]].nCol)
				continue;
#endif

			xItem.nMasterID = vMasterID[i];
			xItem.nSlaverID = -1;
			xItem.uMarkFlag = MARKFLAG_MARK_ALL;

			// 当选择按印章顺序标记时， 如果芯片模块中存在变量，则相同的芯片模块也要分头打
			//pWafer = pWaferDoc->GetWafer(nMWaferID);
			//if (NULL != pWafer && pWafer->IsExistVarObj())
			//{
			//	vWorkItems.push_back(xItem);
			//	continue;
			//}

			// 查找从头中的匹配印章
			for (size_t j = 0; j < nSlaverSize; j++)
			{
				if (vSlaverID[j] < 0 || vSlaverID[j] >= nMetasCount)
					continue;

				nSWaferID = vWaferMetas[vSlaverID[j]].m_nWaferID;
				if (iWCnt != nSWaferID)
					continue;
#ifdef ODD_COL_MID_LAST_MARK
				if (uMidCol == vWaferMetas[vSlaverID[j]].nCol)
					continue;
#endif

				if (nMWaferID == nSWaferID) {
					xItem.nSlaverID = vSlaverID[j];
					vSlaverID[j] = -1;  // 从头ID已经被匹配，赋值为-1
					xItem.uMarkFlag = MARKFLAG_ONLY_MARK_NON_VAR;  // 对于主从头匹配的印章，如果存在变量，则第一遍先标记相同内容，后面再添加一遍流程，只标记不同内容
					break;
				}
			}
			vWorkItems.push_back(xItem);
		}
#ifdef ODD_COL_MID_LAST_MARK    
		// 收集奇数列剩下的中间一列
		if (-1 != uMidCol) {
			for (size_t i = 0; i < nMasterSize; i++)
			{
				if (vMasterID[i] < 0 || vMasterID[i] >= nMetasCount)
					continue;

				nMWaferID = vWaferMetas[vMasterID[i]].m_nWaferID;
				if (iWCnt != nMWaferID)
					continue;

				// 如果是奇数列，则中间一列最后标记
				if (uMidCol != vWaferMetas[vMasterID[i]].nCol)
					continue;


				xItem.nMasterID = vMasterID[i];
				xItem.nSlaverID = -1;
				xItem.uMarkFlag = MARKFLAG_MARK_ALL;

				// 当选择按印章顺序标记时， 如果芯片模块中存在变量，则相同的芯片模块也要分头打
				//pWafer = pWaferDoc->GetWafer(nMWaferID);
				//if (NULL != pWafer && pWafer->IsExistVarObj())
				//{
				//	vWorkItems.push_back(xItem);
				//	continue;
				//}

				// 查找从头中的匹配印章
				for (size_t j = 0; j < nSlaverSize; j++)
				{
					if (vSlaverID[j] < 0 || vSlaverID[j] >= nMetasCount)
						continue;

					nSWaferID = vWaferMetas[vSlaverID[j]].m_nWaferID;
					if (iWCnt != nSWaferID)
						continue;

					if (uMidCol != vWaferMetas[vSlaverID[j]].nCol)
						continue;

					if (nMWaferID == nSWaferID) {
						xItem.nSlaverID = vSlaverID[j];
						vSlaverID[j] = -1;  // 从头ID已经被匹配，赋值为-1
						xItem.uMarkFlag = MARKFLAG_ONLY_MARK_NON_VAR;  // 对于主从头匹配的印章，如果存在变量，则第一遍先标记相同内容，后面再添加一遍流程，只标记不同内容
						break;
					}
				}
				vWorkItems.push_back(xItem);
			}
		}
#endif
	}

	//for (auto & item : vWorkItems) {
	//	if(-1 != item.nSlaverID)
	//		CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_DEBUG, "%d:%d", vWaferMetas[item.nMasterID].GetWaferID(), vWaferMetas[item.nSlaverID].GetWaferID());
	//}

	// 如果前面收集的标记印章列表中(主从头匹配上)存在印章模板包含变量或2D Barcode，则需要在标记顺序列表中再添加两个元素（只标记主头变量内容，只标记从头变量内容）；
	size_t iMatchWorkItemCnt = vWorkItems.size();
	for (size_t i = 0; i < iMatchWorkItemCnt; i++)
	{
		if ((-1 != vWorkItems[i].nMasterID) && (-1 != vWorkItems[i].nSlaverID))
		{
			int iWaferID = vWaferMetas[vWorkItems[i].nMasterID].m_nWaferID;
			bool bExistVarObj = pWaferDoc->GetWafer(iWaferID)->IsExistVarObj();
			bool b2DBarcode = pWaferDoc->GetWafer(iWaferID)->IsExist2DBarcode();
			bool bTextIndex = pWaferDoc->GetWafer(iWaferID)->IsExistTextIndex();
			if (bExistVarObj || b2DBarcode || bTextIndex) {
				// 添加一个主头对象
				xItem.nMasterID = vWorkItems[i].nMasterID;
				xItem.nSlaverID = -1;
				xItem.uMarkFlag = MARKFLAG_ONLY_MARK_VAR;
				vWorkItems.push_back(xItem);

				// 添加一个从头对象
				xItem.nMasterID = -1;
				xItem.nSlaverID = vWorkItems[i].nSlaverID;
				xItem.uMarkFlag = MARKFLAG_ONLY_MARK_VAR;
				vWorkItems.push_back(xItem);
			}
		}
	}

	// 没有匹配的从头印章，添加到工作列表后面
	for (size_t i = 0; i < nSlaverSize; i++)
	{
		if (-1 != vSlaverID[i])
		{
			xItem.uMarkFlag = MARKFLAG_MARK_ALL;
			xItem.nMasterID = -1;
			xItem.nSlaverID = vSlaverID[i];
			vWorkItems.push_back(xItem);
		}
	}

	return vWorkItems.size();
}

int32_t CWaferGroup::GetColCount()
{
	//int nColCount = 0;
	//for (auto & item : vWaferMetas)
	//{
	//	nColCount = max(nColCount, item.nCol);
	//}
	return m_ArrayInfo.cell.nColCount;
}

int32_t CWaferGroup::GetRowCount()
{
	//int nRowCount = 0;
	//for (auto & item : vWaferMetas)
	//{
	//	nRowCount = max(nRowCount, item.nRow);
	//}
	return m_ArrayInfo.cell.nRowCount;
}

int32_t CWaferGroup::GetWaferMetaID(int32_t row, int32_t col)
{
#if 0  // 当阵列数据变多时，此处的运算会影响速度
	for (size_t i = 0, nSize = vWaferMetas.size(); i < nSize; i++)
	{
		if (row == vWaferMetas[i].nRow && col == vWaferMetas[i].nCol)
			return i;
	}
	return -1;
#else
	return (col - 1) * m_ArrayInfo.cell.nRowCount + (m_ArrayInfo.cell.nRowCount - row);
#endif
}

double CWaferGroup::GetCellHeight()
{
	return m_ArrayInfo.cell.dCellHight;
}

double CWaferGroup::GetCellWidth()
{
	return m_ArrayInfo.cell.dCellWidth;
}

bool CWaferGroup::Rotate(double cx, double cy, double dbDeg)
{
	if (D_EQUAL(dbDeg, 0))
		return true;

	for (auto & item : vWaferMetas)
	{
		item.xDim.Rotate(cx, cy, dbDeg);
	}

	return true;
}

bool CWaferGroup::Rotate(double dbDeg)
{
	if (D_EQUAL(dbDeg, 0))
		return false;

	for (auto & item : vWaferMetas)
	{
		item.xDim.Rotate(dbDeg);
	}

	return true;
}

bool CWaferGroup::Repos(double dx, double dy)
{
	if (D_EQUAL(dx, 0) && D_EQUAL(dy, 0))
		return true;
	
	for (auto & item : vWaferMetas)
	{
		item.xDim.Repos(dx, dy);
	}

	return true;
}

bool CWaferGroup::ArrayR(int rc, int cc, double sx, double sy, 
	double dx, double dy, int c, double dp)
{// 水平向右
	int wc = 0;   // 模块数量
	double dstepx = 0;
	for (int row = 0; row < rc; row++)
	{
		dstepx = sx;
		for (int col = 0; col < cc; col++)
		{
			vWaferMetas[wc].xDim.Repos(dstepx, sy - row * dy);
			if (0 != c && (col + 1) % c == 0)
				dstepx += dp;
			else
				dstepx += dx;

			vWaferMetas[wc].nRow = row + 1;
			vWaferMetas[wc].nCol = col + 1;
			wc++;
		}
	}
	return true;
}

bool CWaferGroup::ArrayRG(int rc, int cc, double sx, double sy, 
	double dx, double dy, int c, double dp)
{// 水平向右，来回
	int nCellC = cc / c;
	int wc = 0;   // 模块数量
	double dstepx = 0;
	for (int row = 0; row < rc; row++)
	{
		bool bOdd = row & 0x01;   // 是否为奇数 1, 3, 5
		dstepx = bOdd ? (cc - nCellC) * dx + (nCellC - 1) * dp + sx : sx;
		for (int col = 0; col < cc; col++)
		{			
			vWaferMetas[wc].xDim.Repos(dstepx, sy - row * dy);
			if (0 != c && (col + 1) % c == 0)
				dstepx += bOdd ? (-dp) : dp;
			else
				dstepx += bOdd ? (-dx) : dx; 

			vWaferMetas[wc].nRow = row + 1;
			vWaferMetas[wc].nCol = bOdd ? (cc - col) : col + 1;
			wc++;
		}
	}
	return true;
}

bool CWaferGroup::ArrayD(int rc, int cc, double sx, double sy, 
	double dx, double dy, int c, double dp)
{// 垂直向下
	int wc = 0;   // 模块数量
	double dstepx = sx;
	for (int col = 0; col < cc; col++)
	{
		for (int row = 0; row < rc; row++)
		{
			vWaferMetas[wc].xDim.Repos(dstepx, sy - row * dy);
			vWaferMetas[wc].nRow = row + 1;
			vWaferMetas[wc].nCol = col + 1;
			wc++;
		}
		if (0 != c && (col + 1) % c == 0)
			dstepx += dp;
		else
			dstepx += dx;
	}
	return true;
}

bool CWaferGroup::ArrayDG(int rc, int cc, double sx, double sy, 
	double dx, double dy, int c, double dp)
{// 垂直向下，来回
	int wc = 0;   // 模块数量
	double dstepx = sx;
	for (int col = 0; col < cc; col++)
	{
		bool bOdd = col & 0x01;   // 是否为奇数
		for (int row = 0; row < rc; row++)
		{
			vWaferMetas[wc].xDim.Repos(dstepx, 
				sy - (bOdd ? (rc - 1 - row) : row) * dy);
			vWaferMetas[wc].nRow = bOdd ? (rc - row) : row + 1;
			vWaferMetas[wc].nCol = col + 1;
			wc++;
		}
		if (0 != c && (col + 1) % c == 0)
			dstepx += dp;
		else
			dstepx += dx;
	}
	return true;
}

/**
   ttttttr
   lttttrr
   llbbbbr
   lbbbbbb

*/
bool CWaferGroup::ArraySO(int rc, int cc, double sx, double sy, 
	double dx, double dy, int c, double dp)
{// 螺旋，由外向里
	int wc = 0;   // 模块数量
	int sr = 0;   // 起始行
	int sc = 0;   // 起始列
	int er = rc - 1;  // 结束行
	int ec = cc - 1;  // 结束列

	bool bEnd = false;
	int nC = vWaferMetas.size();
	while (wc < nC)
	{
		// 上横线
		for (int col = sc; col < ec; col++)
		{
			vWaferMetas[wc].xDim.Repos(sx + col * dx, sy - sr * dy);
			vWaferMetas[wc].nCol = col + 1;
			vWaferMetas[wc].nRow = sr + 1;
			if (++wc >= nC) break;
		}
		// 右竖线
		for (int row = sr; row < er; row++)
		{
			vWaferMetas[wc].xDim.Repos(sx + ec * dx, sy - row * dy);
			vWaferMetas[wc].nCol = ec + 1;
			vWaferMetas[wc].nRow = row + 1;
			if (++wc >= nC) break;
		}
		// 下横线
		for (int col = ec; col > sc; col--)
		{
			vWaferMetas[wc].xDim.Repos(sx + col * dx, sy - er * dy);
			vWaferMetas[wc].nCol = col + 1;
			vWaferMetas[wc].nRow = er + 1;
			if (++wc >= nC) break;
		}
		// 左竖线
		for (int row = er; row > sr; row--)
		{
			vWaferMetas[wc].xDim.Repos(sx + sc * dx, sy - row * dy);
			vWaferMetas[wc].nCol = sc + 1;
			vWaferMetas[wc].nRow = row + 1;
			if (++wc >= nC) break;
		}

		// 改变sr sc er ec
		sr++; sc++; er--; ec--;
		// 最后一个元素
		if ((sr == er || sc == ec)
			&& wc == nC - 1)
		{
			vWaferMetas[wc].xDim.Repos(sx + sc * dx, sy - ec * dy);
			vWaferMetas[wc].nCol = sc + 1;
			vWaferMetas[wc].nRow = ec + 1;
			if (++wc >= nC) break;
		}
	}
	return true;
}
bool CWaferGroup::ArraySI(int rc, int cc, double sx, double sy, 
	double dx, double dy, int c, double dp)
{// 螺旋，由里向外
	int sr = 0;   // 起始行
	int sc = 0;   // 起始列
	int er = rc - 1;  // 结束行
	int ec = cc - 1;  // 结束列

	bool bEnd = false;
	int wc = vWaferMetas.size() - 1;
	while (wc >= 0)
	{
		// 上横线
		for (int col = sc; col < ec; col++)
		{
			vWaferMetas[wc].xDim.Repos(sx + col * dx, sy - sr * dy);
			vWaferMetas[wc].nCol = col + 1;
			vWaferMetas[wc].nRow = sr + 1;
			if (--wc < 0) break;
		}
		// 右竖线
		for (int row = sr; row < er; row++)
		{
			vWaferMetas[wc].xDim.Repos(sx + ec * dx, sy - row * dy);
			vWaferMetas[wc].nCol = ec + 1;
			vWaferMetas[wc].nRow = row + 1;
			if (--wc < 0) break;
		}
		// 下横线
		for (int col = ec; col > sc; col--)
		{
			vWaferMetas[wc].xDim.Repos(sx + col * dx, sy - er * dy);
			vWaferMetas[wc].nCol = col + 1;
			vWaferMetas[wc].nRow = er + 1;
			if (--wc < 0) break;
		}
		// 左竖线
		for (int row = er; row > sr; row--)
		{
			vWaferMetas[wc].xDim.Repos(sx + sc * dx, sy - row * dy);
			vWaferMetas[wc].nCol = sc + 1;
			vWaferMetas[wc].nRow = row + 1;
			if (--wc < 0) break;
		}

		// 改变sr sc er ec
		sr++; sc++; er--; ec--;
		// 最后一个元素
		if ((sr == er || sc == ec)
			&& wc == 0)
		{
			vWaferMetas[wc].xDim.Repos(sx + sc * dx, sy - ec * dy);
			vWaferMetas[wc].nCol = sc + 1;
			vWaferMetas[wc].nRow = ec + 1;
			if (--wc < 0) break;
		}
	}
	return true;
}

bool CWaferGroup::ArrayU(int rc, int cc, double sx, double sy,
	double dx, double dy, int c, double dp)
{// 垂直向上
	int wc = 0;   // 模块数量
	double dstepx = sx;
	int nBlockC = 0;  // 块数量
	for (int col = 0; col < cc; col++)
	{
		for (int row = 0; row < rc; row++)
		{
			vWaferMetas[wc].xDim.Repos(dstepx, sy - (rc - 1 - row) * dy);
			vWaferMetas[wc].nRow = rc - row;
			vWaferMetas[wc].nCol = col + 1;
			vWaferMetas[wc].m_nBlockID = nBlockC;
			wc++;
		}
		if (0 != c && (col + 1) % c == 0)
		{
			dstepx += dp;
			nBlockC++;
		}
		else
			dstepx += dx;
	}
	return true;
}

bool CWaferGroup::ArrayU(double sx, double sy, double pitchCol, double pitchRow)
{
	int ncPerGroupX = (0 == m_ArrayInfo.group.nGroupCount) ? m_ArrayInfo.cell.nColCount : m_ArrayInfo.group.nGroupCount;   // 水平方向每个Group的印章列数量
	int ncPerGroupY = (0 == m_ArrayInfo.group.nGroupCountVer) ? m_ArrayInfo.cell.nRowCount : m_ArrayInfo.group.nGroupCountVer;   // 垂直方向每个Group的印章列数量
	int cc = m_ArrayInfo.cell.nColCount;   // 列数
	int rc = m_ArrayInfo.cell.nRowCount;   // 行数
	double dbGroupPitch = m_ArrayInfo.group.dbGroupPitch;  // 水平方向块间距
	double dbGroupPitchVer = m_ArrayInfo.group.dbGroupPitchVer;  // 垂直方向块间距
	int nIndentStyle = m_ArrayInfo.group.nIndentStyle;
	int ncilCol = m_ArrayInfo.group.nIterleaveCountX;   // 间隔偏移-每次间隔的列数量
	int ncilRow = m_ArrayInfo.group.nIterleaveCountY;   // 间隔偏移-每次间隔的行数量
	double dbIndentCol = m_ArrayInfo.group.dbIterleaveIndentX;   // 间隔偏移-列缩进的距离
	double dbIndentRow = m_ArrayInfo.group.dbIterleaveIndentY;   // 间隔偏移-行缩进的距离
	int nDegX = (-1 == m_ArrayInfo.group.nIterleaveDegX) ? 0 : m_ArrayInfo.group.nIterleaveDegX - m_ArrayInfo.cell.nWaferDeg;     // 缩进列的印章方向，相对于单颗芯片方向的角度
	int nDegY = (-1 == m_ArrayInfo.group.nIterleaveDegY) ? 0 : m_ArrayInfo.group.nIterleaveDegY - m_ArrayInfo.cell.nWaferDeg;     // 缩进行的印章方向，相对于单颗芯片方向的角度
	int wc = 0;   // 模块数量
	int nBlockC = 0;  // 水平方向块数量
	int nBlockCVer = 0; // 垂直方向块数量
	bool bIndentCol = false, bIndentRow = false;
	int ncilCold = ncilCol * 2;
	int ncilRowd = ncilRow * 2;
	double dstepx = sx;
	double dstepy = sy - (rc - 1) * pitchRow;
	for (int col = 0; col < cc; col++)
	{
		nBlockCVer = 0;
		dstepy = sy - (rc - 1) * pitchRow;
		for (int row = 0; row < rc; row++)
		{
			// 改变列印章方向
			if ((0 == nIndentStyle) && 0 != ncilCol && (col % ncilCold >= ncilCol))
			{
				if (0 != nDegX)
					vWaferMetas[wc].xDim.Rotate(nDegX);
				bIndentCol = true;
			}
			// 改变行印章方向
			if ((1 == nIndentStyle) && 0 != ncilRow && (row % ncilRowd >= ncilRow))
			{
				if (0 != nDegY)
					vWaferMetas[wc].xDim.Rotate(nDegY);
				bIndentRow = true;
			}
			vWaferMetas[wc].xDim.Repos(dstepx + (bIndentRow ? dbIndentRow : 0), dstepy - (bIndentCol ? dbIndentCol : 0));
			vWaferMetas[wc].nRow = rc - row;
			vWaferMetas[wc].nCol = col + 1;
			vWaferMetas[wc].m_nBlockID = nBlockC;
			vWaferMetas[wc].m_nBlockIDVer = nBlockCVer;
			wc++;
			bIndentCol = bIndentRow = false;

			// 垂直方向的块数量判断
			if (0 != ncPerGroupY && (row + 1) % ncPerGroupY == 0)
			{
				dstepy += dbGroupPitchVer;
				nBlockCVer++;
			}
			else
				dstepy += pitchRow;
		}
		// 水平方向的块数量判断
		if (0 != ncPerGroupX && (col + 1) % ncPerGroupX == 0)
		{
			dstepx += dbGroupPitch;
			nBlockC++;
		}
		else
			dstepx += pitchCol;
	}
	return true;
}

bool CWaferGroup::ChangeArrayU(int rc, int cc, double dx, double dy, double dbColPitchCS)
{
	int wc = 0;   // 模块数量
	int nCol2 = 0;
	bool bL2 = false;
	for (int col = 0; col < cc; col++)
	{
		for (int row = 0; row < rc; row++)
		{
			if (2 == vWaferMetas[wc].GetHead())
			{
				bL2 = true;
				vWaferMetas[wc].xDim.Repos(dy * col + dbColPitchCS * nCol2, dx * row);
			}
			else
				vWaferMetas[wc].xDim.Repos(dy * col, dx * row);

			wc++;
		}
		if (bL2)
		{
			nCol2++;
		}
	}
	return true;
}

bool CWaferGroup::ChangeArrayU(int rc, int cc, double dx, double dy, double grouppitch, double grouppitchVer, double dbColPitchCS)
{
	int wc = 0;   // 模块数量
	int nCol2 = 0;
	bool bL2 = false;
	for (int col = 0; col < cc; col++)
	{
		for (int row = 0; row < rc; row++)
		{
			if (2 == vWaferMetas[wc].GetHead()) {
				bL2 = true;
				// 为什么要用（col - vWaferMetas[wc].m_nBlockID）？： 在块修改间距和列修改间距都不为零的情况下，遇到块间距时，不能又加块间距，又加列间距
				vWaferMetas[wc].xDim.Repos(dy * (col - vWaferMetas[wc].m_nBlockID)
					+ grouppitch * vWaferMetas[wc].m_nBlockID + dbColPitchCS * nCol2
					, dx *  (row - vWaferMetas[wc].m_nBlockIDVer) + grouppitchVer * vWaferMetas[wc].m_nBlockIDVer);
			}
			else {
				vWaferMetas[wc].xDim.Repos(dy * (col - vWaferMetas[wc].m_nBlockID) + grouppitch * vWaferMetas[wc].m_nBlockID
					, dx *  (row - vWaferMetas[wc].m_nBlockIDVer) + grouppitchVer * vWaferMetas[wc].m_nBlockIDVer);
			}
			wc++;
		}
		if (bL2)
		{
			nCol2++;
		}
	}
	return true;
}

bool CWaferGroup::ChangeArrayIndent(const xArray_t &xNewArray)
{
	int cc = m_ArrayInfo.cell.nColCount;
	int rc = m_ArrayInfo.cell.nRowCount;
	int nIndentStyle = m_ArrayInfo.group.nIndentStyle;
	int ncilCol = m_ArrayInfo.group.nIterleaveCountX;   // 间隔偏移-每次间隔的列数量
	int ncilRow = m_ArrayInfo.group.nIterleaveCountY;   // 间隔偏移-每次间隔的行数量
	double dbIndentCol = xNewArray.group.dbIterleaveIndentX - m_ArrayInfo.group.dbIterleaveIndentX;   // 间隔偏移-列缩进的距离
	double dbIndentRow = xNewArray.group.dbIterleaveIndentY - m_ArrayInfo.group.dbIterleaveIndentY;   // 间隔偏移-行缩进的距离
	int nDegX = 0;     // 缩进列的印章方向，相对于单颗芯片方向的角度
	int nDegY = 0;     // 缩进行的印章方向，相对于单颗芯片方向的角度
	int nIterLeaveDegXOld = (-1 == m_ArrayInfo.group.nIterleaveDegX) ? m_ArrayInfo.cell.nWaferDeg : m_ArrayInfo.group.nIterleaveDegX;
	int nIterLeaveDegXNew = (-1 == xNewArray.group.nIterleaveDegX) ? xNewArray.cell.nWaferDeg : xNewArray.group.nIterleaveDegX;
	int nIterLeaveDegYOld = (-1 == m_ArrayInfo.group.nIterleaveDegY) ? m_ArrayInfo.cell.nWaferDeg : m_ArrayInfo.group.nIterleaveDegY;
	int nIterLeaveDegYNew = (-1 == xNewArray.group.nIterleaveDegY) ? xNewArray.cell.nWaferDeg : xNewArray.group.nIterleaveDegY;
	nDegX = (nIterLeaveDegXNew - nIterLeaveDegXOld) - (xNewArray.cell.nWaferDeg - m_ArrayInfo.cell.nWaferDeg);
	nDegY = (nIterLeaveDegYNew - nIterLeaveDegYOld) - (xNewArray.cell.nWaferDeg - m_ArrayInfo.cell.nWaferDeg);

	bool bIndentCol = false, bIndentRow = false;
	int ncilCold = ncilCol * 2;
	int ncilRowd = ncilRow * 2;
	int wc = 0;   // 模块数量
	for (int col = 0; col < cc; col++)
	{
		for (int row = 0; row < rc; row++)
		{
			// 改变列印章方向
			if ((0 == nIndentStyle) && 0 != ncilCol && (col % ncilCold >= ncilCol))
			{
				if (0 != nDegX)
					vWaferMetas[wc].xDim.Rotate(nDegX);
				bIndentCol = true;
			}
			// 改变行印章方向
			if ((1 == nIndentStyle) && 0 != ncilRow && (row % ncilRowd >= ncilRow))
			{
				if (0 != nDegY)
					vWaferMetas[wc].xDim.Rotate(nDegY);
				bIndentRow = true;
			}
			vWaferMetas[wc].xDim.Repos((bIndentRow ? dbIndentRow : 0)
				, -(bIndentCol ? dbIndentCol : 0));
			wc++;
			bIndentCol = bIndentRow = false;
		}
	}
	return true;
}

FField_t CWaferGroup::GetMatrixField() const
{
	DPoint_t dp[4];
	double dL, dT, dR, dB;
	dL = m_ArrayInfo.group.dbCenterX - m_ArrayInfo.group.dbGroupW * 0.5;
	dT = m_ArrayInfo.group.dbCenterY + m_ArrayInfo.group.dbGroupH * 0.5;
	dR = m_ArrayInfo.group.dbCenterX + m_ArrayInfo.group.dbGroupW * 0.5;
	dB = m_ArrayInfo.group.dbCenterY - m_ArrayInfo.group.dbGroupH * 0.5;
	dp[0] = DPoint_t(dL, dT);  // 左上角
	dp[1] = DPoint_t(dR, dT);  // 右上角
	dp[2] = DPoint_t(dR, dB);  // 右下角
	dp[3] = DPoint_t(dL, dB);  // 左下角

							   // 旋转
	for (int i = 0; i < 4; i++)
	{
		dp[i].rotate(m_ArrayInfo.group.dbCenterX, m_ArrayInfo.group.dbCenterY, DEG2RAD(m_ArrayInfo.group.dbGroupDeg));
	}

	// 旋转后，重新确定边界
	dL = min(min(min(dp[0].x, dp[1].x), dp[2].x), dp[3].x);
	dB = min(min(min(dp[0].y, dp[1].y), dp[2].y), dp[3].y);
	dR = max(max(max(dp[0].x, dp[1].x), dp[2].x), dp[3].x);
	dT = max(max(max(dp[0].y, dp[1].y), dp[2].y), dp[3].y);

	return FField_t(DPoint_t(dL, dT), DPoint_t(dR, dB));
}


void CWaferGroup::PrintHeadID(int nMasterID, int nSlaverID)
{
	CString s;
	s.Format(_T("主头ID:%d, 从头ID:%d\n"), nMasterID, nSlaverID);
	OutputDebugString(s);
}

int CWaferGroup::GetUsedLaserPenCount(int & nLayerID)
{
	int nLaserPenC = 0;
	CWaferGraphDoc * pWaferDoc = CWaferEngine::Instance()->GetGraphDoc();
	int nLayerC = pWaferDoc->GetLayerCount();
	for (int i = 0; i < nLayerC; i++)
	{
		if (pWaferDoc->IsLayerUsed(i)) {
			nLaserPenC++;
			nLayerID = i;
		}
	}
	return nLaserPenC;
}

/*********************************************************************/
CWaferMapDoc::CWaferMapDoc()
{
	m_iFileIndex = 0;
	m_iMaxFileIndex = 0;
}
CWaferMapDoc::~CWaferMapDoc()
{
}

CWaferGroup * CWaferMapDoc::AppendWaferGroup(const xArray_t &ArrayData)
{
	CWaferGroup *pWaferGroup = new CWaferGroup;
	CWaferGraphDoc * pWaferGraphDoc = CWaferEngine::Instance()->GetGraphDoc();
	pWaferGroup->SetArrayInfo(ArrayData);

	// 生成row * col个模块xWaferMeta_t
	xWaferMeta_t xWaferMeta;
	int nWaferCount = ArrayData.cell.nRowCount * ArrayData.cell.nColCount;
	while (0 < nWaferCount--)
	{
		xWaferMeta.m_nWaferID = 0;   // 默认为初始bin
		xWaferMeta.pWaferGroup = pWaferGroup;
		xWaferMeta.xDim.Init(ArrayData.cell.dCellWidth, ArrayData.cell.dCellHight, ArrayData.cell.nWaferDeg);
		pWaferGroup->vWaferMetas.push_back(xWaferMeta);
	}

	// 阵列
	int nSize = pWaferGroup->vWaferMetas.size();
	if (nSize == 0)
		return NULL;

	FField_t f0;
	pWaferGroup->vWaferMetas[0].xDim.GetField(f0);

	double w = f0.width();
	double h = f0.height();
	double stepx = 0;  // 水平间隔，左左间隔
	double stepy = 0;  // 垂直间隔，上上间隔
	if (ArrayData.cell.nPitchType == 0)  // 左左
	{
		stepx = ArrayData.cell.dbColPitch;
		stepy = ArrayData.cell.dbRowPitch;
	}
	else if (ArrayData.cell.nPitchType == 1)   // 右左
	{
		stepx = ArrayData.cell.dbColPitch + w;
		stepy = ArrayData.cell.dbRowPitch + h;
	}

	int nArrayCroupCount = ArrayData.group.nGroupCount;
	if (nArrayCroupCount == 0)
		nArrayCroupCount = ArrayData.cell.nColCount;
	int nGroupCount = ArrayData.cell.nColCount / nArrayCroupCount;
		double dbPitchW = (ArrayData.group.dbGroupW - (ArrayData.cell.nColCount - nGroupCount) * stepx
		- ArrayData.cell.dCellWidth
		- (nGroupCount - 1) * ArrayData.group.dbGroupPitch) / 2;
	double dbPitchH = (ArrayData.group.dbGroupH - (ArrayData.cell.nRowCount - 1) * stepy
		- ArrayData.cell.dCellHight) / 2;

	double sx = dbPitchW - f0.Left;   // 初始偏移值
	double sy = -dbPitchH + f0.Top;

	// 按照阵列类型移动模块
	switch (ArrayData.cell.nStyle)
	{
		//case 0: // 水平向右
		//	pWaferGroup->ArrayR(ArrayData.cell.nRowCount, ArrayData.cell.nColCount,
		//		sx, sy, stepx, stepy, nArrayCroupCount, ArrayData.group.dbGroupPitch);
		//	break;
		//case 1: // 水平来回向右
		//	pWaferGroup->ArrayRG(ArrayData.cell.nRowCount, ArrayData.cell.nColCount,
		//		sx, sy, stepx, stepy, nArrayCroupCount, ArrayData.group.dbGroupPitch);
		//	break;
		//case 2: // 垂直向下
		//	pWaferGroup->ArrayD(ArrayData.cell.nRowCount, ArrayData.cell.nColCount,
		//		sx, sy, stepx, stepy, nArrayCroupCount, ArrayData.group.dbGroupPitch);
		//	break;
		//case 3: // 垂直向下，来回
		//	pWaferGroup->ArrayDG(ArrayData.cell.nRowCount, ArrayData.cell.nColCount,
		//		sx, sy, stepx, stepy, nArrayCroupCount, ArrayData.group.dbGroupPitch);
		//	break;
		//case 4: // 螺旋，由外向内
		//	pWaferGroup->ArraySO(ArrayData.nRowCount, ArrayData.nColCount, 
		//		sx, sy, stepx, stepy);
		//	break;
		//case 5: // 螺旋，由内向外
		//	pWaferGroup->ArraySI(ArrayData.nRowCount, ArrayData.nColCount, 
		//		sx, sy, stepx, stepy);
		//	break;
	case 4: // 垂直向上
		pWaferGroup->ArrayU(sx, sy, stepx, stepy);
		//pWaferGroup->ArrayU(ArrayData.cell.nRowCount, ArrayData.cell.nColCount,
		//	sx, sy, stepx, stepy, nArrayCroupCount, ArrayData.group.dbGroupPitch);
		break;
	}

	if (pWaferGroup->vWaferMetas.size() > 0)
	{
		vWaferGroups.push_back(pWaferGroup);
	}

	// 移动和旋转整版对象
	double dbOffsetX = ArrayData.group.dbCenterX - ArrayData.group.dbGroupW / 2;
	double dbOffsetY = ArrayData.group.dbCenterY - (-ArrayData.group.dbGroupH / 2);
	pWaferGroup->Repos(dbOffsetX, dbOffsetY);

	pWaferGroup->Rotate(ArrayData.group.dbCenterX, ArrayData.group.dbCenterY, ArrayData.group.dbGroupDeg);

	return pWaferGroup;
}

void CWaferMapDoc::ChangeWaferGroup(const xArray_t &xOldArray, const xArray_t &xNewArray, int iGroupIndex)
{
	if (iGroupIndex < 0 || iGroupIndex > vWaferGroups.size())
		return;

	CWaferGroup *pWaferGroup = vWaferGroups[iGroupIndex];

	double dbRowPitch = xOldArray.cell.dbRowPitch;
	double dbColPitch = xOldArray.cell.dbColPitch;
	double dbColPitchCS = xOldArray.cell.dbColPitchCS;
	double dbCenterX = xOldArray.group.dbCenterX;
	double dbCenterY = xOldArray.group.dbCenterY;
	double dbGroupDeg = xOldArray.group.dbGroupDeg;
	double dbCellDir = (double)xOldArray.cell.nWaferDeg;
	double dbGroupPitch = xOldArray.group.dbGroupPitch;
	double dbGroupPitchVer = xOldArray.group.dbGroupPitchVer;

	// 印章自旋转
	pWaferGroup->Rotate(xNewArray.cell.nWaferDeg - dbCellDir);

	// 印章平移
	pWaferGroup->Repos(xNewArray.group.dbCenterX - dbCenterX, xNewArray.group.dbCenterY - dbCenterY);

	// 阵列旋转
	pWaferGroup->Rotate(xNewArray.group.dbCenterX, xNewArray.group.dbCenterY, xNewArray.group.dbGroupDeg - dbGroupDeg);

	// 修改间距
	pWaferGroup->ChangeArrayU(xNewArray.cell.nRowCount, xNewArray.cell.nColCount,
		xNewArray.cell.dbRowPitch - dbRowPitch, xNewArray.cell.dbColPitch - dbColPitch,
		xNewArray.group.dbGroupPitch - dbGroupPitch,
		xNewArray.group.dbGroupPitchVer - dbGroupPitchVer,
		xNewArray.cell.dbColPitchCS - dbColPitchCS);
	// 修改缩进量
	pWaferGroup->ChangeArrayIndent(xNewArray);

	pWaferGroup->SetArrayInfo(xNewArray);
}

void CWaferMapDoc::RemoveWaferGroup(const char *sName)
{

}
void CWaferMapDoc::SetModified(bool modify, bool bAddUndo)
{
	m_IsModify = modify;
	if (bAddUndo)
	{
		SaveTmpFile();
	}
}
bool CWaferMapDoc::GetModified() const
{
	// 如果文档本身被修改，则不用获取对象是否被修改，直接返回

	return m_IsModify;
}


bool CWaferMapDoc::SaveFile(IFileWriter *pTheWriter)
{
    int iSize = vWaferGroups.size();
    if (NULL == pTheWriter) return false;

    pTheWriter->WriteBegin(CN_WAFERMAP_BASE);    //Doc开始标志

    for (int i = 0; i < iSize;i++)
    {
        pTheWriter->WriteInt32(CN_WAFERMAP_GROUP, 0);
        vWaferGroups[i]->SaveFile(pTheWriter);
    }

	pTheWriter->WriteInt32(CN_WAFERMAP_TRACK_COUNT, iSize);
	pTheWriter->WriteDouble(CN_WAFERMAP_TRACK_VERPITCH, m_stTrackWay.dbTrackPitchY);
	pTheWriter->WriteDouble(CN_WAFERMAP_TRACK_HORPITCH, m_stTrackWay.dbTrackPitchX);
	pTheWriter->WriteDouble(CN_WAFERMAP_TRACK_DEG, m_stTrackWay.dbTrackDeg);

	// 定位点坐标
	for (auto & item : m_vAnchorPoint)
	{
		pTheWriter->WriteDouble(CN_WAFERMAP_ANCHOR_P_X, item.x);
		pTheWriter->WriteDouble(CN_WAFERMAP_ANCHOR_P_Y, item.y);
	}

	pTheWriter->WriteInt32(CN_WAFERMAP_MARK_ORDER, m_iMarkOrder);
	
    pTheWriter->WriteEnd(CN_WAFERMAP_BASE);    //Doc开始标志
    return true;
}
bool CWaferMapDoc::LoadFile(IFileReader *pTheReader) 
{
    if (NULL == pTheReader)
        return false;
    uint32_t code;

    for (size_t i = 0; i < vWaferGroups.size();i++)
    {
        if (NULL != vWaferGroups[i])
        {
            delete vWaferGroups[i];
        }
    }


    vWaferGroups.clear();
	m_vAnchorPoint.clear();

    CWaferGroup * WaferGroupTmp = NULL;
	DPoint_t dpAnchor;
    bool bFind = false;

	while (CN_WAFERMAP_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
		{
			break;
		}
		else
		{
			switch (code)
			{
			case CN_WAFERMAP_GROUP:
			{
				WaferGroupTmp = new CWaferGroup;
				vWaferGroups.push_back(WaferGroupTmp);
				WaferGroupTmp->LoadFile(pTheReader);
				break;
			}
			case CN_WAFERMAP_TRACK_VERPITCH:
				m_stTrackWay.dbTrackPitchY = pTheReader->GetDouble();
				break;
			case CN_WAFERMAP_TRACK_HORPITCH:
				m_stTrackWay.dbTrackPitchX = pTheReader->GetDouble();
				break;
			case CN_WAFERMAP_TRACK_DEG:
				m_stTrackWay.dbTrackDeg = pTheReader->GetDouble();
				break;
			case CN_WAFERMAP_TRACK_COUNT:
				m_stTrackWay.nTrackCount = pTheReader->GetInt32();
				break;
			case CN_WAFERMAP_ANCHOR_P_X:
				dpAnchor.x = pTheReader->GetDouble();
				break;
			case CN_WAFERMAP_ANCHOR_P_Y:
				dpAnchor.y = pTheReader->GetDouble();
				m_vAnchorPoint.push_back(dpAnchor);
				break;
			case CN_WAFERMAP_MARK_ORDER:
				m_iMarkOrder = pTheReader->GetInt32();
				CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_DEBUG, "打印顺序：%d", m_iMarkOrder);
				break;
            }
        }
    }
    return true;
}

bool CWaferMapDoc::DeleteAllTempFile()
{
	HANDLE hFirstFile = NULL;
	WIN32_FIND_DATAA FindData;

	string sFilePath(".\\His\\ur\\map\\");

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

	m_iFileIndex = 0;
	m_iMaxFileIndex = 0;

	return true;
}

bool CWaferMapDoc::SaveTmpFile()
{
	m_iFileIndex += 1;
	CStringA sFileName(".\\His\\ur\\map\\");
	if (!PathIsDirectoryA(sFileName.GetString()))
	{
		if (!MakeSureDirectoryPathExists(sFileName.GetString()))  //创建多层目录
		{
			return false;
		}
	}
	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	sprintf_s(buff, "%sHis%d.hisb", sFileName, m_iFileIndex);
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
	m_iMaxFileIndex = m_iFileIndex;
	return bRet;
}
bool CWaferMapDoc::LoadTmpFile(int iIndex)
{
	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	if (m_iFileIndex + iIndex <=0 || m_iFileIndex +iIndex > m_iMaxFileIndex)
	{
		return false;
	}
	sprintf_s(buff, ".\\His\\ur\\map\\His%d.hisb", m_iFileIndex + iIndex);
	m_iFileIndex += iIndex;
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(buff))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	
	bool bRet = LoadFile(pTheReader);

	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;
	return bRet;
}

bool CWaferMapDoc::SaveOriginDoc()
{
	CStringA sFileName(".\\His\\Origin\\");
	if (!PathIsDirectoryA(sFileName.GetString()))
	{
		if (!MakeSureDirectoryPathExists(sFileName.GetString()))  //创建多层目录
		{
			return false;
		}
	}
	// 打开文件
	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	sprintf(buff, "%sDoc.tmpb", sFileName);
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

bool CWaferMapDoc::LoadOriginDoc()
{
	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	sprintf(buff, ".\\His\\Origin\\Doc.tmpb");
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(buff))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	
	bool bRet = LoadFile(pTheReader);
	
	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;
	return bRet;
}

void CWaferMapDoc::RemoveAllGroup()
{
    for (size_t i = 0; i < vWaferGroups.size(); i++)
    {
        if (NULL != vWaferGroups[i])
        {
            delete vWaferGroups[i];
        }
    }
    vWaferGroups.clear();
	m_vAnchorPoint.clear();
}
void CWaferMapDoc::Draw(IGraphRender *pTheRender, const FField_t & fClient, bool bDrawPin)
{
    size_t nGroupSize = vWaferGroups.size();
    if (nGroupSize < 1)
    {
        return;
    }
    CWaferGroup *pWaferGroup = NULL;
    for (size_t i = 0; i < nGroupSize;i++)
    {
        pWaferGroup = vWaferGroups[i];
        if(NULL!=pWaferGroup)
            pWaferGroup->Draw(pTheRender, fClient, bDrawPin);
    }
}
int32_t CWaferMapDoc::Mark(const vector<CString>& v2DBarcodeData, const vector<CString>& v2DBarcodeData2, bool bPenOrder, bool bUp)
{
	size_t nGroupSize = vWaferGroups.size();
	if (nGroupSize < 1)
	{
		return -1;
	}
	int32_t iError(0);
	CWaferGroup *pWaferGroup = NULL;
	for (size_t i = 0; i < nGroupSize; i++)
	{
		pWaferGroup = vWaferGroups[i];
		if (NULL != pWaferGroup) {
			iError = pWaferGroup->Mark(CMarkPlatform::Instance(), v2DBarcodeData, v2DBarcodeData2, bPenOrder, bUp);
			//pWaferGroup->SetMapMode(false);
		}		
		if (0 != iError)
			break;
	}
	return iError;
}

int CWaferMapDoc::GetAnchorCount(bool bCCD)
{
	return bCCD ? m_vCCDAnchorPoint.size() : m_vAnchorPoint.size();
}

void CWaferMapDoc::ClearAnchor(bool bCCD)
{
	bCCD ? m_vCCDAnchorPoint.clear() : m_vAnchorPoint.clear();
}

void CWaferMapDoc::AddAnchor(const DPoint_t & dp, bool bCCD)
{
	bCCD ? m_vCCDAnchorPoint.push_back(dp) : m_vAnchorPoint.push_back(dp);
}

void CWaferMapDoc::GetAnchor(int id, DPoint_t & dp, bool bCCD)
{
	if (bCCD)
	{
		if (id < 0 || id >= m_vCCDAnchorPoint.size())
			return;

		dp = m_vCCDAnchorPoint[id];
	}
	else
	{
		if (id < 0 || id >= m_vAnchorPoint.size())
			return;

		dp = m_vAnchorPoint[id];
	}
}

int32_t CWaferMapDoc::CCDAnchor(void)
{
	IPlatform * pPlatform = CMarkPlatform::Instance();

	// 保存原始文档
	if (!SaveOriginDoc())
	{
		pPlatform->PrintLog(emLOG_LEVEL_ERROR, "原始文档保存失败！");
		return -1;
	}

	return 0;
}


int32_t CWaferMapDoc::ResetAnchor(void)
{
	// CCD定位点清空
	m_vCCDAnchorPoint.clear();

	IPlatform * pPlatform = CMarkPlatform::Instance();
	// 恢复原始文档
	if (!LoadOriginDoc())
	{
		pPlatform->PrintLog(emLOG_LEVEL_ERROR, "原始文档加载失败！");
		return -1;
	}
	return 0;
}

FField_t CWaferMapDoc::GetField(void) const
{
	bool bFirst = true;
	FField_t f, fTotal;

	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			it.xDim.GetField(f);
			if (bFirst)
			{
				fTotal = f;
				bFirst = false;
			}
			else
				fTotal = fTotal + f;
		}
	}
	return fTotal;
}

FField_t CWaferMapDoc::GetMatrixField() const
{
	FField_t field;
	bool bFirst = true;
	for (auto & item : vWaferGroups)
	{
		if (nullptr == item)
			continue;

		if (bFirst)
		{
			bFirst = false;
			field = item->GetMatrixField();
		}
		else
		{
			field = field + item->GetMatrixField();
		}
	}
	return field;
}

FField_t CWaferMapDoc::GetSelField(void) const
{
	bool bFirst = true;
	FField_t f, fTotal;

	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if (it.IsSel())
			{
				it.xDim.GetField(f);
				if (bFirst)
				{
					fTotal = f;
					bFirst = false;
				}
				else
					fTotal = fTotal + f;
			}
		}
	}
	return fTotal;
}

void CWaferMapDoc::Repos(double px, double py, int iAbs)
{
	FField_t field = GetField();
	double dx, dy;
	if (emPOS_ABS == iAbs)
	{
		dx = px - field.Left;
		dy = py - field.Top;
	}
	else
	{
		dx = px;
		dy = py;
	}

	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			it.xDim.Repos(dx, dy);
		}
	}
}
void CWaferMapDoc::Rotate(double cx, double cy, double angle)
{
	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			it.xDim.Rotate(cx, cy, angle);
		}
	}
}

void CWaferMapDoc::ReposSel(double px, double py, int iAbs)
{
	FField_t field = GetSelField();
	double dx, dy;
	if (emPOS_ABS == iAbs)
	{
		dx = px - field.Left;
		dy = py - field.Top;
	}
	else
	{
		dx = px;
		dy = py;
	}

	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if(it.IsSel())
				it.xDim.Repos(dx, dy);
		}
	}
}

void CWaferMapDoc::RotateSel(double cx, double cy, double angle)
{
	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if(it.IsSel())
				it.xDim.Rotate(cx, cy, angle);
		}
	}
}

void CWaferMapDoc::Rotate(double angle)
{
	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			it.xDim.Rotate(angle);
		}
	}
}

int32_t CWaferMapDoc::GetMapObjCount(void) const
{
	int32_t nCount = 0;
	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			nCount++;
		}
	}
	return nCount;
}

int32_t CWaferMapDoc::GetGroupCount()
{
	return vWaferGroups.size();
}

CWaferGroup * CWaferMapDoc::GetGroup(int32_t index)
{
	if (index < 0 || index >= vWaferGroups.size())
		return NULL;

	return vWaferGroups[index];
}

int32_t CWaferMapDoc::GetMapObjCount(const char * sName) const
{
	if (NULL == sName)
		return 0;

	CWaferGraphDoc * pWaferDoc = CWaferEngine::Instance()->GetGraphDoc();
	CWaferGraph * pWafer = NULL;
	int32_t nCount = 0;
	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			pWafer = pWaferDoc->GetWafer(it.m_nWaferID);
			if (NULL == pWafer)
				continue;
			if (0 == strcmp(sName, pWafer->GetName()))
				nCount++;
		}
	}
	return nCount;
}

int32_t CWaferMapDoc::GetMapObjSelCount(void) const
{
	int32_t nCount = 0;
	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if (it.IsSel())
				nCount++;
		}
	}
	return nCount;
}

int32_t CWaferMapDoc::GetMapObjMarkCount(void) const
{
	int32_t nCount = 0;
	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if (it.IsMarkable())
				nCount++;
		}
	}
	return nCount;
}

void CWaferMapDoc::ShowPosWnd()
{
#if 0
	if (GetMapObjSelCount() == 0) return;
	CDlgMapPos dlg(this);
	dlg.DoModal();
#endif
}

FField_t CWaferMapDoc::GetLeftSelField() const
{
	bool bFirst = true;
	FField_t f, fTotal;

	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if (1 == it.GetHead()) {
				if (it.IsSel())
				{
					it.xDim.GetField(f);
					if (bFirst)
					{
						fTotal = f;
						bFirst = false;
					}
					else
						fTotal = fTotal + f;
				}
			}
		}
	}
	return fTotal;
}

FField_t CWaferMapDoc::GetRightSelField() const
{
	bool bFirst = true;
	FField_t f, fTotal;

	for (auto & item : vWaferGroups)
	{
		for (auto & it : item->vWaferMetas)
		{
			if (2 == it.GetHead()) {
				if (it.IsSel())
				{
					it.xDim.GetField(f);
					if (bFirst)
					{
						fTotal = f;
						bFirst = false;
					}
					else
						fTotal = fTotal + f;
				}
			}
		}
	}
	return fTotal;
}

void CWaferMapDoc::SetPrintAll()
{
	size_t nGroupSize = vWaferGroups.size();
	if (nGroupSize < 1)
		return;

	for (int i = 0; i < nGroupSize; i++)
	{
		CWaferGroup *pWaferGroup = vWaferGroups[i];
		size_t nWaferSize = pWaferGroup->vWaferMetas.size();
		if (nWaferSize < 1)
		{
			continue;
		}
		for (int j = 0; j < nWaferSize; j++)
		{
			pWaferGroup->vWaferMetas[j].EnableMark(true);
		}
	}
}

void CWaferMapDoc::ChangeWaferID(int iBinID)
{
	size_t nGroupSize = vWaferGroups.size();
	if (nGroupSize < 1)
		return;
	for (int i = 0; i < nGroupSize; ++i) {
		CWaferGroup* pGroup = vWaferGroups[i];
		for (auto& item : pGroup->vWaferMetas) {
			item.SetWaferID(iBinID);
		}
	}
}
