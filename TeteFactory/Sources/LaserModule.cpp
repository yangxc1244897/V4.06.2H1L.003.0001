/**
******************************************************************************
* @文件  
* @作者
* @版本 V1.00.00
* @日期 2018-09-11
* @简述
* @email:jchq0101net@sina.com
******************************************************************************
* @说明
*  
*
******************************************************************************
*/
#include "stdafx.h"
#include "LaserModule.h"
#include "tinyxml2.h"
using namespace tinyxml2;
CLaserModule::CLaserModule()
{
    strcpy_s(m_sFileName,MAX_FILE_NAME, GetSystemDirFile("Module\\modules.xml"));
}
CLaserModule::~CLaserModule()
{}
tlModule_t CLaserModule::GetDefault(void)
{
    tlModule_t module;
    XmlDocument doc;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
    {
        strcpy_s(module.sName,MAX_TEXT_SIZE,""); 
        strcpy_s(module.sFile,MAX_FILE_NAME,""); 
        return module;
    }
    XmlElement* root = doc.RootElement();
        
    XmlElement* eleModule = root->FirstChildElement("Default");
    if (NULL != eleModule)
    {
        strcpy_s(module.sName, MAX_TEXT_SIZE, eleModule->Attribute("Name"));
        strcpy_s(module.sFile, MAX_FILE_NAME, eleModule->Attribute("File"));
    }
    else
    {
        strcpy_s(module.sName, MAX_TEXT_SIZE, "");
        strcpy_s(module.sFile, MAX_FILE_NAME, "");
    }
    return module;
}
void CLaserModule::SetDefault(const tlModule_t *pModule)
{
    XmlDocument doc;
    XmlNode* root = NULL;
    XmlElement* eleModule = NULL;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
    {
        root = doc.InsertEndChild( doc.NewElement( "Root" ) );
        eleModule = root->InsertEndChild(doc.NewElement( "Default" ))->ToElement();
    }
    else
    {
        root = doc.RootElement();
        eleModule = root->FirstChildElement("Default");
        if(NULL==eleModule)
            eleModule = root->InsertEndChild(doc.NewElement( "Default" ))->ToElement();
    }
    
    eleModule->SetAttribute("Name", pModule->sName); 
    eleModule->SetAttribute("File", pModule->sFile);    
    doc.SaveFile(m_sFileName);
}
int32_t CLaserModule::QueryModule(int32_t iModuleID, tlModule_t * pModule)
{
    if (NULL == pModule)
        return -1;
    XmlDocument doc;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
    {
        return -1;
    }
    XmlElement* root = doc.RootElement();
    int32_t iID=0;
    XmlElement* eleModule = root->FirstChildElement("Module");
    int iError = -1;
    while (eleModule)
    {
        if (iID == iModuleID)
        {
            strcpy_s(pModule->sName,MAX_TEXT_SIZE,eleModule->Attribute("Name")); 
            strcpy_s(pModule->sFile,MAX_FILE_NAME,eleModule->Attribute("File"));
            iError = 0;
            break;
        }
        iID++;
        eleModule = eleModule->NextSiblingElement();
    }
    return iError;
}
int32_t CLaserModule::QueryModule(const char *name, tlModule_t * pModule)
{
    XmlDocument doc;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
    {
        return -1;
    }
    XmlElement* root = doc.RootElement();
    XmlElement* eleModule = root->FirstChildElement("Module");
    int iError = -1;
    while (eleModule)
    {
        if (0==strcmp(name,eleModule->Attribute("Name")))
        {
            if (NULL != pModule)
            {
                strcpy_s(pModule->sName, MAX_TEXT_SIZE, eleModule->Attribute("Name"));
                strcpy_s(pModule->sFile, MAX_FILE_NAME, eleModule->Attribute("File"));
            }
            iError = 0;
            break;
        }
        eleModule = eleModule->NextSiblingElement();
    }
    return iError;
}
int32_t CLaserModule::LoadModule(tlModule_t * pModule)
{
    if (NULL == pModule)
        return -1;
    return QueryModule(pModule->sName,pModule);
}
int32_t CLaserModule::SaveModule(const tlModule_t * pModule)
{
    if (NULL == pModule)
        return -1;
    if (0 == QueryModule(pModule->sName, NULL))
        return -1;
    XmlDocument doc;
    XmlNode* root = NULL;
    XmlElement* eleModule = NULL;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
        root = doc.InsertEndChild( doc.NewElement( "Root" ) );
    else
        root = doc.RootElement();
    eleModule = root->InsertEndChild(doc.NewElement( "Module" ))->ToElement();
    eleModule->SetAttribute("Name", pModule->sName); 
    eleModule->SetAttribute("File", pModule->sFile);

    doc.SaveFile( m_sFileName );
    return 0;
}
int32_t CLaserModule::ModifyModule(const char *name, const tlModule_t * pModule)
{
    if (NULL == pModule)
        return -1;
    XmlDocument doc;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
    {
        return -1;
    }
    XmlElement* root = doc.RootElement();
    XmlElement* eleModule = root->FirstChildElement("Module");
    int iError = -1;
    while (eleModule)
    {
        if (0==strcmp(name,eleModule->Attribute("Name")))
        {
            eleModule->SetAttribute("Name", pModule->sName); 
            eleModule->SetAttribute("File", pModule->sFile);
            iError = 0;
            break;
        }
        eleModule = eleModule->NextSiblingElement();
    }

    doc.SaveFile(m_sFileName);
    
    return 0;
}
int32_t CLaserModule::RemoveModule(int32_t iID)
{
    XmlDocument doc;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
    {
        return -1;
    }
    XmlElement* root = doc.RootElement();
    XmlElement* eleModule = root->FirstChildElement("Module");
    int iError = -1;
    int32_t iIndex = 0;
    while (eleModule)
    {
        if (iIndex==iID)
        {
            root->DeleteChild(eleModule);
            iError = 0;
            break;
        }
        iIndex++;
        eleModule = eleModule->NextSiblingElement();
    }

    doc.SaveFile(m_sFileName);

    return 0;
}
int32_t CLaserModule::GetAllModule(std::vector<tlModule_t> &Modules)
{
    XmlDocument doc;
    if (XML_SUCCESS != doc.LoadFile(m_sFileName))
    {
        return -1;
    }
    XmlElement* root = doc.RootElement();
    XmlElement* eleModule = root->FirstChildElement("Module");
    Modules.clear();
    while (eleModule)
    {
        tlModule_t module;
        strcpy_s(module.sName,MAX_TEXT_SIZE,eleModule->Attribute("Name"));
        strcpy_s(module.sFile,MAX_FILE_NAME,eleModule->Attribute("File"));        
        Modules.push_back(module);
        eleModule = eleModule->NextSiblingElement("Module");
    }
    return Modules.size();
}



