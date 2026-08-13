#include "stdafx.h"
#include "SettingChangeDetected.h"
#include "Sources\MarkPlatform.h"
#include "IniFileHander.h"
#include "UserMananger.h"

CSettingChangeDetected* CSettingChangeDetected::_Instance = NULL;


CSettingChangeDetected* CSettingChangeDetected::Instance()
{
	if (NULL == _Instance)
	{
		_Instance = new CSettingChangeDetected;
	}
	return _Instance;
}

void CSettingChangeDetected::Delete()
{
	if (NULL != _Instance)
	{
		delete _Instance;
		_Instance = NULL;
	}
}


CSettingChangeDetected::CSettingChangeDetected()
{
	m_FactoryConfig = CFactoryConfig::Instance();
	CString sFilePath = m_FactoryConfig->m_sLogFilePath + _T("\\SystemSetRecord");
	CreateMultiPath(sFilePath);
	CTime tm = CTime::GetCurrentTime();
	CString sTimeMonth = tm.Format(_T("%Y-%m-%d"));
	CString sFileName;
	sFileName.Format(_T("%s\\%s.txt"), sFilePath, sTimeMonth);
	if (!m_File.Open(sFileName, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate/* | CFile::shareDenyWrite*/))
	{
		AfxMessageBox(sFileName + _T("日志记录文件打开失败"));
	}
	m_File.SeekToEnd();
}


CSettingChangeDetected::~CSettingChangeDetected()
{
	m_File.Close();
}


