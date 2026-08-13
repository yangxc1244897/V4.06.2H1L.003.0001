/**
******************************************************************************
* @模块 
* @作者 jchq
* @版本 V1.00.00
* @日期 2018-09-21
* @简述 
* @email:jchq0101net@sina.com
******************************************************************************
* @说明 
*    
*    
*    
*      
*        
*    
*    
******************************************************************************
*/
#include "stdafx.h"
#include "WaferEngine.h"
#include "WaferFile.h"

CWaferEngine* CWaferEngine::_instance = NULL;
//---------------------------------------------------------------------------
CWaferEngine *CWaferEngine::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CWaferEngine();
    }
    return _instance;
}
CWaferEngine::CWaferEngine()
{
	m_pGraphDoc = new CWaferGraphDoc;
	m_pMapDoc = new CWaferMapDoc;
	m_iFileVersion = 1000;
}
CWaferEngine::~CWaferEngine()
{
	delete m_pGraphDoc;
	delete m_pMapDoc;
}

bool CWaferEngine::LoadGraphDoc(const char *sName)
{
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(sName))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	bool bRet = true;
	int iDocCount = 0;
	uint32_t code;
	while (CN_FILE_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_FILE_VER:
				m_iFileVersion = pTheReader->GetInt32();
				break;
			case CN_FILE_WAFERDOC:
				if (!m_pGraphDoc->LoadFile(pTheReader))
					bRet = false;
				break;
			default:
				break;
			}
		}
	}
	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;

	m_pGraphDoc->SetName(sName);

	return bRet;
}

bool CWaferEngine::LoadDocWithoutPen(const char *sName, std::vector<string> & vsPenName)
{
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(sName))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	bool bRet = true;
	int iDocCount = 0;
	uint32_t code;
	while (CN_FILE_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_FILE_VER:
				m_iFileVersion = pTheReader->GetInt32();
				break;
			case CN_FILE_WAFERDOC:
				if (!m_pGraphDoc->LoadFileWithoutPen(pTheReader, vsPenName))
					bRet = false;
				break;
			default:
				break;
			}
		}
	}
	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;

	m_pGraphDoc->SetName(sName);

	return bRet;
}


bool CWaferEngine::SaveGraphDoc(const char *sName)
{
	CFileWriter *pTheWriter = new CFileWriter();
	if (!pTheWriter->Open(sName))
	{
		delete pTheWriter;
		pTheWriter = NULL;
		return false;
	}
	pTheWriter->WriteBegin(CN_FILE_BASE);
	pTheWriter->WriteInt32(CN_FILE_VER, (int32_t)m_iFileVersion);

	pTheWriter->WriteInt32(CN_FILE_WAFERDOC, 0);
	m_pGraphDoc->SaveFile(pTheWriter);
	
	pTheWriter->WriteEnd(CN_FILE_BASE);
	pTheWriter->Close();
	delete pTheWriter;
	pTheWriter = NULL;

	m_pGraphDoc->SetName(sName);  

	return true;
}

bool CWaferEngine::LoadMapDoc(const char *sName)
{
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(sName))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	bool bRet = true;
	int iDocCount = 0;
	CWaferGraphDoc *pTheDoc = NULL;
	uint32_t code;
	while (CN_FILE_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_FILE_VER:
				m_iFileVersion = pTheReader->GetInt32();
				break;
			case CN_FILE_WAFERDOC:
				m_pGraphDoc->LoadFile(pTheReader);   // 因为只会使用文档中的MAP内容，所以印章加载失败也没关系
				break;
			case CN_FILE_WAFERMAP:
				m_pMapDoc->LoadFile(pTheReader);
				m_pMapDoc->DeleteAllTempFile();
				m_pMapDoc->SetModified(false, true);  // 只添加到撤销列表，不修改更改模式
				break;
			default:
				break;
			}
		}
	}
	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;

	SetFileName(sName);

	return bRet;
}
bool CWaferEngine::SaveMapDoc(const char *sName)
{
	return false;
}

bool CWaferEngine::LoadFile(const char *sName)
{
	CFileReader *pTheReader = new CFileReader();
	if (!pTheReader->Open(sName))
	{
		delete pTheReader;
		pTheReader = NULL;
		return false;
	}
	bool bRet = true;
	int iDocCount = 0;
	CWaferGraphDoc *pTheDoc = NULL;
	uint32_t code;
	while (CN_FILE_BASE == pTheReader->ReadData(&code))
	{
		if (CODED_VERIFY(code))
			break;
		else
		{
			switch (code)
			{
			case CN_FILE_VER:
				m_iFileVersion = pTheReader->GetInt32();
				break;
			case CN_FILE_WAFERDOC:
				if (!m_pGraphDoc->LoadFile(pTheReader))
					bRet = false;
				break;
			case CN_FILE_WAFERMAP:
				if (!m_pMapDoc->LoadFile(pTheReader))
					bRet = false;
				m_pMapDoc->DeleteAllTempFile();
				m_pMapDoc->SetModified(false, true);  // 只添加到撤销列表，不修改更改模式
				break;
			default:
				break;
			}
		}
	}
	pTheReader->Close();
	delete pTheReader;
	pTheReader = NULL;

	SetFileName(sName);

	return bRet;
}

bool CWaferEngine::SaveFile(const char * sName)
{
	CFileWriter *pTheWriter = new CFileWriter();
	if (!pTheWriter->Open(sName))
	{
		delete pTheWriter;
		pTheWriter = NULL;
		return false;
	}
	pTheWriter->WriteBegin(CN_FILE_BASE);
	pTheWriter->WriteInt32(CN_FILE_VER, (int32_t)m_iFileVersion);

	pTheWriter->WriteInt32(CN_FILE_WAFERDOC, 0);
	m_pGraphDoc->SaveFile(pTheWriter);

	pTheWriter->WriteInt32(CN_FILE_WAFERMAP, 0);
	m_pMapDoc->SaveFile(pTheWriter);

	pTheWriter->WriteEnd(CN_FILE_BASE);
	pTheWriter->Close();
	delete pTheWriter;
	pTheWriter = NULL;

	SetFileName(sName);

	return true;
}

bool CWaferEngine::GetModified(void)
{
	return (m_pGraphDoc->GetModified() || m_pMapDoc->GetModified());
}

void CWaferEngine::ResetModified(void)
{
	m_pGraphDoc->SetModified(false);
	m_pMapDoc->SetModified(false);
}

void CWaferEngine::RemoveAllDoc()
{
	m_pMapDoc->RemoveAllGroup();
	m_pGraphDoc->RemoveAllWafer();
	m_pGraphDoc->DeleteAllLayer();
	//m_pGraphDoc->AppendLayer();
	//m_pGraphDoc->SetActiveLayer(0);
	m_pGraphDoc->SetStripMap(false);
}
