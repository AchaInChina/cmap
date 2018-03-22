#pragma once
#ifndef _DELEGATE_H_
#define _DELEGATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <vector>
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

/////////////////////////////////////////////////////////////////////////////////
/// \class FuncCache
/// \brief ��������Ĵ���//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType>
class FuncCache
{
	static const int SIZE = 128;
	typedef ReturnType (*func_caller)(FuncCache*);
	/// \class MemberFuncAssist
	/// \brief �����Ա�����Ĵ����ĸ�����//
	class FuncCacheAssist
	{
	public:
		/// \brief ���캯������ʼ����//
		FuncCacheAssist(FuncCache* pFunc)
		{
			m_Size = 0;
			m_pFunc = pFunc;
			// ��ȡ��ƫ�Ʊ����λ//
			m_pFunc->m_Cur = 0;
		}
		/// \brief ����������//
		~FuncCacheAssist(void)
		{
			// ������ǰѹ��Ĳ���//
			if (m_Size > 0)
				m_pFunc->Pop(m_Size);
		}
		/// \brief ѹ��ָ����С�����ݡ�//
		uint Push(const void* pData, uint size)
		{
			m_Size += size;
			return m_pFunc->Push(pData, size);
		}
		/// ѹ������Ĵ�С//
		int                m_Size;
		/// �����Ա�����Ĵ���//
		FuncCache*         m_pFunc;
	};
public:
	/// \brief ���캯������ʼ����//
	FuncCache(func_caller func)
	{
		m_Size = 0;
		m_Cur = 0;
		m_Func = func;
	}
	/// \brief ѹ��ָ����С�����ݡ�//
	uint Push(const void* pData, uint size)
	{
		size = (size <= SIZE - m_Size)? size : (SIZE - m_Size);
		memcpy(m_Buffer + m_Size, pData, size);
		m_Size += size;
		return size;
	}
	/// \brief ����ָ����С�����ݡ�//
	uint Pop(uint size)
	{
		size = (size < m_Size)? size : m_Size;
		m_Size -= size;
		return size;
	}
	/// \brief ��ȡָ����С�����ݣ�����ָ�롣//
	void* Read(uint size)
	{
		m_Cur += size;
		return (m_Buffer + m_Cur - size);
	}
	/// \brief ִ��һ�������ĺ�����//
	ReturnType Execute(const void* ptr,const void* pData)
	{
		// �ø����ṹ����//
		FuncCacheAssist assist(this);

		assist.Push(&ptr, sizeof(void*));

		// ѹ�����//
		assist.Push(&pData, sizeof(void*));
		// ִ�к���//
		return m_Func(this);
	}
protected:
	/// ���󣬺���������ָ��Ļ�����//
	uchar         m_Buffer[SIZE];
	/// ��������С
	uint          m_Size;
	/// ��������ȡ�õ�ƫ��//
	uint          m_Cur;
	/// ����������ָ��//
	func_caller   m_Func;
public:
	void * executeFuncPtr;
};
/////////////////////////////////////////////////////////////////////////////////
/// \class MFuncCall_1
/// \brief ���������ĳ�Ա����ִ����//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType, typename Caller, typename Func, typename ParamTypeObject, typename ParamType>
class MFuncCall_1
{
public:
	/// \brief ִ�����������ĳ�Ա������//
	static ReturnType MFuncCall(FuncCache<ReturnType>* pMFunc)
	{
		// ��ö���ָ��
		Caller* pCaller = *(Caller**)pMFunc->Read(sizeof(Caller*));
		// ��ó�Ա����ָ��//
		Func func = *(Func*)pMFunc->Read(sizeof(Func));

		ParamTypeObject* pPtr = *(ParamTypeObject**)pMFunc->Read(sizeof(ParamTypeObject*));

		// ��ò�����ָ��//
		ParamType* pData = *(ParamType**)pMFunc->Read(sizeof(ParamType*));

		// ִ�г�Ա����
		return (pCaller->*func)(* pPtr, *pData);
	}
};
/////////////////////////////////////////////////////////////////////////////////
/// \class L_SignalRoot
/// \brief ���ͼ���ϸ���¼�ί��������//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType>
class L_SignalRoot
{
protected:
	typedef map<void*, vector<FuncCache<ReturnType>>> func_map;
	/// �¼����Ͱ󶨵��¼�ί�������б�//
	func_map m_MemberFuncMap;

public:
	/// \brief ָ���¼�����ж��ָ��������¼�ί������//
	template <typename Caller, typename Func>
	void MFuncUnregister(Caller* pCaller, Func func)
	{
		void * buff = 0;
		memcpy(&buff, (const void*)&func, sizeof(Func));
		func_map& func_list = m_MemberFuncMap;
		func_map::iterator it = func_list.find(pCaller);
		if (it != func_list.end())
		{
			vector<FuncCache<ReturnType>>::iterator vectorit = it->second.begin();
			while(vectorit != it->second.end())
			{
				if (vectorit->executeFuncPtr == buff)
				{
					it->second.erase(vectorit);
					break;
				}
				vectorit ++;
			}
		}
	}
	/// \brief ��������¼�ί������//
	void MFuncClear(void)
	{
		m_MemberFuncMap.clear();
	}
	
