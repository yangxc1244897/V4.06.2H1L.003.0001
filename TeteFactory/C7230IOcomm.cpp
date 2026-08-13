#include "stdafx.h"
#include "C7230IOcomm.h"


C7230IOcomm::C7230IOcomm()
{
	m_nCardID = -1;
	m_dwInData = 0x00000000;
	m_dwOutData0 = m_dwOutData1 = 0x00000000;
}


C7230IOcomm::~C7230IOcomm()
{
	if (m_nCardID >= 0) {
		Release_Card(m_nCardID);
	}
}


bool C7230IOcomm::InitCard()
{
	m_nCardID = Register_Card(PCI_7230, 0);
	return m_nCardID < 0 ? false : true;
}


bool C7230IOcomm::GetIOStatus(int port, unsigned int &status)  //»ñÈ¡IO×´Ì¬
{
	status = IO_2D_IN_INVALID;
	U32 dwPortData, dwChangeMask;
	int n = pow(2, port);

	DI_ReadPort(m_nCardID, 0, &dwPortData);
#if 1
	if ((dwPortData & n) == n)
	{
		status = IO_2D_IN_VALID;
	}
	else {
		status = IO_2D_IN_INVALID;
	}
#else 

	dwChangeMask = dwPortData ^ m_dwInData;

	if (dwChangeMask != 0x00) {
		m_dwInData = dwPortData;
		if ((dwChangeMask & n) == n) // if the target bit is changed 
		{
			if ((dwPortData & n) == n)
			{
				status = IO_2D_IN_VALID;
			}
			else {
				status = IO_2D_IN_INVALID;
			}
		}
	}
#endif
	return true;
}

bool C7230IOcomm::SetIOStatus(int port, unsigned int signal)  //ÉèÖÃIO×´Ì¬
{
	int n = pow(2, port);
	if (IO_2D_OUT_VALID == signal) {
		m_dwOutData0 |= n;
	} else{
		m_dwOutData0 &= (~n);
	}
	DO_WritePort(m_nCardID, 0, m_dwOutData0);
	return true;
}