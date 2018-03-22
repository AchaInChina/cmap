#include <vector>  
#include <queue>  
//#include <afxmt.h>  

using namespace std;  
typedef unsigned char       BYTE;


class  CMemoryBuffer  
{  
public:  
	CMemoryBuffer(void);  
	CMemoryBuffer(const CMemoryBuffer &other);  
	CMemoryBuffer(const BYTE *tpBytes ,int tiLength);  
	virtual ~CMemoryBuffer(void);  

	//�õ��ڲ�ָ�롪��const  
	const BYTE * c_Bytes() const;  
	//���ڲ��������ݵ�������  
	BYTE * CopyOut(int &tiLength) const;  
	//ȷ��tppBytesָ��������㹻��  
	void CopyTo(const BYTE ** tppBytes, int &tiLenth) const;  
	//���ⲿ���ݿ�������  
	void CopyFrom(const BYTE * tpBytes , int tiLength);  
	//���ⲿ���ݿ�������,���  
	void Append(const BYTE * tpBytes , int tiLength);  
	void Append(const BYTE & tByte);  
	//���ⲿ���ݿ�������,����  
	void Insert(int tiStartIndex,const BYTE * tpBytes , int tiLength);  


	CMemoryBuffer & operator = (const CMemoryBuffer &other);  

	CMemoryBuffer & operator += (const CMemoryBuffer &other);  

	const std::vector<BYTE> &GetBuffer() const { return m_vctBuffer; }  

	void Clear() ;  

	int GetLength() const { return (int)m_vctBuffer.size(); }  

	bool IsEmpty() const { return m_vctBuffer.size() == 0; }  

public:  
	vector<BYTE> m_vctBuffer;  
};  