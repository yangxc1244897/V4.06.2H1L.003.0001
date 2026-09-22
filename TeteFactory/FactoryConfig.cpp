
#include "stdafx.h"
#include "FactoryConfig.h"

static const CString g_sConfigPath = _T("config\\factory.ini");
CFactoryConfig* CFactoryConfig::_instance = NULL;
//---------------------------------------------------------------------------
CFactoryConfig *CFactoryConfig::Instance()
{
    if(NULL==_instance)
    {
        _instance = new CFactoryConfig();
    }
    return _instance;
}


CFactoryConfig::CFactoryConfig()
{     
	m_bEnableDummy = false;
	m_bEnableFirstPrint = false;
	m_iMappingDebugSet = 0;
}
CFactoryConfig::~CFactoryConfig()
{
	SaveConfig();
}

void CFactoryConfig::LoadConfig(void)
{
	m_nWorkMode = GetPrivateProfileInt(_T("Global"), _T("WorkMode"), emWorkMode_Manual, g_sConfigPath);
	m_bAutoMark = (bool)GetPrivateProfileInt(_T("Global"), _T("AutoMark"), 1, g_sConfigPath);
	m_bAutoStartJob = (bool)GetPrivateProfileInt(_T("Global"), _T("AutoJob"), 0, g_sConfigPath);
	m_bOpenRMS = (bool)GetPrivateProfileInt(_T("Global"), _T("RMS"), 1, g_sConfigPath);
	m_bOpenRollTip = (bool)GetPrivateProfileInt(_T("Global"), _T("RollTip"), 1, g_sConfigPath);
	m_nPort = GetPrivateProfileInt(_T("Global"), _T("Port"), 8888, g_sConfigPath);
	//m_nStartMarkSignalType = GetPrivateProfileInt(_T("Global"), _T("StartMarkSignel"), 1, g_sConfigPath);  
	//m_nFinishMarkSignalType = GetPrivateProfileInt(_T("Global"), _T("FinishMarkSignel"), 0, g_sConfigPath);
	m_bMCCheckWarn = (bool)GetPrivateProfileInt(_T("Global"), _T("MCCheckWarn"), 1, g_sConfigPath);
	m_nRetryTimes = GetPrivateProfileInt(_T("Global"), _T("MCCheckRetryTimes"), 4, g_sConfigPath);
	TCHAR buff[1024] = { 0 };
	GetPrivateProfileString(_T("Path"), _T("MarkFilePath"), _T("D:\\YZ"), buff, 1024, g_sConfigPath);
	m_sMarkFilePath = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Path"), _T("LogFilePath"), _T("D:\\TeteLog"), buff, 1024, g_sConfigPath);
	m_sLogFilePath = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("FTPIP"), _T("127.0.0.1"), buff, 1024, g_sConfigPath);
	m_sFtpIP = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("WarnTip"), _T(""), buff, 1024, g_sConfigPath);
	m_sWarnTips = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("MESIP"), _T("127.0.0.1"), buff, 1024, g_sConfigPath);
	m_sMesIP = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("MobFilePath"), _T("D:\\YZ\\MOB"), buff, 1024, g_sConfigPath);
	m_sMobFilePath = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("DeviceNo"), _T("TETE00000001"), buff, 1024, g_sConfigPath);
	m_sDeviceNo = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("BarcodeHead"), _T("BB-KK-"), buff, 1024, g_sConfigPath);
	m_sBarcodeHead = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("MCCheckTip"), _T("警告：该产品需要送MC首检！"), buff, 1024, g_sConfigPath);
	m_sMCCheckTip = buff;
	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("MESURL"), _T("http://172.25.1.103:10085/LotinfoSvc.asmx"), buff, 1024, g_sConfigPath);
	m_sMesUrl = buff;

	m_bManulStamp = (bool)GetPrivateProfileInt(_T("Global"), _T("ManulStamp"), 0, g_sConfigPath);
	m_bCheckOrderStatus = (bool)GetPrivateProfileInt(_T("Global"), _T("CheckOrderStatus"), 1, g_sConfigPath);
	m_bMaxMarkTimesLimit = (bool)GetPrivateProfileInt(_T("Global"), _T("MaxMarkTimesLimit"), 0, g_sConfigPath);
	m_nMaxMarkTimes = GetPrivateProfileInt(_T("Global"), _T("MaxMarkTimes"), 100, g_sConfigPath);
	m_bPenOrder = true;  // GetData(_T("Global"), _T("PenOrder"), true);
	m_bCheck2DBarcode = GetData(_T("Global"), _T("Check2DBarcode"), true);
	m_bCheckStripMap = GetData(_T("Global"), _T("CheckStripMap"), true);
	m_bCheckVisualSystem = GetData(_T("Global"), _T("CheckVisualSystem"), true);
	m_bVisionCheck = GetData(_T("Global"), _T("VisionCheck"), false);
	m_bAddFilePrefix = GetData(_T("Global"), _T("AddFilePrefix"), false);
	m_bCheckEQPStripIdIsMatchLotId = GetData(_T("Global"), _T("CheckEQPStripIdIsMatchLotId"), false);
	m_bCheckProductType = GetData(_T("Global"), _T("CheckProductType"), false);

	m_sLotIDXMLPath = GetData(_T("HT"), _T("LotIDXMLPath"), CString(_T("D:\\XML")));
	m_sScripIDXMLPath = GetData(_T("HT"), _T("ScripIDXMLPath"), CString(_T("D:\\XML")));
	m_emUseScan = (emScannerType_t)GetData(_T("HT"), _T("UseScan"), emScannerType_Client1);
	m_sScannerIP[0] = GetData(_T("HT"), _T("ScannerIP_1"), CString(_T("127.0.0.1")));
	m_sScannerIP[1] = GetData(_T("HT"), _T("ScannerIP_2"), CString(_T("127.0.0.1")));
	m_nScannerPort[0] = GetData(_T("HT"), _T("ScannerPort_1"), 3000);
	m_nScannerPort[1] = GetData(_T("HT"), _T("ScannerPort_2"), 3001);
	m_nScannStartSignal[0] = GetData(_T("HT"), _T("ScannerStartSignal_1"), 0);
	m_nScannStartSignal[1] = GetData(_T("HT"), _T("ScannerStartSignal_2"), 1);
	m_nScannStartSignal2[0] = GetData(_T("HT"), _T("ScannerStartSignal2_1"), 2);
	m_nScannStartSignal2[1] = GetData(_T("HT"), _T("ScannerStartSignal2_2"), 3);
	m_nScannReadySingle = GetData(_T("HT"), _T("m_nScannReadySingle"),1);
	m_nScanUse2DPort = GetData(_T("HT"), _T("m_nScanUse2DPort"), 0);
	m_nScannOKSignal = GetData(_T("HT"), _T("ScanningOK"), 2);
	m_nScannNGSignal = GetData(_T("HT"), _T("ScanningNG"), 3);
	m_nScanWarnSignal = GetData(_T("HT"), _T("ScanningWarn"), 6);
	m_nBadSignal = GetData(_T("HT"), _T("ScanningBad"), 8);
	m_nScannTime = 500; // GetData(_T("HT"), _T("ScanTime"), 500);
	m_ExitMarkSignal = GetData(_T("HT"), _T("ExitMarkSignal"), 10);
	m_CleanMarkFileSignal = GetData(_T("HT"), _T("CleanMarkFileSignal"), 1);
	m_StopScannerSignal = GetData(_T("HT"), _T("StopScannerSignal"), 7);;
	m_nReadyPort = GetData(_T("HT"), _T("ReadyPort"), 0);
	m_nStartPort = GetData(_T("HT"), _T("StartPort"), 1);
	m_nBusyPort = GetData(_T("HT"), _T("BusyPort"), 2);
	m_nEndPort = GetData(_T("HT"), _T("EndPort"), 3);
	m_nCommIP = GetData(_T("HT"), _T("CommIP"), CString(_T("127.0.0.1")));
	m_nCommPort = GetData(_T("HT"), _T("CommPrrt"), 80);
	m_nScanInputTimeOutPort = GetData(_T("HT"), _T("ScanInputTimeOutPort"), 4);
	m_nScanEnablePWS = GetData(_T("HT"), _T("ScanEnablePWS"), 5);
	m_nScanPWSOK = GetData(_T("HT"), _T("ScanPWSOK"), 6);
	m_nScanPWSNG = GetData(_T("HT"), _T("ScanPWSNG"), 7);

	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("StampPath"), _T("D:\\YZ"), buff, 1024, g_sConfigPath);
	m_sStampPath = buff;

	m_sGFHPath = GetData(_T("Global"), _T("GFHPath"), CString(_T("D:\\GFH")));
	m_sLaserParamSave = GetData(_T("Global"), _T("LaserParamSave"), CString(_T("D:\\LaserParam")));

	m_sMesCheckUrl = GetData(_T("Global"), _T("MesCheckUrl"), CString(_T("http://172.25.1.103:18802/WebServiceOf2D.asmx")));
	m_sMesTrackInUrl = GetData(_T("Global"), _T("MesTrackInUrl"), CString(_T("http://172.20.1.115:10083/Service1.asmx")));
	m_sMesThickUrl = GetData(_T("Global"), _T("MesThickUrl"), CString(_T("http://172.25.1.103:10030")));
	m_sMesProductType = GetData(_T("Global"), _T("MesProductType"), CString(_T("http://172.20.1.153:4030")));

	m_s2DPath = GetData(_T("Global"), _T("2DContentPath"), CString(_T("D:\\HT DB")));
	m_nCheck2DBarcodeRepeat = GetData(_T("Global"), _T("Check2DBarcodeRepeat"), 2);
	m_nSavePeriod = GetData(_T("Global"), _T("SavePeriod"), 14);
	m_nPortPLC = GetData(_T("Global"), _T("PortPLC"), 8001);
	
	m_bCheckVisionSwitchProcess = GetData(_T("Global"), _T("bCheckVisionSwitchProcess"), true);
	m_bCheckPLCSwitchProcess = GetData(_T("Global"), _T("bCheckPLCSwitchProcess"), true);
	m_bCheckFinsPLC = GetData(_T("Global"), _T("bCheckFinsPLC"), true);

	m_iVisionCheckPort = GetData(_T("Global"), _T("m_iVisionCheckPort"), 6000);
	m_iFinsPort = GetData(_T("Global"), _T("m_iFinsPort"), 8080);

	//
	m_FinishedLotIP = GetData(_T("Global"), _T("m_FinishedLotIP"), CString(_T("172.25.1.103")));
	m_EnableFinishedLot = GetData(_T("Global"), _T("m_EnableFinishedLot"), false);
	m_RemotLotURL = GetData(_T("Global"), _T("m_RemotLotURL"), CString(_T("172.25.1.103")));
	m_EnableRemoteLotURL = GetData(_T("Global"), _T("m_EnableRemoteLotURL"), false);


	memset(buff, sizeof buff, 0);
	GetPrivateProfileString(_T("Global"), _T("m_FinsIP"), _T("192.168.100.100"), buff, 1024, g_sConfigPath);
	m_sFinsIP = buff;
	
}

