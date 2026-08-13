/**
  ******************************************************************************
  * @模块 
  * @作者 jchq
  * @版本 V1.00.00
  * @日期 2017-09-21
  * @email:jchq0101net@sina.com
  ******************************************************************************
  * @功能描述
  * 
  *   
  *
  *
  *
  ******************************************************************************
  */

#include "stdafx.h"
#include "ActionFactory.h"
#include "ThirdParty\TinyXML\tinyxml.h"
#include "tinyxml2.h"
#include "MarkAction.h"
#include "Common\LogServer.h"
#include <algorithm>

CActionFactory* CActionFactory::_instance = NULL;
//---------------------------------------------------------------------------
CActionFactory *CActionFactory::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CActionFactory();
    }
    return _instance;
}

CActionFactory::CActionFactory()
{
    m_theActionsList.clear();
}
CActionFactory::~CActionFactory()
{
    size_t size = m_theActionsList.size();
    IMarkAction *pTheAction = NULL;
	for(size_t i=0;i<size;i++)
	{
		pTheAction = m_theActionsList[i];
		delete pTheAction;
	}
    m_theActionsList.clear();
}
IMarkAction *CActionFactory::NewAction(int iActionType)
{
	IMarkAction *pTheAction = NULL;
	switch (iActionType)
	{
	case emACTION_INPUT:
		pTheAction = new CActionInput;
		break;
	case emACTION_OUTPUT:
		pTheAction = new CActionOutput;
		break;    
	case emACTION_DELAY:
		pTheAction = new CActionDelay;
		break;
    case emACTION_JUMP:
        pTheAction = new CActionJump;
        break;
	case emACTION_MARKDOC:
		pTheAction = new CActionMarkDoc;
		break;
	case emACTION_MOTION:
		pTheAction = new CActionMotion;
		break;
    case emACTION_VISION:
		pTheAction = new CActionVision;
		break;
	default:break;
	}
	if (NULL != pTheAction)
	{
        pTheAction->SetID(m_theActionsList.size());
        pTheAction->Init(m_pPlatform);
		m_theActionsList.push_back(pTheAction);
	}
	return pTheAction;
}
void CActionFactory::DeleteAction(IMarkAction *pTheAction)
{
    if (NULL==pTheAction)
		return;

	size_t size = m_theActionsList.size();
    IMarkAction *pAction = NULL;
	std::vector<IMarkAction *>::iterator itt;
    for (itt = m_theActionsList.begin(); itt != m_theActionsList.end();)
	{
		pAction = (IMarkAction *)*itt;
        if (pTheAction == pAction)
        {
            itt = m_theActionsList.erase(itt);
            delete pAction;
            pAction = NULL;
            break;
        }
        else
            itt++;
	}
}
IMarkAction *CActionFactory::GetAction(int32_t iActionID)
{
    if((iActionID<0)||(iActionID>=m_theActionsList.size()))
        return NULL;
	    
    return m_theActionsList[iActionID];
}
IMarkAction *CActionFactory::FirstAction(void)
{
    if(m_theActionsList.size()==0)
            return NULL;
    return m_theActionsList[0];
}
IMarkAction *CActionFactory::NextAction(IMarkAction *pTheAction)
{
    size_t size = m_theActionsList.size();
    if(size<1)
        return NULL;
    IMarkAction *pAction = NULL;
    size_t id;
    if (emACTION_JUMP == pTheAction->GetActionType())
    {
        id = ((CActionJump *)pTheAction)->dwJumpID;
    }
    else
        id = pTheAction->GetID()+1;
    
    if (id >= size)
        return NULL;//--return m_theActionsList[0];
    return m_theActionsList[id];
}
IMarkAction *CActionFactory::LastAction(void)
{
    size_t size = m_theActionsList.size();
    if(size<1)
        return NULL;
    return m_theActionsList[size-1];
}
int32_t CActionFactory::ActiveAction(int32_t iActionID)
{
    IMarkAction *pTheAction = GetAction(iActionID);
    if(NULL==pTheAction)
        return -1;
    
    return pTheAction->ActiveAction();
    
}
void CActionFactory::EnableActive(bool IsEnable)
{
    if (IsEnable)
    {
        m_IsActiveEnable = true;
        size_t size = m_theActionsList.size();
        IMarkAction *pTheAction = NULL;
        for(size_t i=0;i<size;i++)
        {
            pTheAction = m_theActionsList[i];
            pTheAction->EnableAction(true);
        }
    }
    else
    {
        m_IsActiveEnable = false;
        size_t size = m_theActionsList.size();
        IMarkAction *pTheAction = NULL;
        for(size_t i=0;i<size;i++)
        {
            pTheAction = m_theActionsList[i];
            pTheAction->EnableAction(false);
        }
    }
}
bool CActionFactory::ImportActions(const char *sFileName)
{
#if 0
    CFileReader *pTheReader = new CFileReader();
    if(!pTheReader->Open(sFileName))
    {
        delete pTheReader;
        pTheReader =NULL;
        return false;
    }
    uint32_t code;
    while (pTheReader->ReadData(&code))
    {
        if(0x00000000==(code&0x00FFFF00))  //--起始码
            continue;
        else if(0x00FFFF00==(code&0x00FFFF00))  //--结束码
            break;
        if (CN_FILE_VER == code)
        {
        }
        else if(CN_ACTION_BASE == code)
        {
            while (pTheReader->ReadData(&code))
            {
                if (0x00FFFF00 == (code & 0x00FFFF00))  //--结束码
                    break;
                if (CN_ACTION_TYPE == code)
                {
                    int iActionType = pTheReader->GetInt32();
                    if(emACTION_INPUT==iActionType)
                    {
                        CActionInput *pTheAction = new CActionInput();
                        string_t sCaption = theActionElement->FirstChildElement("Caption")->GetText();
                        pTheAction->SetCaption(sCaption);
                        pTheAction->iSign = atoi(theActionElement->FirstChildElement("Sign")->GetText());
                        pTheAction->dwPins = atoi(theActionElement->FirstChildElement("Pins")->GetText());
                        m_pActionsList->AddTail(pTheAction);
                    }
                    else if(emACTION_OUTPUT==iActionType)
                    {}
                    else if(emACTION_DELAY==iActionType)
                    {}
                    else if(emACTION_MARKDOC==iActionType)
                    {}
                    else if(emACTION_MOTION==iActionType)
                    {}
                }
            }
        }
    }
    pTheReader->Close();
    delete pTheReader;
    pTheReader = NULL;
#else
    tinyxml2::XmlDocument TheXmlFile;
    if (tinyxml2::XML_SUCCESS!=TheXmlFile.LoadFile(sFileName))
        return false;

    tinyxml2::XmlElement *theRootElement = TheXmlFile.RootElement();
    
    tinyxml2::XmlElement *theActionElement;
    //--IMarkAction *pTheAction = NULL;
    theActionElement = theRootElement->FirstChildElement("Action");
    while(NULL != theActionElement)
    {
        int iActionType = atoi(theActionElement->Attribute("Type"));        
        if(emACTION_INPUT==iActionType)
        {
            CActionInput *pTheAction = (CActionInput *)NewAction(iActionType);
            const char *sCaption = theActionElement->FirstChildElement("Caption")->GetText();
            pTheAction->SetCaption(sCaption);
            pTheAction->iSign = atoi(theActionElement->FirstChildElement("Sign")->GetText());
            pTheAction->dwPins = atoi(theActionElement->FirstChildElement("Pins")->GetText());
            //--m_theActionsList.push_back(pTheAction);
        }
        else if(emACTION_OUTPUT==iActionType)
        {
            CActionOutput *pTheAction = (CActionOutput *)NewAction(iActionType);
            const char *sCaption = theActionElement->FirstChildElement("Caption")->GetText();
            pTheAction->SetCaption(sCaption);
            pTheAction->iSign = atoi(theActionElement->FirstChildElement("Sign")->GetText());
            pTheAction->dwPins = atoi(theActionElement->FirstChildElement("Pins")->GetText());
            //--m_theActionsList.push_back(pTheAction);
        }
        else if(emACTION_DELAY==iActionType)
        {
            CActionDelay *pTheAction = (CActionDelay *)NewAction(iActionType);
            const char *sCaption = theActionElement->FirstChildElement("Caption")->GetText();
            pTheAction->SetCaption(sCaption);
            pTheAction->dwmin = atoi(theActionElement->FirstChildElement("Min")->GetText());
            pTheAction->dwsec = atoi(theActionElement->FirstChildElement("Sec")->GetText());
            pTheAction->dwmsec = atoi(theActionElement->FirstChildElement("mSec")->GetText());
            //--m_theActionsList.push_back(pTheAction);
        }
        else if(emACTION_JUMP==iActionType)
        {
            CActionJump *pTheAction = (CActionJump *)NewAction(iActionType);      
            const char *sCaption = theActionElement->FirstChildElement("Caption")->GetText();
            pTheAction->SetCaption(sCaption);
            pTheAction->dwJumpID = atoi(theActionElement->FirstChildElement("JumpID")->GetText());
            //--m_theActionsList.push_back(pTheAction);
        }
        else if(emACTION_MARKDOC==iActionType)
        {
            CActionMarkDoc *pTheAction = (CActionMarkDoc *)NewAction(iActionType); 
            const char *sCaption = theActionElement->FirstChildElement("Caption")->GetText();
            pTheAction->SetCaption(sCaption);
            pTheAction->iDocID = atoi(theActionElement->FirstChildElement("DocID")->GetText());
            pTheAction->iMarkTimes = atoi(theActionElement->FirstChildElement("Times")->GetText());
            //--m_theActionsList.push_back(pTheAction);
        }
        else if(emACTION_MOTION==iActionType)
        {
            CActionMotion *pTheAction = (CActionMotion *)NewAction(iActionType); 
            const char *sCaption = theActionElement->FirstChildElement("Caption")->GetText();
            pTheAction->SetCaption(sCaption);
            /*pTheAction->mp.x = atoi(theActionElement->FirstChildElement("X")->GetText());
            pTheAction->mp.y = atoi(theActionElement->FirstChildElement("Y")->GetText());
            pTheAction->mp.z = atoi(theActionElement->FirstChildElement("Z")->GetText());
            pTheAction->mp.a = atoi(theActionElement->FirstChildElement("A")->GetText());
            pTheAction->mp.b = atoi(theActionElement->FirstChildElement("B")->GetText());*/
            //--m_theActionsList.push_back(pTheAction);
        }
        
        theActionElement = theActionElement->NextSiblingElement("Action");
    }
#endif
    return true;
}
bool CActionFactory::ExportActions(const char *sFileName)
{   
#if 0
    CFileWriter *pTheWriter = new CFileWriter();
    if(!pTheWriter->Open(sFileName))
    {
        delete pTheWriter;
        pTheWriter =NULL;
        return false;
    }
    pTheWriter->WriteBegin(CN_FILE_BASE);                                                
    pTheWriter->WriteInt32(CN_FILE_VER, 10000);

    //--遍历文档，保存文档内容
    IMarkAction *pAction = NULL; 
    POSITION pos = m_pActionsList->GetHeadPosition();
    while (NULL != pos)
    {
        pAction = (IMarkAction *)m_pActionsList->GetNext(pos);
        pTheWriter->WriteBegin(CN_ACTION_BASE);
        int iActionType = pAction->GetActionType();
        pTheWriter->WriteInt32(CN_ACTION_TYPE, iActionType);
        pTheWriter->WriteString(CN_ACTION_TYPE, pAction->GetCaption());
        if(emACTION_INPUT==iActionType)
        {
            CActionInput *pTheAction = (CActionInput *)pAction;     
            pTheWriter->WriteInt32(CN_ACTION_SIGN, pTheAction->iSign);
            pTheWriter->WriteInt32(CN_ACTION_PINS, pTheAction->dwPins);
        }
        else if(emACTION_OUTPUT==iActionType)
        {
            CActionOutput *pTheAction = (CActionOutput *)pAction;                        
            pTheWriter->WriteInt32(CN_ACTION_SIGN, pTheAction->iSign);
            pTheWriter->WriteInt32(CN_ACTION_PINS, pTheAction->dwPins);
        }
        else if(emACTION_DELAY==iActionType)
        {
            CActionDelay *pTheAction = (CActionDelay *)pAction;      
            pTheWriter->WriteInt32(CN_ACTION_DMIN, pTheAction->dwmin);
            pTheWriter->WriteInt32(CN_ACTION_DSEC, pTheAction->dwsec);
            pTheWriter->WriteInt32(CN_ACTION_MSEC, pTheAction->dwmsec);
        }
        else if(emACTION_JUMP==iActionType)
        {
            CActionJump *pTheAction = (CActionJump *)pAction;                        
            pTheWriter->WriteInt32(CN_ACTION_JUMP, pTheAction->dwJumpID);
        }
        else if(emACTION_MARKDOC==iActionType)
        {
            CActionMarkDoc *pTheAction = (CActionMarkDoc *)pAction;        
            pTheWriter->WriteInt32(CN_ACTION_DOCID, pTheAction->iDocID);
            pTheWriter->WriteInt32(CN_ACTION_TIMES, pTheAction->iMarkTimes);
        }
        else if(emACTION_MOTION==iActionType)
        {
            CActionMotion *pTheAction = (CActionMotion *)pAction;    
            pTheWriter->WriteInt32(CN_ACTION_MX, pTheAction->mp.x);
            pTheWriter->WriteInt32(CN_ACTION_MY, pTheAction->mp.y);
            pTheWriter->WriteInt32(CN_ACTION_MZ, pTheAction->mp.z);
            pTheWriter->WriteInt32(CN_ACTION_MA, pTheAction->mp.a);
            pTheWriter->WriteInt32(CN_ACTION_MB, pTheAction->mp.b);
        }
        else if(emACTION_VISION==iActionType)
        {
            CActionVision *pTheAction = (CActionVision *)pAction;    
            pTheWriter->WriteInt32(CN_ACTION_OX, pTheAction->dbOffsetX);
            pTheWriter->WriteInt32(CN_ACTION_OY, pTheAction->dbOffsetY);
            pTheWriter->WriteInt32(CN_ACTION_RA, pTheAction->dbRotateA);
        }
        pTheWriter->WriteEnd(CN_ACTION_BASE);
    }

    pTheWriter->WriteEnd(CN_FILE_BASE);                             	
    pTheWriter->Close();      
    delete pTheWriter;
    pTheWriter = NULL;

#else
    TiXmlDocument * pTheDoc = new TiXmlDocument(sFileName);       
    pTheDoc->InsertEndChild(TiXmlDeclaration("1.0","UTF-8","no"));
    TiXmlElement *theRootElement = pTheDoc->InsertEndChild(TiXmlElement("TETE_Actions"))->ToElement();
    theRootElement->SetAttribute("xmlns","http://www.tetelaser.com.cn");
    theRootElement->SetAttribute("xmlns:mf","http://www.tetelaser.com.cn/markpen");
    
    TiXmlElement *theActionElement;
    size_t size = m_theActionsList.size();
    IMarkAction *pAction = NULL;
    /*char sCation[512];*/
    for(size_t i=0;i<size;i++)
	{
		pAction = m_theActionsList[i];
        int iActionType = pAction->GetActionType();
        theActionElement = theRootElement->InsertEndChild(TiXmlElement("Action"))->ToElement();
        theActionElement->SetAttribute("Type", iActionType);

        theActionElement->InsertEndChild(TiXmlElement("Caption"))->InsertEndChild(TiXmlText(pAction->GetCaption()));
        if(emACTION_INPUT==iActionType)
        {
            CActionInput *pTheAction = (CActionInput *)pAction;                        
            theActionElement->InsertEndChild(TiXmlElement("Sign"))->InsertEndChild(TiXmlText(pTheAction->iSign));
            theActionElement->InsertEndChild(TiXmlElement("Pins"))->InsertEndChild(TiXmlText(pTheAction->dwPins));
        }
        else if(emACTION_OUTPUT==iActionType)
        {
            CActionOutput *pTheAction = (CActionOutput *)pAction;                        
            theActionElement->InsertEndChild(TiXmlElement("Sign"))->InsertEndChild(TiXmlText(pTheAction->iSign));
            theActionElement->InsertEndChild(TiXmlElement("Pins"))->InsertEndChild(TiXmlText(pTheAction->dwPins));
        }
        else if(emACTION_DELAY==iActionType)
        {
            CActionDelay *pTheAction = (CActionDelay *)pAction;                          
            theActionElement->InsertEndChild(TiXmlElement("Min"))->InsertEndChild(TiXmlText(pTheAction->dwmin));
            theActionElement->InsertEndChild(TiXmlElement("Sec"))->InsertEndChild(TiXmlText(pTheAction->dwsec));
            theActionElement->InsertEndChild(TiXmlElement("mSec"))->InsertEndChild(TiXmlText(pTheAction->dwmsec));
        }
        else if(emACTION_JUMP==iActionType)
        {
            CActionJump *pTheAction = (CActionJump *)pAction;                        
            theActionElement->InsertEndChild(TiXmlElement("JumpID"))->InsertEndChild(TiXmlText(pTheAction->dwJumpID));
        }
        else if(emACTION_MARKDOC==iActionType)
        {
            CActionMarkDoc *pTheAction = (CActionMarkDoc *)pAction;                         
            theActionElement->InsertEndChild(TiXmlElement("DocID"))->InsertEndChild(TiXmlText(pTheAction->iDocID));
            theActionElement->InsertEndChild(TiXmlElement("Times"))->InsertEndChild(TiXmlText(pTheAction->iMarkTimes));
        }
        else if(emACTION_MOTION==iActionType)
        {
            CActionMotion *pTheAction = (CActionMotion *)pAction;                       
            /*theActionElement->InsertEndChild(TiXmlElement("X"))->InsertEndChild(TiXmlText(pTheAction->mp.x));
            theActionElement->InsertEndChild(TiXmlElement("Y"))->InsertEndChild(TiXmlText(pTheAction->mp.y));
            theActionElement->InsertEndChild(TiXmlElement("Z"))->InsertEndChild(TiXmlText(pTheAction->mp.z));
            theActionElement->InsertEndChild(TiXmlElement("A"))->InsertEndChild(TiXmlText(pTheAction->mp.a));
            theActionElement->InsertEndChild(TiXmlElement("B"))->InsertEndChild(TiXmlText(pTheAction->mp.b));*/
        }
        else if(emACTION_VISION==iActionType)
        {
            CActionVision *pTheAction = (CActionVision *)pAction;                         
            theActionElement->InsertEndChild(TiXmlElement("OffsetX"))->InsertEndChild(TiXmlText(pTheAction->dbOffsetX));
            theActionElement->InsertEndChild(TiXmlElement("OffsetY"))->InsertEndChild(TiXmlText(pTheAction->dbOffsetY));
            theActionElement->InsertEndChild(TiXmlElement("RotateA"))->InsertEndChild(TiXmlText(pTheAction->dbRotateA));
        }
	}

    pTheDoc->SaveFile();
    delete pTheDoc;
#endif
    return true;
}

//全局IMarkAction对象升序排序函数
bool lessaction(IMarkAction*  m1, IMarkAction* m2)
{
	return m1->GetID() < m2->GetID();
}

void CActionFactory::SortByID(const std::vector<int> &vid)
{
	IMarkAction* pTheAction = NULL;

	//修改动作ID
	int iSize = vid.size();
	int iActionCnt = GetActionCount();
	for (int i = 0; i < iSize; i++)
	{
		if (vid[i] >= 0 && vid[i] < iActionCnt
			&& vid[i] != i)
		{
			pTheAction = GetAction(vid[i]);
			pTheAction->SetID(i);
		}
	}

	//按照ID升序排序
	std::sort(m_theActionsList.begin(), m_theActionsList.end(), lessaction);
}