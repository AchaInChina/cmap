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
/// \brief 函数对象寄存器//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType>
class FuncCache
{
	static const int SIZE = 128;
	typedef ReturnType (*func_caller)(FuncCache*);
	/// \class MemberFuncAssist
	/// \brief 对象成员函数寄存器的辅助器//
	class FuncCacheAssist
	{
	public:
		/// \brief 构造函数，初始化。//
		FuncCacheAssist(FuncCache* pFunc)
		{
			m_Size = 0;
			m_pFunc = pFunc;
			// 读取用偏移必须归位//
			m_pFunc->m_Cur = 0;
		}
		/// \brief 析构函数。//
		~FuncCacheAssist(void)
		{
			// 弹出以前压入的参数//
			if (m_Size > 0)
				m_pFunc->Pop(m_Size);
		}
		/// \brief 压入指定大小的数据。//
		uint Push(const void* pData, uint size)
		{
			m_Size += size;
			return m_pFunc->Push(pData, size);
		}
		/// 压入参数的大小//
		int                m_Size;
		/// 对象成员函数寄存器//
		FuncCache*         m_pFunc;
	};
public:
	/// \brief 构造函数，初始化。//
	FuncCache(func_caller func)
	{
		m_Size = 0;
		m_Cur = 0;
		m_Func = func;
	}
	/// \brief 压入指定大小的数据。//
	uint Push(const void* pData, uint size)
	{
		size = (size <= SIZE - m_Size)? size : (SIZE - m_Size);
		memcpy(m_Buffer + m_Size, pData, size);
		m_Size += size;
		return size;
	}
	/// \brief 弹出指定大小的数据。//
	uint Pop(uint size)
	{
		size = (size < m_Size)? size : m_Size;
		m_Size -= size;
		return size;
	}
	/// \brief 读取指定大小的数据，返回指针。//
	void* Read(uint size)
	{
		m_Cur += size;
		return (m_Buffer + m_Cur - size);
	}
	/// \brief 执行一个参数的函数。//
	ReturnType Execute(const void* ptr,const void* pData)
	{
		// 用辅助结构控制//
		FuncCacheAssist assist(this);

		assist.Push(&ptr, sizeof(void*));

		// 压入参数//
		assist.Push(&pData, sizeof(void*));
		// 执行函数//
		return m_Func(this);
	}
protected:
	/// 对象，函数，参数指针的缓冲区//
	uchar         m_Buffer[SIZE];
	/// 缓冲区大小
	uint          m_Size;
	/// 缓冲区读取用的偏移//
	uint          m_Cur;
	/// 操作函数的指针//
	func_caller   m_Func;
public:
	void * executeFuncPtr;
};
/////////////////////////////////////////////////////////////////////////////////
/// \class MFuncCall_1
/// \brief 两个参数的成员函数执行体//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType, typename Caller, typename Func, typename ParamTypeObject, typename ParamType>
class MFuncCall_1
{
public:
	/// \brief 执行两个参数的成员函数。//
	static ReturnType MFuncCall(FuncCache<ReturnType>* pMFunc)
	{
		// 获得对象指针
		Caller* pCaller = *(Caller**)pMFunc->Read(sizeof(Caller*));
		// 获得成员函数指针//
		Func func = *(Func*)pMFunc->Read(sizeof(Func));

		ParamTypeObject* pPtr = *(ParamTypeObject**)pMFunc->Read(sizeof(ParamTypeObject*));

		// 获得参数的指针//
		ParamType* pData = *(ParamType**)pMFunc->Read(sizeof(ParamType*));

		// 执行成员函数
		return (pCaller->*func)(* pPtr, *pData);
	}
};
/////////////////////////////////////////////////////////////////////////////////
/// \class L_SignalRoot
/// \brief 类型检查严格的事件委托器基类//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType>
class L_SignalRoot
{
protected:
	typedef map<void*, vector<FuncCache<ReturnType>>> func_map;
	/// 事件名和绑定的事件委托器的列表//
	func_map m_MemberFuncMap;

public:
	/// \brief 指定事件名，卸载指定对象的事件委托器。//
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
	/// \brief 清空所有事件委托器。//
	void MFuncClear(void)
	{
		m_MemberFuncMap.clear();
	}
	
	//得到当前委托器列表大小//
	int MFuncSiz()
	{
		return m_MemberFuncMap.size();
	}
};
/////////////////////////////////////////////////////////////////////////////////
/// \class L_Signal_1
/// \brief 类型检查严格，两个参数的事件委托器//
/////////////////////////////////////////////////////////////////////////////////
template <typename ReturnType, typename ParamTypeObject, typename ParamType>
class L_Signal_1 : public L_SignalRoot<ReturnType>
{
public:
	/// \brief 指定事件名，注册对应的两个参数的事件委托器。//
	template <typename Caller, typename Func>
	void MFuncRegister(Caller* pCaller, Func func)
	{
		// 指定专门处理一个参数的函数执行体//
		FuncCache<ReturnType> mfunc(MFuncCall_1<ReturnType, Caller, Func, ParamTypeObject, ParamType>::MFuncCall);
		// 压入对象和函数//
		mfunc.Push(&pCaller, sizeof(Caller*));
		mfunc.Push(&func, sizeof(Func));
		memcpy(&mfunc.executeFuncPtr, (const void*)&func, sizeof(Func));

		// 添加到事件委托器列表//
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
	/// \brief 指定事件名，调用其对应的一个参数的事件委托器。//
	ReturnType MFuncCall(const ParamTypeObject& ptr,const ParamType& data)
	{ 
		// 清空返回值
		ReturnType result;
		memset(&result, 0, sizeof(result));
		// 对于所有委托器，调用注册的函数//
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