#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>

#include "leveldb/db.h"
#include <json/json.h>

using namespace std;

class dbController {

private:
    leveldb::DB* db;
    leveldb::Options options;

public:

	int connect(string dataBase);
	void CloseDB();
	void addNewUser(Json::Value &user, string &error);
	int verifyLogin(Json::Value &user);
};

#endif 