void CFactoryConfig::SaveConfig(void)
{
	CString s;
	s.Format(_T("%d"), m_nWorkMode);
	WritePrivateProfileString(_T("Global"), _T("WorkMode"), s, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("AutoMark"), m_bAutoMark ? _T("1") : _T("0"), g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("AutoJob"), m_bAutoStartJob ? _T("1") : _T("0"), g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("RMS"), m_bOpenRMS ? _T("1") : _T("0"), g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("RollTip"), m_bOpenRollTip ? _T("1") : _T("0"), g_sConfigPath);
	s.Format(_T("%d"), m_nPort);
	WritePrivateProfileString(_T("Global"), _T("Port"), s, g_sConfigPath);	
	//s.Format(_T("%d"), m_nStartMarkSignalType);
	//WritePrivateProfileString(_T("Global"), _T("StartMarkSignel"), s, g_sConfigPath);
	//s.Format(_T("%d"), m_nFinishMarkSignalType);
	//WritePrivateProfileString(_T("Global"), _T("FinishMarkSignel"), s, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("MCCheckWarn"), m_bMCCheckWarn ? _T("1") : _T("0"), g_sConfigPath);
	s.Format(_T("%d"), m_nRetryTimes);
	WritePrivateProfileString(_T("Global"), _T("MCCheckRetryTimes"), s, g_sConfigPath);

	WritePrivateProfileString(_T("Path"), _T("MarkFilePath"), m_sMarkFilePath, g_sConfigPath);
	WritePrivateProfileString(_T("Path"), _T("LogFilePath"), m_sLogFilePath, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("FTPIP"), m_sFtpIP, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("WarnTip"), m_sWarnTips, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("MESIP"), m_sMesIP, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("MESURL"), m_sMesUrl, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("MobFilePath"), m_sMobFilePath, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("DeviceNo"), m_sDeviceNo, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("BarcodeHead"), m_sBarcodeHead, g_sConfigPath);
	WritePrivateProfileString(_T("Global"), _T("MCCheckTip"), m_sMCCheckTip, g_sConfigPath);

	WritePrivateProfileString(_T("Global"), _T("ManulStamp"), m_bManulStamp ? _T("1") : _T("0"), g_sConfigPath);
    WritePrivateProfileString(_T("Global"), _T("CheckOrderStatus"), m_bCheckOrderStatus ? _T("1") : _T("0"), g_sConfigPath);

	WritePrivateProfileString(_T("Global"), _T("MaxMarkTimesLimit"), m_bMaxMarkTimesLimit ? _T("1") : _T("0"), g_sConfigPath);
	s.Format(_T("%d"), m_nMaxMarkTimes);
	WritePrivateProfileString(_T("Global"), _T("MaxMarkTimes"), s, g_sConfigPath);
	WriteData(_T("Global"), _T("PenOrder"), m_bPenOrder);
	WriteData(_T("Global"), _T("Check2DBarcode"), m_bCheck2DBarcode);
	WriteData(_T("Global"), _T("CheckStripMap"), m_bCheckStripMap);
	WriteData(_T("Global"), _T("CheckVisualSystem"), m_bCheckVisualSystem);
	WriteData(_T("Global"), _T("VisionCheck"), m_bVisionCheck);
	WriteData(_T("Global"), _T("AddFilePrefix"), m_bAddFilePrefix);
	WriteData(_T("Global"), _T("CheckEQPStripIdIsMatchLotId"), m_bCheckEQPStripIdIsMatchLotId);
	WriteData(_T("Global"), _T("CheckProductType"), m_bCheckProductType);

	WriteData(_T("HT"), _T("LotIDXMLPath"), m_sLotIDXMLPath);
	WriteData(_T("HT"), _T("ScripIDXMLPath"), m_sScripIDXMLPath);
	WriteData(_T("HT"), _T("UseScan"), m_emUseScan);
	WriteData(_T("HT"), _T("ScannerIP_1"), m_sScannerIP[0]);
	WriteData(_T("HT"), _T("ScannerIP_2"), m_sScannerIP[1]);
	WriteData(_T("HT"), _T("ScannerPort_1"), m_nScannerPort[0]);
	WriteData(_T("HT"), _T("ScannerPort_2"), m_nScannerPort[1]);
	WriteData(_T("HT"), _T("ScannerStartSignal_1"), m_nScannStartSignal[0]);
	WriteData(_T("HT"), _T("ScannerStartSignal_2"), m_nScannStartSignal[1]);
	WriteData(_T("HT"), _T("ScannerStartSignal2_1"), m_nScannStartSignal2[0]);
	WriteData(_T("HT"), _T("ScannerStartSignal2_2"), m_nScannStartSignal2[1]);
	WriteData(_T("HT"), _T("m_nScannReadySingle"), m_nScannReadySingle);
	WriteData(_T("HT"), _T("m_nScanUse2DPort"), m_nScanUse2DPort);
	WriteData(_T("HT"), _T("ScanningOK"), m_nScannOKSignal);
	WriteData(_T("HT"), _T("ScanningNG"), m_nScannNGSignal);
	WriteData(_T("HT"), _T("ScanningWarn"), m_nScanWarnSignal);
	WriteData(_T("HT"), _T("ScanningBad"), m_nBadSignal);
	WriteData(_T("HT"), _T("ScanningSignal"), m_nScanningSignal);
	WriteData(_T("HT"), _T("ScanTime"), m_nScannTime);
	WriteData(_T("HT"), _T("ExitMarkSignal"), m_ExitMarkSignal);
	WriteData(_T("HT"), _T("CleanMarkFileSignal"), m_CleanMarkFileSignal);
	WriteData(_T("HT"), _T("StopScannerSignal"), m_StopScannerSignal);
	WriteData(_T("HT"), _T("ScanInputTimeOutPort"), m_nScanInputTimeOutPort);
	WriteData(_T("HT"), _T("ScanInputTimeOutPort"), m_nScanInputTimeOutPort);
	WriteData(_T("HT"), _T("ScanEnablePWS"), m_nScanEnablePWS);
	WriteData(_T("HT"), _T("ScanPWSOK"), m_nScanPWSOK);
	WriteData(_T("HT"), _T("ScanPWSNG"), m_nScanPWSNG);

	WriteData(_T("HT"), _T("StartPort"), m_nStartPort);
	WriteData(_T("HT"), _T("BusyPort"), m_nBusyPort);
	WriteData(_T("HT"), _T("ReadyPort"), m_nReadyPort);
	WriteData(_T("HT"), _T("EndPort"), m_nEndPort);
	WriteData(_T("HT"), _T("CommIP"), m_nCommIP);
	WriteData(_T("HT"), _T("CommPrrt"), m_nCommPort);

	WritePrivateProfileString(_T("Global"), _T("StampPath"), m_sStampPath, g_sConfigPath);
	WriteData(_T("Global"), _T("GFHPath"), m_sGFHPath);
	WriteData(_T("Global"), _T("LaserParamSave"), m_sLaserParamSave);

	WriteData(_T("Global"), _T("MesCheckUrl"), m_sMesCheckUrl);
	WriteData(_T("Global"), _T("MesTrackInUrl"), m_sMesTrackInUrl);
	WriteData(_T("Global"), _T("MesThickUrl"), m_sMesThickUrl);
	WriteData(_T("Global"), _T("MesProductType"), m_sMesProductType);
	WriteData(_T("Global"), _T("2DContentPath"), m_s2DPath);
	WriteData(_T("Global"), _T("Check2DBarcodeRepeat"), m_nCheck2DBarcodeRepeat);
	WriteData(_T("Global"), _T("SavePeriod"), m_nSavePeriod);

	WriteData(_T("Global"), _T("PortPLC"), m_nPortPLC);

	WriteData(_T("Global"), _T("bCheckVisionSwitchProcess"), m_bCheckVisionSwitchProcess);
	WriteData(_T("Global"), _T("bCheckPLCSwitchProcess"), m_bCheckPLCSwitchProcess);
	WriteData(_T("Global"), _T("bCheckFinsPLC"), m_bCheckFinsPLC);
	

	WriteData(_T("Global"), _T("m_iVisionCheckPort"), m_iVisionCheckPort);
	WriteData(_T("Global"), _T("m_iFinsPort"), m_iFinsPort);
//
	WriteData(_T("Global"), _T("m_FinishedLotIP"), m_FinishedLotIP);
	WriteData(_T("Global"), _T("m_EnableFinishedLot"), m_EnableFinishedLot);

	WriteData(_T("Global"), _T("m_RemotLotURL"), m_RemotLotURL);
	WriteData(_T("Global"), _T("m_EnableRemoteLotURL"), m_EnableRemoteLotURL);
	WritePrivateProfileString(_T("Global"), _T("m_FinsIP"), m_sFinsIP, g_sConfigPath);
}


