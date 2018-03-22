//#include "StdAfx.h"  
#include "cmemorybuff.h"  



CMemoryBuffer::CMemoryBuffer(void)  
{  
}  

CMemoryBuffer::~CMemoryBuffer(void)  
{  
	this->Clear();  
}  

CMemoryBuffer::CMemoryBuffer(const CMemoryBuffer &other)  
{  
	*this = other;  
}  

CMemoryBuffer::CMemoryBuffer(const BYTE *tpBytes ,int tiLength)  
{  
	this->CopyFrom(tpBytes,tiLength);  
}  

void CMemoryBuffer::Clear()  
{  
	vector<BYTE>().swap(this->m_vctBuffer);   
}  


const BYTE * CMemoryBuffer::c_Bytes() const  
{  
	if(this->IsEmpty()) return NULL;  
	return &m_vctBuffer[0];  
}  

BYTE * CMemoryBuffer::CopyOut(int &tiLength) const  
{  
	tiLength = this->GetLength();  
	if(this->IsEmpty()) return NULL;  
	BYTE *pBytes = new BYTE[tiLength];  
	memcpy(pBytes,&m_vctBuffer[0],tiLength);  
	return pBytes;  
}  

void CMemoryBuffer::CopyFrom(const BYTE * tpBytes , int tiLength)  
{  
	this->Clear();  
	if(tpBytes == NULL || tiLength == 0) return;  
	m_vctBuffer.resize(tiLength,0);  
	memcpy(&m_vctBuffer[0],tpBytes,tiLength);  

}  

void CMemoryBuffer::Append(const BYTE * tpBytes , int tiLength)  
{  
	if(tpBytes == NULL || tiLength == 0) return;  
	m_vctBuffer.resize(this->GetLength() + tiLength,0);  
	memcpy(&m_vctBuffer[0] + this->GetLength() - tiLength,tpBytes,tiLength);  
}  

void CMemoryBuffer::Append(const BYTE & tByte)  
{  
	m_vctBuffer.push_back(tByte);  

}  

void CMemoryBuffer::Insert(int tiStartIndex,const BYTE * tpBytes , int tiLength)  
{  
	if(tpBytes == NULL || tiLength == 0) return;  
	int iBufferSize = this->GetLength();  
	if(tiStartIndex > iBufferSize) return;  
	if(tiStartIndex == iBufferSize)  
	{  
		this->Append(tpBytes,tiLength);  
	}  
	else if((tiStartIndex + tiLength) < iBufferSize)  
	{  
		memcpy(&m_vctBuffer[0] + tiStartIndex,tpBytes,tiLength);  
	}  
	else  
	{  
		m_vctBuffer.resize(tiStartIndex + tiLength ,0);  
		memcpy(&m_vctBuffer[0] + tiStartIndex,tpBytes,tiLength);  
	}  

}  

void CMemoryBuffer::CopyTo(const BYTE ** tppBytes, int &tiLength)const  
{  
	if(tppBytes == NULL || *tppBytes == NULL || this->IsEmpty()) return;  
	tiLength = this->GetLength();  
	memcpy(tppBytes,&m_vctBuffer[0],tiLength);  

}  



CMemoryBuffer & CMemoryBuffer::operator = (const CMemoryBuffer &other)  
{  
	this->Clear();  
	if (!other.IsEmpty())  
	{  
		m_vctBuffer.insert(m_vctBuffer.begin(),other.GetBuffer().begin(),other.GetBuffer().end());  
	}  
	return *this;  
}  

CMemoryBuffer & CMemoryBuffer::operator += (const CMemoryBuffer &other)  
{  
	if (!other.IsEmpty())  
	{  
		m_vctBuffer.insert(m_vctBuffer.end(),other.GetBuffer().begin(),other.GetBuffer().end());  
	}  
	return *this;  
}  