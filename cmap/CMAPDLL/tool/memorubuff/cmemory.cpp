//#include "StdAfx.h"  
#include "cmemorystream.h"  

CMemoryStream::CMemoryStream(void)  
{  
	m_pBuffer = new CMemoryBuffer();  
	m_iOffset = 0;  
}  

CMemoryStream::CMemoryStream(CMemoryBuffer *tpbuffer)  
{  
	m_pBuffer = tpbuffer;  
	m_iOffset = 0;  
}  

CMemoryStream * CMemoryStream::GetInstance()  
{  
	CMemoryStream *pStream = new CMemoryStream(new CMemoryBuffer);  
	return pStream;  
}  



CMemoryStream::~CMemoryStream(void)  
{  
	if(m_pBuffer != NULL)  
		delete m_pBuffer;  
	m_pBuffer = NULL;  
}  

void CMemoryStream::Write(LPCWSTR szStr)  
{  
	if (szStr == NULL )  
	{  
		return;  
	}  
	int iLength = (int)wcslen(szStr) * sizeof(wchar_t);  
	Write(iLength);  
	if (iLength != 0)  
	{  
		Write((const BYTE*)szStr,iLength);  
	}  
}  

void CMemoryStream::Write(LPCSTR szStr)  
{  
	if (szStr == NULL )  
	{  
		return;  
	}  
	int iLength = (int)strlen(szStr) * sizeof(char);  
	Write(iLength);  
	if (iLength != 0)  
	{  
		Write((const BYTE*)szStr,iLength);  
	}  
}  


void CMemoryStream::Write(const string &szStr)  
{  
	Write(szStr.c_str());  
}  

void CMemoryStream::Write(const vector<string> &szStrVct)  
{  
	int iCnt = (int)szStrVct.size();  
	Write(iCnt);  
	for (int i = 0; i < (int)szStrVct.size(); i ++)  
	{  
		Write(szStrVct.at(i));  
	}  
}  


void CMemoryStream::Write(const wstring &szStr)  
{  
	Write(szStr.c_str());  
}  

void CMemoryStream::Write(const vector<wstring> &szStrVct)  
{  
	int iCnt = (int)szStrVct.size();  
	Write(iCnt);  
	for (int i = 0; i < (int)szStrVct.size(); i ++)  
	{  
		Write(szStrVct.at(i));  
	}  
}  




void CMemoryStream::Write(const CMemoryBuffer &field)  
{  
	Write(field.GetLength());  
	Write(field.c_Bytes(),field.GetLength());  
}  


void CMemoryStream::Write(const BYTE * tpBytes , const int &tiLength)  
{  
	m_pBuffer->Insert(m_iOffset,tpBytes,tiLength);  
	m_iOffset +=tiLength;  
}  

void CMemoryStream::Read( BYTE * tpBytes , const int tiLength)  
{  
	if(tpBytes == NULL) return;  
	if(m_iOffset + tiLength > m_pBuffer->GetLength()) return;  
	memcpy((tpBytes),m_pBuffer->c_Bytes() + m_iOffset,tiLength);  
	m_iOffset +=tiLength;  
}  

void CMemoryStream::Read(string &tszStr)  
{  
	int iBytesCnt = 0;  
	Read(iBytesCnt);  
	if (iBytesCnt >= 0)  
	{  
		Read(tszStr,iBytesCnt);  
	}  
}  

void CMemoryStream::Read(vector<string> &szStrVct)  
{  
	int iCnt = 0;  
	Read(iCnt);  
	if (iCnt >= 0)  
	{  
		for (int i = 0; i < iCnt; i ++)  
		{  
			string temp;  
			Read(temp);  
			szStrVct.push_back(temp);  
		}  
	}  
}  


void CMemoryStream::Read(string &tszStr,const int tiNumberofcharacter)  
{  
	char *pszTemp = new char[tiNumberofcharacter + 1];  
	memset(pszTemp,0,(tiNumberofcharacter + 1) * sizeof(char));  
	Read((BYTE*)pszTemp,(sizeof(char))*tiNumberofcharacter);  
	tszStr = pszTemp;  
	delete []pszTemp;  
}  

void CMemoryStream::Read(wstring &tszStr)  
{  
	int iBytesCnt = 0;  
	Read(iBytesCnt);  
	if (iBytesCnt >= 0)  
	{  
		Read(tszStr,iBytesCnt / sizeof(wchar_t) );  
	}  
}  

void CMemoryStream::Read(vector<wstring> &szStrVct)  
{  
	int iCnt = 0;  
	Read(iCnt);  
	if (iCnt >= 0)  
	{  
		for (int i = 0; i < iCnt; i ++)  
		{  
			wstring temp;  
			Read(temp);  
			szStrVct.push_back(temp);  
		}  
	}  
}  

void CMemoryStream::Read(wstring &tszStr,const int tiNumberofcharacter)  
{  
	wchar_t *pszTemp = new wchar_t[tiNumberofcharacter + 1];  
	memset(pszTemp,0,(tiNumberofcharacter + 1) * sizeof(wchar_t));  
	Read((BYTE*)pszTemp,(sizeof(wchar_t))*tiNumberofcharacter);  
	tszStr = pszTemp;  
	delete []pszTemp;  
}  

void CMemoryStream::Read( CMemoryBuffer &field)  
{  
	int iBytesCnt = 0;  
	Read(iBytesCnt);  
	BYTE *pBytes = new BYTE[iBytesCnt];  
	memset(pBytes,0,iBytesCnt);  
	Read(pBytes,iBytesCnt);  
	field.Append(pBytes,iBytesCnt);  
}  





void CMemoryStream::Seek(int tioffset, SeekOrigin loc)  
{  
	switch(loc)  
	{  
	case soBegin:  
		m_iOffset = tioffset;  
		break;  
	case soCurrent:  
		m_iOffset += tioffset;  
		break;  
	case soEnd:  
		m_iOffset = m_pBuffer->GetLength();  
		break;  
	}  
}  