void CFactoryConfig::WriteData(const CString & section, const CString & variable, const CString & s)
{
	WritePrivateProfileString(section, variable, s, g_sConfigPath);
}

void CFactoryConfig::WriteData(const CString & section, const CString & variable, int n)
{
	CString s;
	s.Format(_T("%d"), n);
	WritePrivateProfileString(section, variable, s, g_sConfigPath);
}

void CFactoryConfig::WriteData(const CString & section, const CString & variable, double d)
{
	CString s;
	s.Format(_T("%f"), d);
	WritePrivateProfileString(section, variable, s, g_sConfigPath);
}

void CFactoryConfig::WriteData(const CString & section, const CString & variable, bool b)
{
	CString s;
	s.Format(_T("%d"), (int)b);
	WritePrivateProfileString(section, variable, s, g_sConfigPath);
}

CString CFactoryConfig::GetData(const CString & section, const CString & variable, const CString & sD)
{
	TCHAR buff[1024];
	GetPrivateProfileString(section, variable, sD, buff, 1024, g_sConfigPath);
	return (CString)buff;
}

int CFactoryConfig::GetData(const CString & section, const CString & variable, int nD)
{
	return GetPrivateProfileInt(section, variable, nD, g_sConfigPath);
}

double CFactoryConfig::GetData(const CString & section, const CString & variable, double dD)
{
	TCHAR buff[1024];
	CString s;
	s.Format(_T("%f"), dD);
	GetPrivateProfileString(section, variable, s, buff, 1024, g_sConfigPath);
	return _ttof(buff);
}

bool CFactoryConfig::GetData(const CString & section, const CString & variable, bool bD)
{
	return (bool)GetPrivateProfileInt(section, variable, (int)bD, g_sConfigPath);
}


