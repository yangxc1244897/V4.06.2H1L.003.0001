#pragma once
#include "UserMngDefine.h"
#include <vector>
#include <set>
#include <map>
#include "tlString.h"
#include "IniFileHander.h"
class CUserMananger
{
public:
	~CUserMananger();
	static CUserMananger* Instance();
	emUserAuthority_t GetCurUser() const;
	CString GetCurUserName() const;
	void ChangeUser(const emUserAuthority_t& user);
	void GetAllUser(std::vector<CString>& vsUserName);
	/**
	* @brief 密码验证。管理员密码可以通过所有用户，技术员密码可以通过技术员和操作员，操作员只能通过操作员
	*/
	bool VerifyPwd(const emUserAuthority_t& user, const CString& spwd);
	void GetUserRights(const emUserAuthority_t& user, std::set<emUserOperatorType_t>& vUserAuthority);
	void AddRight(const emUserAuthority_t& user, const emUserOperatorType_t& UserAuthority);
	void RemoveRight(const emUserAuthority_t& user, const emUserOperatorType_t& UserAuthority);
	CString GetUserName(const emUserAuthority_t& user) const;
	CString GetUserPwd(const emUserAuthority_t& user) const;
	void ChangeUserPwd(const emUserAuthority_t& user, const CString& pwd);
	bool VerifyRight(const emUserOperatorType_t& UserAuthority);

private:
	CUserMananger();
	CString combinUserOperatorType(const std::set<emUserOperatorType_t>&emOperatorType);
	CString combinUserOperatorType(const emUserOperatorType_t DefaultOperatorType[], int iCount);
	bool parseUserOperatorType(const CString& s, std::set<emUserOperatorType_t>&emOperatorType);
	CString getusername(const emUserAuthority_t& user) const;
private:
	std::set<emUserOperatorType_t> m_vUserAuthority[MAX_USER_TYPE_COUNT];
	CString m_sUsePwd[MAX_USER_TYPE_COUNT];
	static CUserMananger* _instance;
	emUserAuthority_t m_emUserCur;  // 当前用户
	CString m_sUserLoginFileName;
	CString m_sUserRightFileName;
	CString m_sUserPwdFileName;
	CString m_sUserPwdSect[MAX_USER_TYPE_COUNT];
};

#define USERMNG CUserMananger::Instance()
#define USER_VERIFY(r) USERMNG->VerifyRight(r)

// 带返回的验证
#define USER_VERIFY_RET(r) 	if (!USER_VERIFY(r)) {AfxMessageBox(_T("权限不够！"));	return;}
#define USER_VERIFY_RETV(r, v) 	if (!USER_VERIFY(r)) {AfxMessageBox(_T("权限不够！"));	return v;}

