/**
  ******************************************************************************
  * @文件     
  * @作者  
  * @版本 V1.00.00
  * @日期 2017-04-11
  * @简述   
  ******************************************************************************
  * @说明
  *
  *
  * 
  ******************************************************************************
  */
#include "WaferGraphDoc.h"
//---------------------------------------------------------------------------
class CPltFile
{
public:
	CPltFile(void);
	~CPltFile(void);

	int LoadFile(const char *sFileName, CWaferGraph *pTheDoc);
	int loadCorelDrawPlt(FILE *pFile, int iPen, IGraphObject *pTheObject);
	int loadAutoCadPlt(FILE *pFile, int iPen, IGraphObject *pTheObject);
	int GetAllPenHPGL(FILE* fp, int* pnLayer, int& nLayer);
	int PltGotoNext(FILE* fp, CStringA* pStr, int nNum);
	int GetTPoint(FILE* fp, DPoint_t* pPoint);
};