bool CSettingChangeDetected::CompareAndChangeBoolVar(const CString& sVariableName, bool bNow)
{
	CStringA sTip;
	if (_T("m_bOpenRMS") == sVariableName)
	{
		if (m_FactoryConfig->m_bOpenRMS == bNow) return false;
		sTip.Format("启用RMS:从[%s]修改为[%s]", m_FactoryConfig->m_bOpenRMS ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bOpenRMS = bNow;
	}
	else if (_T("m_bOpenRollTip") == sVariableName)
	{
		if (m_FactoryConfig->m_bOpenRollTip == bNow) return false;
		sTip.Format("启用滚动提示栏:从[%s]修改为[%s]", m_FactoryConfig->m_bOpenRollTip ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bOpenRollTip = bNow;
	}
	else if (_T("m_bMCCheckWarn") == sVariableName)
	{
		if (m_FactoryConfig->m_bMCCheckWarn == bNow) return false;
		sTip.Format("启用MC抽检:从[%s]修改为[%s]", m_FactoryConfig->m_bMCCheckWarn ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bMCCheckWarn = bNow;
	}
	else if (_T("m_bCheckOrderStatus") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckOrderStatus == bNow) return false;
		sTip.Format("启用产品状态检查:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckOrderStatus ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckOrderStatus = bNow;
	}
	else if (_T("m_bMaxMarkTimesLimit") == sVariableName)
	{
		if (m_FactoryConfig->m_bMaxMarkTimesLimit == bNow) return false;
		sTip.Format("启用条数限制功能:从[%s]修改为[%s]", m_FactoryConfig->m_bMaxMarkTimesLimit ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bMaxMarkTimesLimit = bNow;
	}
	else if (_T("m_bPenOrder") == sVariableName)
	{
		if (m_FactoryConfig->m_bPenOrder == bNow) return false;
		sTip.Format("启用按激光参数顺序:从[%s]修改为[%s]", m_FactoryConfig->m_bPenOrder ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bPenOrder = bNow;
	}
	else if (_T("m_bCheck2DBarcode") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheck2DBarcode == bNow) return false;
		sTip.Format("启用2D读码器:从[%s]修改为[%s]", m_FactoryConfig->m_bCheck2DBarcode ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheck2DBarcode = bNow;
	}
	else if (_T("m_bCheckStripMap") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckStripMap == bNow) return false;
		sTip.Format("启用MAPPING功能:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckStripMap ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckStripMap = bNow;
	}
	else if (_T("m_bAddFilePrefix") == sVariableName)
	{
		if (m_FactoryConfig->m_bAddFilePrefix == bNow) return false;
		sTip.Format("启用添加文件前缀BB-KK:从[%s]修改为[%s]", m_FactoryConfig->m_bAddFilePrefix ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bAddFilePrefix = bNow;
	}
	else if (_T("m_bCheckVisualSystem") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckVisualSystem == bNow) return false;
		sTip.Format("启用视觉定位系统:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckVisualSystem ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckVisualSystem = bNow;
	}
	else if (_T("m_bCheckEQPStripIdIsMatchLotId") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckEQPStripIdIsMatchLotId == bNow) return false;
		sTip.Format("启用基板条码绑定检查:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckEQPStripIdIsMatchLotId ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckEQPStripIdIsMatchLotId = bNow;
	}
	else if (_T("m_bCheckProductType") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckProductType == bNow) return false;
		sTip.Format("启用产品类型检查:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckProductType ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckProductType = bNow;
	}
	else if (_T("m_bCheckVisionSwitchProcess") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckVisionSwitchProcess == bNow) return false;
		sTip.Format("启用视觉切换制程:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckVisionSwitchProcess ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckVisionSwitchProcess = bNow;
	}
	else if (_T("m_bCheckPLCSwitchProcess") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckPLCSwitchProcess == bNow) return false;
		sTip.Format("启用PLC切换制程:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckPLCSwitchProcess ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckPLCSwitchProcess = bNow;
	}
	else if (_T("m_bCheckFinsPLC") == sVariableName)
	{
		if (m_FactoryConfig->m_bCheckFinsPLC == bNow) return false;
		sTip.Format("启用FinsPLC客户端:从[%s]修改为[%s]", m_FactoryConfig->m_bCheckFinsPLC ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bCheckFinsPLC = bNow;
	}
	else if (_T("m_bVisionCheck") == sVariableName)
	{
		if (m_FactoryConfig->m_bVisionCheck == bNow) return false;
		sTip.Format("发送Mapping信息:从[%s]修改为[%s]", m_FactoryConfig->m_bVisionCheck ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_bVisionCheck = bNow;
	}
	else if (_T("m_EnableFinishedLot") == sVariableName)
	{
		if (m_FactoryConfig->m_EnableFinishedLot == bNow) return false;
		sTip.Format("Mes 远程校验:从[%s]修改为[%s]", m_FactoryConfig->m_EnableFinishedLot ? "启用" : "不启用", bNow ? "启用" : "不启用");
		m_FactoryConfig->m_EnableFinishedLot = bNow;
	}
	else
	{

		return false;
	}

	if (!sTip.IsEmpty())
	{
		sTip = CStringA(USERMNG->GetCurUserName()) + "	" + sTip;
		CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, sTip.GetString());
		CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_DEBUG, sTip.GetString());

		recordCommonFile(sTip.GetString());
	}
	return true;
}

bool CSettingChangeDetected::CompareAndChangeIntVar(const CString& sVariableName, int iNow)
{
	CStringA sTip;
	if (_T("m_nWorkMode") == sVariableName)
	{
		if (m_FactoryConfig->m_nWorkMode == iNow) return false;
		sTip.Format("作业模式:从[%d]修改为[%d]", m_FactoryConfig->m_nWorkMode, iNow);
		m_FactoryConfig->m_nWorkMode = iNow;
	}
	else if (_T("m_nPort") == sVariableName)
	{
		if (m_FactoryConfig->m_nPort == iNow) return false;
		sTip.Format("视觉端口:从[%d]修改为[%d]", m_FactoryConfig->m_nPort, iNow);
		m_FactoryConfig->m_nPort = iNow;
	}
	else if (_T("m_nPortPLC") == sVariableName)
	{
		if (m_FactoryConfig->m_nPortPLC == iNow) return false;
		sTip.Format("PLC端口:从[%d]修改为[%d]", m_FactoryConfig->m_nPortPLC, iNow);
		m_FactoryConfig->m_nPortPLC = iNow;
	}
	else if (_T("m_nRetryTimes") == sVariableName)
	{
		if (m_FactoryConfig->m_nRetryTimes == iNow) return false;
		sTip.Format("MC抽检次数上限:从[%d]修改为[%d]", m_FactoryConfig->m_nRetryTimes, iNow);
		m_FactoryConfig->m_nRetryTimes = iNow;
	}
	else if (_T("m_nMaxMarkTimes") == sVariableName)
	{
		if (m_FactoryConfig->m_nMaxMarkTimes == iNow) return false;
		sTip.Format("最大打印条数:从[%d]修改为[%d]", m_FactoryConfig->m_nMaxMarkTimes, iNow);
		m_FactoryConfig->m_nMaxMarkTimes = iNow;
	}
	else if (_T("m_nCheck2DBarcodeRepeat") == sVariableName)
	{
		if (m_FactoryConfig->m_nCheck2DBarcodeRepeat == iNow) return false;
		sTip.Format("二维码对比模式:从[%d]修改为[%d]", m_FactoryConfig->m_nCheck2DBarcodeRepeat, iNow);
		m_FactoryConfig->m_nCheck2DBarcodeRepeat = iNow;
	}
	else if (_T("m_iVisionCheckPort") == sVariableName)
	{
		if (m_FactoryConfig->m_iVisionCheckPort == iNow) return false;
		sTip.Format("视觉检测端口:从[%d]修改为[%d]", m_FactoryConfig->m_iVisionCheckPort, iNow);
		m_FactoryConfig->m_iVisionCheckPort = iNow;
	}
	else if (_T("m_iFinsPort") == sVariableName)
	{
		if (m_FactoryConfig->m_iFinsPort == iNow) return false;
		sTip.Format("Fins客户端端口:从[%d]修改为[%d]", m_FactoryConfig->m_iFinsPort, iNow);
		m_FactoryConfig->m_iFinsPort = iNow;
	}

	
	else
	{
		return false;
	}

	if (!sTip.IsEmpty())
	{
		sTip = CStringA(USERMNG->GetCurUserName()) + "	" + sTip;
		CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, sTip.GetString());
		CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_DEBUG, sTip.GetString());
		recordCommonFile(sTip.GetString());
	}

	return true;
}

bool CSettingChangeDetected::CompareAndChangeDoubleVar(const CString& sVariableName, double dbNow)
{
	return true;
}

bool CSettingChangeDetected::CompareAndChangeStringVar(const CString& sVariableName, const CString& sNow)
{
	
	CStringA sTip;
	if (_T("m_sLotIDXMLPath") == sVariableName)
	{
		if (m_FactoryConfig->m_sLotIDXMLPath == sNow) return false;
		sTip.Format("LotID文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sLotIDXMLPath), CStringA(sNow));
		m_FactoryConfig->m_sLotIDXMLPath = sNow;
	}
	else if (_T("m_sScripIDXMLPath") == sVariableName)
	{
		if (m_FactoryConfig->m_sLotIDXMLPath == sNow) return false;
		sTip.Format("StripID文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sScripIDXMLPath), CStringA(sNow));
		m_FactoryConfig->m_sScripIDXMLPath = sNow;
	}
	else if (_T("m_sMarkFilePath") == sVariableName)
	{
		if (m_FactoryConfig->m_sMarkFilePath == sNow) return false;
		sTip.Format("阵列图档文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMarkFilePath), CStringA(sNow));
		m_FactoryConfig->m_sMarkFilePath = sNow;
	}
	else if (_T("m_sLogFilePath") == sVariableName)
	{
		if (m_FactoryConfig->m_sLogFilePath == sNow) return false;
		sTip.Format("日志文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sLogFilePath), CStringA(sNow));
		m_FactoryConfig->m_sLogFilePath = sNow;
	}
	else if (_T("m_sFtpIP") == sVariableName)
	{
		if (m_FactoryConfig->m_sFtpIP == sNow) return false;
		sTip.Format("FTP文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sFtpIP), CStringA(sNow));
		m_FactoryConfig->m_sFtpIP = sNow;
	}
	else if (_T("m_sWarnTips") == sVariableName)
	{
		if (m_FactoryConfig->m_sWarnTips == sNow) return false;
		sTip.Format("滚动提示语:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sWarnTips), CStringA(sNow));
		m_FactoryConfig->m_sWarnTips = sNow;
	}
	else if (_T("m_sMesIP") == sVariableName)
	{
		if (m_FactoryConfig->m_sMesIP == sNow) return false;
		sTip.Format("服务器IP:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMesIP), CStringA(sNow));
		m_FactoryConfig->m_sMesIP = sNow;
	}
	else if (_T("m_sMesUrl") == sVariableName)
	{
		if (m_FactoryConfig->m_sMesUrl == sNow) return false;
		sTip.Format("单颗URL:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMesUrl), CStringA(sNow));
		m_FactoryConfig->m_sMesUrl = sNow;
	}
	else if (_T("m_sMesCheckUrl") == sVariableName)
	{
		if (m_FactoryConfig->m_sMesCheckUrl == sNow) return false;
		sTip.Format("基板URL:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMesCheckUrl), CStringA(sNow));
		m_FactoryConfig->m_sMesCheckUrl = sNow;
	}
	else if (_T("m_sMesTrackInUrl") == sVariableName)
	{
		if (m_FactoryConfig->m_sMesTrackInUrl == sNow) return false;
		sTip.Format("过站URL:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMesTrackInUrl), CStringA(sNow));
		m_FactoryConfig->m_sMesTrackInUrl = sNow;
	}
	else if (_T("m_sMesThickUrl") == sVariableName)
	{
		if (m_FactoryConfig->m_sMesThickUrl == sNow) return false;
		sTip.Format("晶圆厚度URL:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMesThickUrl), CStringA(sNow));
		m_FactoryConfig->m_sMesThickUrl = sNow;
	}
	else if (_T("m_sMesUrlProducttype") == sVariableName)
	{
		if (m_FactoryConfig->m_sMesProductType == sNow) return false;
		sTip.Format("产品类型URL:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMesProductType), CStringA(sNow));
		m_FactoryConfig->m_sMesProductType = sNow;
	}
	else if (_T("m_sMobFilePath") == sVariableName)
	{
		if (m_FactoryConfig->m_sMobFilePath == sNow) return false;
		sTip.Format("阵列修正文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMobFilePath), CStringA(sNow));
		m_FactoryConfig->m_sMobFilePath = sNow;
	}
	else if (_T("m_sDeviceNo") == sVariableName)
	{
		if (m_FactoryConfig->m_sDeviceNo == sNow) return false;
		sTip.Format("设备名称:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sDeviceNo), CStringA(sNow));
		m_FactoryConfig->m_sDeviceNo = sNow;
	}
	else if (_T("m_sBarcodeHead") == sVariableName)
	{
		if (m_FactoryConfig->m_sBarcodeHead == sNow) return false;
		sTip.Format("图档头部:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sBarcodeHead), CStringA(sNow));
		m_FactoryConfig->m_sBarcodeHead = sNow;
	}
	else if (_T("m_sMCCheckTip") == sVariableName)
	{
		if (m_FactoryConfig->m_sMCCheckTip == sNow) return false;
		sTip.Format("MC抽检提示语:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sMCCheckTip), CStringA(sNow));
		m_FactoryConfig->m_sMCCheckTip = sNow;
	}
	else if (_T("m_s2DPath") == sVariableName)
	{
		if (m_FactoryConfig->m_s2DPath == sNow) return false;
		sTip.Format("Unit模式下文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_s2DPath), CStringA(sNow));
		m_FactoryConfig->m_s2DPath = sNow;
	}
	else if (_T("m_sStampPath") == sVariableName)
	{
		if (m_FactoryConfig->m_sStampPath == sNow) return false;
		sTip.Format("印章图档文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sStampPath), CStringA(sNow));
		m_FactoryConfig->m_sStampPath = sNow;
	}
	else if (_T("m_sGFHPath") == sVariableName)
	{
		if (m_FactoryConfig->m_sGFHPath == sNow) return false;
		sTip.Format("印章规范号文件路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sGFHPath), CStringA(sNow));
		m_FactoryConfig->m_sGFHPath = sNow;
	}
	else if(_T("m_sLaserParamSave") == sVariableName)
	{
		if (m_FactoryConfig->m_sLaserParamSave == sNow) return false;
		sTip.Format("激光参数保存路径:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sLaserParamSave), CStringA(sNow));
		m_FactoryConfig->m_sLaserParamSave = sNow;
	}
	else if (_T("CaveatTime") == sVariableName)
	{
		sTip.Format("激光器报警时间修改为[%s]", sNow);
	}
	else if (_T("m_sFinsIP") == sVariableName)
	{
		if (m_FactoryConfig->m_sFinsIP == sNow) return false;
		sTip.Format("Fins客户端ip:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_sFinsIP), CStringA(sNow));
		m_FactoryConfig->m_sFinsIP = sNow;
	}
	else if (_T("m_FinishedLotUrl") == sVariableName)
	{
		if (m_FactoryConfig->m_FinishedLotUrl == sNow) return false;
		sTip.Format("校验mes 网络端口:从[%s]修改为[%s]", CStringA(m_FactoryConfig->m_FinishedLotUrl),CStringA(sNow));
		m_FactoryConfig->m_FinishedLotUrl = sNow;

		}
	else
	{
		return false;
	}

	if (!sTip.IsEmpty())
	{
		sTip = CStringA(USERMNG->GetCurUserName()) + "	" + sTip;
		CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, sTip.GetString());
		CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_DEBUG, sTip.GetString());
		recordCommonFile(sTip.GetString());
		
	}
	return true;
}

void CSettingChangeDetected::RecordVarChange(const CString& sVariableName)
{
	CStringA sTip;
	if (_T("LaserDiodeTime") == sVariableName)
	{
		sTip.Format("激光器已使用时间被清零");
	}

	if (!sTip.IsEmpty())
	{
		sTip = CStringA(USERMNG->GetCurUserName()) + "	" + sTip;
		CMarkPlatform::Instance()->Message(emMSG_TYPE_INFO, sTip.GetString());
		CMarkPlatform::Instance()->PrintLog(emLOG_LEVEL_DEBUG, sTip.GetString());
		recordCommonFile(sTip.GetString());
	}
}

void CSettingChangeDetected::recordCommonFile(const CStringA& s)
{
	if (INVALID_HANDLE_VALUE != m_File.m_hFile)
	{
		char TimeBuffer[256];
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		CStringA sContent;
		sContent.Format("(%04u%02u%02u %u:%u:%u.%u)	%s\r\n",
			systime.wYear, systime.wMonth, systime.wDay,
			systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds
			, s);
			
		m_File.Write(sContent.GetString(), sContent.GetLength());
	}
}