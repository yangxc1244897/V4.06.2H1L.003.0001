#include "stdafx.h"
#include "WaferLayer.h"
#include "tlGraphFile.h"
#include "tlMarkDevice.h"
#include "WaferNode.h"

//int clPenColor[] = { 0x000000,0x000080,0xff0000,0xff00ff,0x800080,0x008080,0x808000 };

CWaferGraphLayer::CWaferGraphLayer(void)
{
    m_IsLocked = false;
    m_IsMarkable = true;
    m_IsVisible = true;
    m_iLineColor = 0x000000;
}

CWaferGraphLayer::~CWaferGraphLayer(void)
{
	if (nullptr != m_pThePen)
	{
		delete m_pThePen;   // 暂时采用此种方式删除，
		m_pThePen = nullptr;
	}
}

void CWaferGraphLayer::SetName(const char *sName)
{
	strcpy_s(m_sName, sizeof(m_sName), sName);
}

const char *CWaferGraphLayer::GetName(void) const
{
	return m_sName;
}

void CWaferGraphLayer::SetPen(IMarkPen *pThePen)
{
	m_pThePen = pThePen;
}

IMarkPen *CWaferGraphLayer::GetPen(void)
{
	return m_pThePen;
}

void CWaferGraphLayer::SetLock(bool lock)
{
	m_IsLocked = lock;
}

bool CWaferGraphLayer::IsLocked(void) const
{
	return m_IsLocked;
}

void CWaferGraphLayer::SetVisible(bool visible)
{
	m_IsVisible = visible;
}

bool CWaferGraphLayer::IsVisible(void) const
{
	return m_IsVisible;
}

void CWaferGraphLayer::SetMarkable(bool mark)
{
	m_IsMarkable = mark;
}

bool CWaferGraphLayer::IsMarkable(void) const
{
	return m_IsMarkable;
}

bool CWaferGraphLayer::LoadFile(IFileReader *pTheReader)
{
	if (NULL == pTheReader) return false;

	bool bRet = true;
	uint32_t code;
	while (CN_LAYER_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_LAYER_MARKABLE:
				m_IsMarkable = pTheReader->GetBool();
				break;
			case CN_LAYER_LOCKED:
				m_IsLocked = pTheReader->GetBool();
				break;
			case CN_LAYER_VISIBLE:
				m_IsVisible = pTheReader->GetBool();
				break;
			case CN_LAYER_NAME:
				pTheReader->GetString(m_sName);
				break;
			case CN_LAYER_LINECOLOR:
				m_iLineColor = pTheReader->GetInt32();
				break;
			case CN_LAYER_PEN:
				if (!m_pThePen->LoadFile(pTheReader))
					bRet = false;
				break;
			default:break;
			}
		}
	}
	return bRet;
}

bool CWaferGraphLayer::SaveFile(IFileWriter *pTheWriter)
{
	if (NULL == pTheWriter) return false;
	
	pTheWriter->WriteBegin(CN_LAYER_BASE);		// 图层开始标志

	pTheWriter->WriteBool(CN_LAYER_MARKABLE, m_IsMarkable);
	pTheWriter->WriteBool(CN_LAYER_LOCKED, m_IsLocked);
	pTheWriter->WriteBool(CN_LAYER_VISIBLE, m_IsVisible);

	pTheWriter->WriteString(CN_LAYER_NAME, m_sName);
	pTheWriter->WriteInt32(CN_LAYER_LINECOLOR, m_iLineColor);

	//--激光笔
	pTheWriter->WriteInt32(CN_LAYER_PEN, 0);
	m_pThePen->SaveFile(pTheWriter);  

	pTheWriter->WriteEnd(CN_LAYER_BASE);	// 图层结束标志

	return true;
}

