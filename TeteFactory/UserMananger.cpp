#include "stdafx.h"
#include "UserMananger.h"
#include "EncryptPwd.h" 

CUserMananger* CUserMananger::_instance = nullptr;
CUserMananger::CUserMananger()
{
	m_sUserLoginFileName = _T("config\\UserLogin.ini");
	m_sUserPwdFileName = _T("config\\password.ini");
	m_sUserRightFileName = _T("config\\UserRight.ini");
	m_sUserPwdSect[USER_A_ADMIN] = _T("admin");
	m_sUserPwdSect[USER_A_ENGINEER] = _T("engineer");
	m_sUserPwdSect[USER_A_TECH] = _T("tech");
	m_sUserPwdSect[USER_A_OPER] = _T("factory");

	CTime currentTime = CTime::GetCurrentTime();

	m_emUserCur = (emUserAuthority_t)IniFileRead(_T("Global"), _T("Type"), USER_A_ADMIN, m_sUserLoginFileName);
	CString sDefaultPwd[MAX_USER_TYPE_COUNT] = { EncryptPWD(currentTime.Format(_T("TETE%Y%m%d"))),  EncryptPWD(_T("admin")), EncryptPWD(_T("TF")), EncryptPWD(_T("")) };

	CString sDefaultOperatorType[MAX_USER_TYPE_COUNT] = { combinUserOperatorType(g_emDefaultAdminOperatorType, dim(g_emDefaultAdminOperatorType))
		,combinUserOperatorType(g_emDefaultEngineerOperatorType, dim(g_emDefaultEngineerOperatorType))
		,combinUserOperatorType(g_emDefaultTechOperatorType, dim(g_emDefaultTechOperatorType))
		,combinUserOperatorType(g_emDefaultGuestOperatorType, dim(g_emDefaultGuestOperatorType)) };


	for (int i = 0; i < MAX_USER_TYPE_COUNT; i++)
	{
		CString s, sect;
		s = IniFileRead(_T("Global"), m_sUserPwdSect[i], sDefaultPwd[i], m_sUserPwdFileName);
		m_sUsePwd[i] = DecryptPWD(s);

		sect.Format(_T("UserRight_%d"), i);
		s = IniFileRead(_T("Global"), sect, sDefaultOperatorType[i], m_sUserRightFileName);
		parseUserOperatorType(s, m_vUserAuthority[i]);
	}
}

CUserMananger::~CUserMananger()
{

}

CUserMananger* CUserMananger::Instance()
{
	if (NULL == _instance)
	{
		_instance = new CUserMananger();
	}
	return _instance;
}

emUserAuthority_t CUserMananger::GetCurUser() const
{
	return m_emUserCur;
}

CString CUserMananger::getusername(const emUserAuthority_t& user) const
{
	switch (user)
	{
	case USER_A_ADMIN:
		return _T("管理员");
		break;
	case USER_A_ENGINEER:
		return _T("工程师");
		break;
	case USER_A_TECH:
		return _T("技术员");
		break;
	case USER_A_OPER:
		return _T("操作员");
		break;
	}
	return _T("操作员");
}

CString CUserMananger::GetCurUserName() const
{
	return getusername(m_emUserCur);
}

CString CUserMananger::GetUserName(const emUserAuthority_t& user) const
{
	return getusername(user);
}

void CUserMananger::ChangeUser(const emUserAuthority_t& user)
{
	m_emUserCur = user;
	IniFileWrite(_T("Global"), _T("Type"), user, m_sUserLoginFileName);
}

void CUserMananger::GetAllUser(std::vector<CString>& vsUserName)
{
	vsUserName.push_back(_T("管理员"));
	vsUserName.push_back(_T("工程师"));
	vsUserName.push_back(_T("技术员"));
	vsUserName.push_back(_T("操作员"));
}

