#pragma once
#include "Dask.h"
#pragma comment(lib, "PCI-Dask.lib")

class C7230IOcomm
{
public:
	C7230IOcomm();
	~C7230IOcomm();

private:
	int m_nCardID;  //7230¿¨µÄ¿¨ºÅ
	U32 m_dwInData;
	U32 m_dwOutData0;
	U32 m_dwOutData1;

public:
	bool InitCard();
	bool GetIOStatus(int port, unsigned int &status);  //»ñÈ¡IO×´Ì¬
	bool SetIOStatus(int port, unsigned int signal);   //ÉèÖÃIO×´Ì¬
};

