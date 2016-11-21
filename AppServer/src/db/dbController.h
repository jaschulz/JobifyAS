#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>

#include "leveldb/db.h"
#include "../Model/jobifyCredentials.h"
#include <jsoncpp/json/json.h>

using namespace std;

class dbController {

protected:
    leveldb::DB* db;
    leveldb::Options options;

public:
	int connect(string dataBase);
	void CloseDB();
	string printDB();
};

#endif 