bool CUserMananger::VerifyPwd(const emUserAuthority_t& user, const CString& spwd)
{
	if (user < 0 || user >= MAX_USER_TYPE_COUNT)
		return false;

	CString spwdlower = spwd;
	spwdlower = spwdlower.MakeLower();
	switch (user)
	{
	case USER_A_ADMIN:
		return m_sUsePwd[user].MakeLower() == spwdlower;
	case USER_A_ENGINEER:
		return (m_sUsePwd[user].MakeLower() == spwdlower || m_sUsePwd[USER_A_ADMIN].MakeLower() == spwdlower);
	case USER_A_TECH:
		return (m_sUsePwd[user].MakeLower() == spwdlower 
			|| m_sUsePwd[USER_A_ADMIN].MakeLower() == spwdlower 
			|| m_sUsePwd[USER_A_ENGINEER].MakeLower() == spwdlower);

	case USER_A_OPER:
		return (m_sUsePwd[user].MakeLower() == spwdlower
			|| m_sUsePwd[USER_A_ADMIN].MakeLower() == spwdlower
			|| m_sUsePwd[USER_A_ENGINEER].MakeLower() == spwdlower
			|| m_sUsePwd[USER_A_TECH].MakeLower() == spwdlower);
	}
	
	return false;
}

CString CUserMananger::combinUserOperatorType(const std::set<emUserOperatorType_t>&emOperatorType)
{
	CString s;
	for (auto& item : emOperatorType)
	{
		s += chInt2Str(item).c_str();
		s += _T(",");
	}
	return s;
}

CString CUserMananger::combinUserOperatorType(const emUserOperatorType_t DefaultOperatorType[], int iCount)
{
	CString s;
	for (int i = 0; i < iCount; i++)
	{
		s += chInt2Str(DefaultOperatorType[i]).c_str();
		s += _T(",");
	}
	return s;
}

bool CUserMananger::parseUserOperatorType(const CString& s, std::set<emUserOperatorType_t>&emOperatorType)
{
	std::vector<TString> vsData;
	if (!chParseString(s.GetString(), vsData, _T(',')))
		return false;

	for (auto& item : vsData)
	{
		emOperatorType.insert((emUserOperatorType_t)_ttoi(item.c_str()));
	}

	return true;
}

void CUserMananger::GetUserRights(const emUserAuthority_t& user, std::set<emUserOperatorType_t>& vUserAuthority)
{
	if (user < 0 || user >= MAX_USER_TYPE_COUNT)
		return ;
	vUserAuthority = m_vUserAuthority[user];
}

void CUserMananger::AddRight(const emUserAuthority_t& user, const emUserOperatorType_t& UserAuthority)
{
	if (user < 0 || user >= MAX_USER_TYPE_COUNT)
		return;

	m_vUserAuthority[user].insert(UserAuthority);

	CString sect;
	sect.Format(_T("UserRight_%d"), user);
	IniFileWrite(_T("Global"), sect, combinUserOperatorType(m_vUserAuthority[user]), m_sUserRightFileName);

}

void CUserMananger::RemoveRight(const emUserAuthority_t& user, const emUserOperatorType_t& UserAuthority)
{
	if (user < 0 || user >= MAX_USER_TYPE_COUNT)
		return;

	m_vUserAuthority[user].erase(UserAuthority);

	CString sect;
	sect.Format(_T("UserRight_%d"), user);
	IniFileWrite(_T("Global"), sect, combinUserOperatorType(m_vUserAuthority[user]), m_sUserRightFileName);
}

CString CUserMananger::GetUserPwd(const emUserAuthority_t& user) const
{
	if (user < 0 || user >= MAX_USER_TYPE_COUNT)
		return _T("");

	return m_sUsePwd[user];
}

void CUserMananger::ChangeUserPwd(const emUserAuthority_t& user, const CString& pwd)
{
	if (user < 0 || user >= MAX_USER_TYPE_COUNT)
		return;

	m_sUsePwd[user] = pwd;

	IniFileWrite(_T("Global"), m_sUserPwdSect[user], EncryptPWD(pwd), m_sUserPwdFileName);
}

bool CUserMananger::VerifyRight(const emUserOperatorType_t& UserAuthority)
{
	return (m_vUserAuthority[m_emUserCur].end() != m_vUserAuthority[m_emUserCur].find(UserAuthority));
}