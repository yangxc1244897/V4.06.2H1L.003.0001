#pragma once
/*密码加解密*/
#define PWD_KEY _T("TeteFactoryMarkSystem")



CString EncryptPWD(CString szData, CString strKey = PWD_KEY);//加密
CString DecryptPWD(CString szData, CString strKey = PWD_KEY);//解密
