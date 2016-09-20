#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>

#include "leveldb/db.h"

using namespace std;

class dbController {

private:
    leveldb::DB* db;
    leveldb::Options options;

public:

	int connect(string dataBase);
	void CloseDB();
	int addNewUser(Json::Value &user);
};

#endif 
