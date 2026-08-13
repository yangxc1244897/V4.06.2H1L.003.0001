#include "stdafx.h"
#include "EncryptPwd.h"



unsigned long _atoiEX(CString hexstr, int length)
{
	TCHAR  revstr[10] = { 0 };  //根据十六进制字符串的长度，这里注意数组不要越界  
	unsigned long   num[10] = { 0 };
	unsigned long   count = 1;
	unsigned long result = 0;
	_tcscpy_s(revstr, hexstr);
	for (int i = length - 1; i >= 0; i--)
	{
		if ((revstr[i] >= '0') && (revstr[i] <= '9'))
			num[i] = revstr[i] - 48;//字符0的ASCII值为48  
		else if ((revstr[i] >= 'a') && (revstr[i] <= 'f'))
			num[i] = revstr[i] - 'a' + 10;
		else if ((revstr[i] >= 'A') && (revstr[i] <= 'F'))
			num[i] = revstr[i] - 'A' + 10;
		else
			num[i] = 0;
		result = result + num[i] * count;
		count = count * 16;//十六进制(如果是八进制就在这里乘以8)      
	}
	return result;

}

CString EncryptPWD(CString szData, CString strKey)
{
	if (szData.IsEmpty())
		return _T("");
	//szData为待加密文本
	//strKey为加密秘钥
	int en[4];
	for (int i = 0; i < 4; i++)
	{
		en[i] = 1 + (int)(15.0*rand() / (RAND_MAX + 1.0));;//生成随机密文(范围可自己调整0-15)
	}
	wchar_t  *pData = szData.GetBuffer(0);//取生成的16进制字符串指针
	int	 nData = szData.GetLength();//取生成的16进制字符串长度
	wchar_t  *pKey = strKey.GetBuffer(0);//秘钥指针
	int	 nKeyLen = strKey.GetLength();//取秘钥长度
	int tmpi = 0;
	CString tmps, enstr;
	for (int i = 0; i <nData; i++)
	{
		tmpi = *pData + pKey[i%nKeyLen] - i*en[i / 4];//按位加密运算
		tmpi = tmpi>65535 ? tmpi - 65535 : tmpi;//范围修正
		tmps.Format(_T("%x"), tmpi);
		//格式输出
		if (tmpi >= 4096)
		{
			enstr += tmps;
		}
		else if (tmpi >= 256)
		{
			enstr += _T("0") + tmps;
		}
		else if (tmpi >= 16)
		{
			enstr += _T("00") + tmps;
		}
		else
		{
			enstr += _T("000") + tmps;
		}
		pData++;
	}
	szData.ReleaseBuffer();
	strKey.ReleaseBuffer();
	CString ens;
	for (int i = 0; i <4; i++)
	{
		tmps.Format(_T("%x"), en[i]);//随机秘钥放在返回字串前占用1个字符
		ens += tmps;
	}
	return  ens + enstr;
}

CString DecryptPWD(CString szData, CString strKey)
{
	if (szData.IsEmpty())
		return _T("");
	//szData为待解密文本
	//strKey为解密秘钥（同加密秘钥）
	int de[4];
	for (int i = 0; i < 4; i++)
	{
		de[i] = _atoiEX(szData.Left(1), 1);//获得随机秘钥
		szData.Delete(0, 1);//剩下的密文
	}
	int dn = szData.GetLength() / 4;
	wchar_t  *pKey = strKey.GetBuffer(0);
	int	 nKeyLen = strKey.GetLength();
	wchar_t *strc = new wchar_t[dn + 1];
	wchar_t *enstr = strc;
	CString tmps;
	int tmpi = 0;
	for (int i = 0; i < dn; i++)
	{
		tmps = szData.Left(4);//取4个字符（16进制字符）
		szData.Delete(0, 4);//剩下的字符
		*strc = (wchar_t)_atoiEX(tmps, 4);
		tmpi = *strc - pKey[i%nKeyLen] + i*de[i / 4];//解密运算 
		tmpi = tmpi < 0 ? tmpi + 65535 : tmpi;//范围判断修正
		*strc = tmpi;
		strc++;
	}
	*strc = 0;
	strKey.ReleaseBuffer();
	tmps = enstr;
	delete[]enstr; enstr = NULL;
	return  tmps;
}