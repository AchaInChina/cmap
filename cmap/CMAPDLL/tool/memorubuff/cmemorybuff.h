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

	//得到内部指针——const  
	const BYTE * c_Bytes() const;  
	//从内部拷贝数据到数据中  
	BYTE * CopyOut(int &tiLength) const;  
	//确保tppBytes指向的数据足够大  
	void CopyTo(const BYTE ** tppBytes, int &tiLenth) const;  
	//从外部数据拷贝数据  
	void CopyFrom(const BYTE * tpBytes , int tiLength);  
	//从外部数据拷贝数据,添加  
	void Append(const BYTE * tpBytes , int tiLength);  
	void Append(const BYTE & tByte);  
	//从外部数据拷贝数据,插入  
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