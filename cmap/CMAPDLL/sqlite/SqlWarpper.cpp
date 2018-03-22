#include "SqlWarpper.h"


SqlWarpper::SqlWarpper(void)
{
	pDb = NULL;
	updateMode = true;
	memset(tableName, 0, 100);
}


SqlWarpper::~SqlWarpper(void)
{
	if (pDb != NULL)
	{
		delete pDb;
	}
}

Database * SqlWarpper::OpenDateBase(char * filename)
{
	if (pDb != NULL)
	{
		delete pDb;
	}
	pDb = new Database(filename, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX);
	return pDb;
}

int SqlWarpper::OpenTable(char * tablename)
{
	strcpy(tableName, tablename);
	if (!pDb->tableExists(tableName))
	{
		string strcmd;
		strcmd += "CREATE TABLE ";
		strcmd += tableName;
		strcmd += "( id INTEGER PRIMARY KEY, LatLngZoom TEXT, PngValue BLOB)";
		try
		{
			pDb->exec(strcmd);
		}
		catch (std::exception& e)
		{
			std::cout << "SQLite exception: " << e.what() << std::endl;
			printf("create table faild\n");
			return -1;
		}
		printf("create table success\n");
	}
	return 0;
}

int SqlWarpper::InsertKeyValue(char *  Key, string Value)
{
	string strcmd;
	int nb;
	
	try
	{
		string queryifexist;
		queryifexist += "SELECT * FROM ";
		queryifexist += tableName;
		queryifexist += " WHERE LatLngZoom = ?";
		SQLite::Statement queryexist( *pDb, queryifexist);
		queryexist.bind(1, Key);
		lock.Lock();
		if (queryexist.executeStep())
		{
			lock.Unlock();
			if (updateMode)
			{
				string updatecmd;
				updatecmd += "UPDATE ";
				updatecmd += tableName; 
				updatecmd += " SET PngValue = ? WHERE LatLngZoom = ?";
				SQLite::Statement insertquery(*pDb, updatecmd);
				insertquery.bind(1, Value);
				insertquery.bind(2, Key);
				lock.Lock();
				nb = insertquery.exec();
				lock.Unlock();
				//printf("update PngValue %d\n", nb);
			}
			else
			{
				return 0;
			}
		}
		else
		{
			lock.Unlock();
			string insercmd;
			insercmd += "INSERT INTO ";
			insercmd += tableName; 
			insercmd +=	" VALUES (NULL, ?, ?)";
			SQLite::Statement insertquery(*pDb, insercmd);
			//insertquery.bind(1, maxid);
			insertquery.bind(1, Key);
			insertquery.bind(2, Value);
			lock.Lock();
			nb = insertquery.exec();
			lock.Unlock();
			//printf("insert PngValue %d\n", nb);
		}
	}
	catch (std::exception& e)
	{
		lock.Unlock();
		std::cout << "SQLite exception: " << e.what() << std::endl;
		printf("InsertKeyValue faild\n");
		return -1;
	}
	return 0;
}

string SqlWarpper::GetKeyValue(char *  Key)
{
	string empty;
	string strcmd;
	strcmd += "SELECT * FROM ";
	strcmd += tableName;
	strcmd += " where LatLngZoom = ?";
	try
	{
		SQLite::Statement query( *pDb, strcmd);
		query.bind(1, Key);
		lock.Lock();
		if (query.executeStep())
		{
			lock.Unlock();
			Column	result = query.getColumn(2);
			string	pngstring( (char *)result.getBlob(), result.getBytes());
			
			return pngstring;
		}
		lock.Unlock();
	}
	catch (std::exception& e)
	{
		lock.Unlock();
		std::cout << "SQLite exception: " << e.what() << std::endl;
		printf("GetKeyValue faild\n");
		return empty;
	}
	return empty;
}