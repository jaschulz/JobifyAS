#ifndef DBCREDENTIALS_H
#define DBCREDENTIALS_H

#include <unistd.h>
#include <stdlib.h>

#include "leveldb/db.h"
#include <json/json.h>
#include "dbController.h"

using namespace std;

class dbCredentials : public dbController {

public:
	string addNewUser(Json::Value &user);
	string verifyLogin(Json::Value &user);
	string printDB();
};

#endif 
