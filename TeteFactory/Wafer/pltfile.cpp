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
#include "stdafx.h"  
#include <stdio.h>
#include <math.h>
#include "pltfile.h"
//---------------------------------------------------------------------------
CPltFile::CPltFile(void)
{
}
//---------------------------------------------------------------------------
CPltFile::~CPltFile(void)
{}
int CPltFile::LoadFile(const char *sFileName, CWaferGraph *pTheDoc)
{
    int iError = 0;
    if (NULL == pTheDoc)
		return -1;

	/*CString sExt = sFileName.Right(4);//--ExtractFileExt(sImageFileName);
    sExt.MakeLower();
    CStringA sFileNameA;
    sFileNameA = sFileName;
    if(0==sExt.Compare(L".plt"))*/
	if(true)
    {
        FILE *pFile;
        short sFlag;
        pFile=fopen(sFileName,"rb");
        fread(&sFlag,2,1,pFile);
        fseek(pFile,0,SEEK_SET);

        int nPenNum = 8;
        int nPenNo[8];
        if(sFlag==0x4E49)   // CorelPlt
        {        
            GetAllPenHPGL(pFile, &nPenNo[0], nPenNum);
            for (int i = 0; i < nPenNum; i++)
            {
                IGraphObject *pTheObject = pTheDoc->AppendObject(emObjType_SHAPE);
                iError = loadCorelDrawPlt(pFile,nPenNo[i],pTheObject);
                if (pTheObject->GetMetaCount()<=0)
					pTheDoc->DeleteObject(pTheObject->GetID());
            }
        }
        else if(sFlag==0x2E1B || sFlag == 0x282E)  // ACAD R14版本的Plt，采用HPGL格式
        {
            GetAllPenHPGL(pFile, &nPenNo[0], nPenNum);
            for (int i = 0; i < nPenNum; i++)
            {
				IGraphObject *pTheObject = pTheDoc->AppendObject(emObjType_SHAPE);
                iError = loadAutoCadPlt(pFile,nPenNo[i],pTheObject);
				if (pTheObject->GetMetaCount() <= 0)
					pTheDoc->DeleteObject(pTheObject->GetID());
            }
        }
        else
        {
            iError = -1;
        }
        fclose(pFile);
    }
    else
    {
        iError = -1;
    }
        
    return iError;
}
int CPltFile::loadCorelDrawPlt(FILE *pFile,int iPen, IGraphObject *pTheObject)
{
	CStringA sCommand;
	CStringA sValueX;
	CStringA sValueY;

	char sLine[256];
	std::vector<DPoint_t> thePoints;
	DPoint_t dp;
    fseek(pFile,0,SEEK_SET);
    bool bMatched = false;
    if (iPen == -1)
    {
        bMatched = true;
    }
	while(1)
    {
		if(NULL==fgets(sLine, 256, pFile))break;
		sCommand = sLine;
		if (sCommand.IsEmpty())break;

        if (-1 != sCommand.Find("SP", 0))
        {	    
            int nIndex = sCommand.GetAt(2)-'0';
            if (nIndex == iPen)
                bMatched = true;
            else
                bMatched = false;
            if (iPen < 0)
                bMatched = true;
        }
        if (!bMatched) continue;

		if (-1 != sCommand.Find("PD", 0))
		{			
			sValueX = sCommand.Mid(2, sCommand.Find(" "));
			sValueY = sCommand.Mid(sCommand.Find(" "), sCommand.Find(";") - sCommand.Find(" "));

			dp.x = atof(sValueX) *0.25*0.001;
			dp.y = atof(sValueY) *0.25*0.001;
			thePoints.push_back(dp);
		}
		else if (-1 != sCommand.Find("PU", 0))
		{
			if (thePoints.size()>1)
			{
				stPolyline_t stData;
				stData.ppt = &thePoints[0];
				stData.isize = thePoints.size();
				pTheObject->AppendMeta(emMETA_TAG_FRAME, stData);
			}
			thePoints.clear();

			sValueX = sCommand.Mid(2, sCommand.Find(" "));
			sValueY = sCommand.Mid(sCommand.Find(" "), sCommand.Find(";") - sCommand.Find(" "));

			dp.x = atof(sValueX) *0.25*0.001;
			dp.y = atof(sValueY) *0.25*0.001;
			thePoints.push_back(dp);
		}
	}
	if (thePoints.size()>1)
	{
		stPolyline_t stData;
		stData.ppt = &thePoints[0];
		stData.isize = thePoints.size();
		pTheObject->AppendMeta(emMETA_TAG_FRAME, stData);
	}
	thePoints.clear();

    return 0;
}
int CPltFile::loadAutoCadPlt(FILE *pFile,int iPen, IGraphObject *pTheObject)
{
    int iError=0;

    CStringA strKey[4];
    strKey[0] = "SP";
    strKey[1] = "PA";
    strKey[2] = "PU";
    strKey[3] = "PD";

    DPoint_t tpPoint;
    std::vector<DPoint_t> vPlotData;
    int nRet = 0;
    int nTemp = 0;
    bool bDown = false;
    char cT;
    tpPoint.x = 0;
    tpPoint.y = 0;
    int nPen = -1;
    fseek(pFile,0,SEEK_SET);
    while(true)
    {
        nRet = PltGotoNext(pFile, &strKey[0], 4);
        if (nRet < 0) break;
        if ((iPen >= 0)&&(nRet != 1)&&(iPen != nPen))
        {//1. 指定了层 2. 非换笔命令 3. 当前层非指定的层：不处理 
            continue;
        }

        switch (nRet)
        {
        case 1 : //换笔
            fread(&cT,1,1,pFile);
            if(feof(pFile)) break;
            if(cT==';') break;  
            nPen = cT-'0'; 
            if (iPen != nPen)
            {
                if (vPlotData.size() > 0)
                {
					stPolyline_t stData;
					stData.ppt = &vPlotData[0];
					stData.isize = vPlotData.size();
					pTheObject->AppendMeta(emMETA_TAG_FRAME, stData);
                    vPlotData.clear();
                }
            }
            break;
        case 2 : //移动坐标
            nTemp = GetTPoint(pFile, &tpPoint);
            if (nTemp < 0) continue;            
            if (bDown )  // 如果笔bDown，则保存到序列中
            {
                vPlotData.push_back(tpPoint);
            }
            break;
        case 3 : //抬笔
            bDown = false;
            if (vPlotData.size() > 0)
            {
				stPolyline_t stData;
				stData.ppt = &vPlotData[0];
				stData.isize = vPlotData.size();
				pTheObject->AppendMeta(emMETA_TAG_FRAME, stData);
                vPlotData.clear();
            }
            break;
        case 4 : //落笔
            bDown = true;   
            vPlotData.clear();
            vPlotData.push_back(tpPoint);
            break;
        default : break;
        }
    }

    return iError;
}
/*int CPltFile::GetAllPenCPlt(FILE* slFile, int* pnLayer, int& nLayer)
{
    TStringList *slFile=new TStringList();
    String strCommand;
    String strX;
    String strY;
    double dX,dY,*ddxy,*data;
    int strL,strS,pCount;
    slFile->LoadFromFile(szFileName);
    slFile->Add("PU000 000");
    int nStrCount=slFile->Count;

    int nMaxLayer = nLayer;
    int nLayerIndex = 0;
    for(int i=0;i< nStrCount;i++)
    {
        strCommand=slFile->Strings[i];
        if (strCommand.AnsiPos("SP") > 0)
        {
            strL = strCommand.Length();
            strS = strCommand.AnsiPos(";");
            strX = strCommand.SubString(3, strS - 3);
            int nIndex = strX.ToInt();
            int i;
            int nPos = nLayerIndex;
            bool bFound = false;

            for (i=0;i<nLayerIndex;i++)
            {
                if (pnLayer[i] == nIndex)
                {
                    bFound = true;
                    break;
                }
                if (pnLayer[i] > nIndex)
                {
                    bFound = false;
                    nPos = i;
                    break;
                }
            }

            if ((!bFound)&&(nLayerIndex < nMaxLayer))
            {
                for (int i = nLayerIndex;i>nPos;i--)
                {
                    pnLayer[i] = pnLayer[i-1];
                }
                pnLayer[nPos] = nIndex;
                nLayerIndex ++;
            }
            continue;
        }
    }

    slFile->Clear();
    delete slFile;
    nLayer = nLayerIndex;
    return nLayerIndex;
}*/
int CPltFile::GetAllPenHPGL(FILE* fp, int* pnLayer, int& nLayer)
{
    CStringA strKey[1];
    strKey[0] = "SP";

    int nRet = 0;
    char cT;
    int nMax = nLayer;

    nLayer = 0;
    while(true)
    {
        nRet = PltGotoNext(fp, &strKey[0], 1);
        if (nRet <= 0) break;

        int nPen;
        bool bFound;
        int nIndex;
        switch (nRet)
        {
        case 1 : //换笔
            fread(&cT,1,1,fp);
            if(feof(fp)) break;
            if(cT==';') break;

            nPen = cT-'0';
            bFound = false;
            nIndex = nLayer;
            for (int i=0;i<nLayer;i++)
            {
                if (pnLayer[i] == nPen)
                {
                    bFound = true;
                    break;
                }
                if (pnLayer[i] > nPen)
                {
                    bFound = false;
                    nIndex = i;
                    break;
                }
            }
            if (!bFound)
            {
                for (int i=nLayer;i>nIndex;i--)
                {
                    pnLayer[i] = pnLayer[i-1];
                }
                pnLayer[nIndex] = nPen;
                nLayer++;
            }
            break;
        default : break;
        }
        if (nLayer >= nMax) break;
    }

    return nLayer;
}
int CPltFile::PltGotoNext(FILE* fp, CStringA* pStr, int nNum)
{
    if (nNum == 0)
    {
        return -1;
    }
    char cT;
    int nIndex[20] = {0};
    int nRet = -1;
    bool bFound = false;
    while (true)
    {
        fread(&cT,1,1,fp);
        if(feof(fp))
            return -1;

        for (int i=0;i<nNum;i++)
        {
            if(cT==(pStr[i].GetAt(nIndex[i])))
            {
                nIndex[i] ++;
                if (nIndex[i] >= pStr[i].GetLength())
                {
                    bFound = true;
                    nRet = i + 1;
                }
            }
            else
            {
                nIndex[i] = 0;
            }
        }
        if (bFound) break;
    }

    return nRet;
}
int CPltFile::GetTPoint(FILE* fp, DPoint_t* pPoint)
{
    char cT;
    CStringA sTT="";

    while(true)
    {
        fread(&cT,1,1,fp);
        if(feof(fp)) return -1;
        if(cT==',') break;
        sTT+=cT;
    }

    try
    {
        pPoint->x = atoi(sTT);
    }
    catch(...)
    {
        return -1;
    }
    sTT="";
    while(true)
    {
        fread(&cT,1,1,fp);
        if(feof(fp)) return -1;
        if(cT==';')break;
        sTT+=cT;
    }
    pPoint->y= atoi(sTT);
    pPoint->x /= 1000;
    pPoint->y /= 1000;
    /*pPoint->x *= 0.1016;
    pPoint->y *= 0.1016;*/
    return 0;
}




