#pragma once
#include "head.h"
#include "sqlite3.h"
#include "Database.h"
#include "MyMutex.h"
using namespace SQLite;

class SqlWarpper
{
public:
	SqlWarpper(void);
	~SqlWarpper(void);

	Database * pDb;
	char tableName[100];
	bool updateMode;
	CMyMutex lock;

	Database * OpenDateBase(char * filename);
	int OpenTable(char * tablename);
	int InsertKeyValue(char *  Key, string Value);
	string GetKeyValue(char *  Key);
};