	//�õ���ǰί�����б��С//
	int MFuncSiz()
	{
		return m_MemberFuncMap.size();
	}
};
/////////////////////////////////////////////////////////////////////////////////
/// \class L_Signal_1
/// \brief ���ͼ���ϸ������������¼�ί����//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType, typename ParamTypeObject, typename ParamType>
class L_Signal_1 : public L_SignalRoot<ReturnType>
{
public:
	/// \brief ָ���¼�����ע���Ӧ�������������¼�ί������//
	template <typename Caller, typename Func>
	void MFuncRegister(Caller* pCaller, Func func)
	{
		// ָ��ר�Ŵ���һ�������ĺ���ִ����//
		FuncCache<ReturnType> mfunc(MFuncCall_1<ReturnType, Caller, Func, ParamTypeObject, ParamType>::MFuncCall);
		// ѹ�����ͺ���//
		mfunc.Push(&pCaller, sizeof(Caller*));
		mfunc.Push(&func, sizeof(Func));
		memcpy(&mfunc.executeFuncPtr, (const void*)&func, sizeof(Func));

		// ��ӵ��¼�ί�����б�//
		func_map& func_list = m_MemberFuncMap;
		func_map::iterator it = func_list.find(pCaller);
		if (it != func_list.end())
		{
			vector<FuncCache<ReturnType>>::iterator vectorit = it->second.begin();
			while(vectorit != it->second.end())
			{
				if (vectorit->executeFuncPtr == mfunc.executeFuncPtr)
				{
					break;
				}
				vectorit ++;
			}
			if (vectorit == it->second.end())
			{
				it->second.push_back(mfunc);
			}
		}
		else
		{
			vector<FuncCache<ReturnType>> funcvector;
			funcvector.push_back(mfunc);
			m_MemberFuncMap.insert(make_pair(pCaller, funcvector));

		}
	}
	/// \brief ָ���¼������������Ӧ��һ���������¼�ί������//
	ReturnType MFuncCall(const ParamTypeObject& ptr,const ParamType& data)
	{ 
		// ��շ���ֵ
		ReturnType result;
		memset(&result, 0, sizeof(result));
		// ��������ί����������ע��ĺ���//
		func_map::iterator it = m_MemberFuncMap.begin();
		while (it != m_MemberFuncMap.end())
		{
			vector<FuncCache<ReturnType>>::iterator vectorit = it->second.begin();
			while(vectorit != it->second.end())
			{
				vectorit->Execute(&ptr, &data);
				vectorit++;
			}
			++it;
		}
		return result;
	}
};

template <typename ClassType, typename ParamType>
class CDelegate
{
public:
	L_Signal_1<bool, ClassType, ParamType> signal;
	template <typename Caller, typename Func>
	void DelegateAdd(Caller* pCaller, Func func);

	template <typename Caller, typename Func>
	void DelegateDel(Caller* pCaller, Func func);

	void DelegateDo(ClassType classvalue, ParamType paramvalue);

	int DelegateSize();
};


template <typename ClassType, typename ParamType>
template <typename Caller, typename Func>
void CDelegate<ClassType, ParamType>::DelegateAdd(Caller* pCaller, Func func)
{
	signal.MFuncRegister( pCaller, func);
}

template <typename ClassType, typename ParamType>
template <typename Caller, typename Func>
void CDelegate<ClassType, ParamType>::DelegateDel(Caller* pCaller, Func func)
{
	signal.MFuncUnregister( pCaller, func);
}

template <typename ClassType, typename ParamType>
void CDelegate<ClassType, ParamType>::DelegateDo(ClassType classvalue, ParamType paramvalue)
{
	signal.MFuncCall(classvalue, paramvalue);
}

template <typename ClassType, typename ParamType>
int CDelegate<ClassType, ParamType>::DelegateSize()
{
	return signal.MFuncSiz();
}

#endif 