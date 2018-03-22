	#pragma once  

	#include "cmemoryBuff.h"  

	#include <string>  
	#include <vector>  
	using namespace std;  

	#define BaseClassDll_API

	#define _Base_Struct_Write_Func(_Struct_Type)   \
	void Write(const _Struct_Type &field){	Write((const BYTE*)&field,sizeof(_Struct_Type));}  

	#define _Base_Struct_Read_Func(_Struct_Type)    \
	void Read( _Struct_Type &field){Read(((BYTE*)&field),sizeof(_Struct_Type));}  

	#define _Struct_Vector_Write_Func(_Struct_Type) \
	void Write(vector<_Struct_Type> &vcts) \
	{\
		Write((int)vcts.size()); 	\
		for (std::vector<_Struct_Type>::iterator iter = vcts.begin();iter != vcts.end(); ++iter)\
		{\
			Write(*iter);\
		}\
	}  

	#define _Struct_Vector_Read_Func(_Struct_Type)\
	void Read(vector<_Struct_Type> &vcts)\
	{\
		int iCnt = 0;Read(iCnt);\
		if(iCnt <= 0) return;\
		_Struct_Type _Item;\
		for (int i = 0;\
		i < iCnt; i ++)\
		{\
		Read(_Item);\
		vcts.push_back(_Item);\
		}\
	}  

	#define _Base_Struct_Read_Write_Func(_Struct_Type)  \
	_Base_Struct_Write_Func(_Struct_Type) \
	_Base_Struct_Read_Func(_Struct_Type) \
	_Struct_Vector_Write_Func(_Struct_Type) \
	_Struct_Vector_Read_Func(_Struct_Type)  

	class BaseClassDll_API CMemoryStream  
	{  
	//private:  
	//	CMemoryStream(void);  
	public:  
//Summary:  
//		Provides the fields that represent reference points in streams for seeking.  
			enum SeekOrigin  
		{  
//Summary:  
//			Specifies the beginning of a stream.  
				soBegin = 0,  

//Summary:  
//			Specifies the current position within a stream.  
				soCurrent = 1,  

//Summary:  
//			Specifies the end of a stream.  
				soEnd = 2,  
		};  
	public:  
		CMemoryStream(CMemoryBuffer *tpbuffer);  
		CMemoryStream();  
		virtual ~CMemoryStream(void);  

		static CMemoryStream * GetInstance(void);  

	public:  
		_Base_Struct_Read_Write_Func(int)  
		_Base_Struct_Read_Write_Func(unsigned int)  
		_Base_Struct_Read_Write_Func(short)  
		_Base_Struct_Read_Write_Func(unsigned short)  
		_Base_Struct_Read_Write_Func(__int64)  
		_Base_Struct_Read_Write_Func(unsigned __int64)  
		_Base_Struct_Read_Write_Func(char)  
		_Base_Struct_Read_Write_Func(unsigned char)  
		_Base_Struct_Read_Write_Func(float)  
		_Base_Struct_Read_Write_Func(double)  
		_Base_Struct_Read_Write_Func(SYSTEMTIME)  
		void Write(LPCWSTR szStr);  
		void Write(LPCSTR szStr);  
		void Write(const string &szStr);  
		void Write(const wstring &szStr);  
		void Write(const vector<string> &szStrVct);  
		void Write(const vector<wstring> &szStrVct);  

		void Read(string &tszStr);  
		void Read(wstring &tszStr);  
		void Read(string &tszStr,const int tiNumberofcharacter);  
		void Read(wstring &tszStr,const int tiNumberofcharacter);  
		void Read(vector<string> &szStrVct);  
		void Read(vector<wstring> &szStrVct);  
		void Write(const CMemoryBuffer &tbuffer);  
		void Read(CMemoryBuffer &tbuffer);  
		void Write(const BYTE * tpBytes , const int &tiLength);  
		void Read(BYTE * tpBytes , const int tiLength);  
	public:  
			void Seek(int tioffset, SeekOrigin loc);  
		int GetOffset()const{return this->m_iOffset;};  
		int GetLength()const{return this->m_pBuffer->GetLength();};  


	protected:  
		CMemoryBuffer *m_pBuffer;  
		int m_iOffset;  
	};