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
#include "..\stdafx.h"
#include "PluginManager.h"
#include "tinyxml2.h"
using namespace tinyxml2;

CPluginManager* CPluginManager::_instance = NULL;
//---------------------------------------------------------------------------
CPluginManager *CPluginManager::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CPluginManager();
    }
    return _instance;
}
CPluginManager::CPluginManager()
{    
    LoadConfig();    
}
CPluginManager::~CPluginManager()
{
    SaveConfig();
}
void CPluginManager::LoadConfig(void)
{
    XmlDocument doc;
    if(XML_SUCCESS!=doc.LoadFile( "config/plugin.xml" ))
        return;
    XmlElement* root = doc.FirstChildElement("Mark");

    /*XMLElement* eleIn = root->FirstChildElement("PinIn");
    TPortSign_t portIn;
    for (int i = 0; i < 16; i++)
    {
        eleIn->QueryIntAttribute("ID", &portIn.iID);
        eleIn->QueryIntAttribute("Pin", &portIn.iPin);
        eleIn->QueryIntAttribute("Sign", &portIn.iSign);
        eleIn->QueryBoolAttribute("Enable", &portIn.IsEnable);
        eleIn->QueryIntAttribute("Period", &portIn.iPeriod);
        m_vPortSignIns[portIn.iID] = portIn;

        eleIn = eleIn->NextSiblingElement("PinIn");
    }*/
}
void CPluginManager::SaveConfig(void)
{
    XmlDocument doc;
    XmlNode* root = doc.InsertEndChild( doc.NewElement( "Mark" ) );

    XmlElement* eleIn;
    /*TPortSign_t portIn;
    for (int i = 0; i < 16; i++)
    {
        portIn = m_vPortSignIns[i];
        eleIn = root->InsertEndChild(doc.NewElement("PinIn"))->ToElement();

        eleIn->SetAttribute("ID", i);
        eleIn->SetAttribute("Pin", portIn.iPin);
        eleIn->SetAttribute("Sign", portIn.iSign);
        eleIn->SetAttribute("Enable", portIn.IsEnable);
        eleIn->SetAttribute("Period", portIn.iPeriod);
    }*/

    doc.SaveFile( "config/plugin.xml" );
}


