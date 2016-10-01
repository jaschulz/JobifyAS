#ifndef DBUSERS_H
#define DBUSERS_H

#include <unistd.h>
#include <stdlib.h>
#include "leveldb/db.h"
#include <json/json.h>
#include "dbController.h"

using namespace std;

class dbUsers : public dbController {

public:
	string editProfile(string &key,Json::Value &user);

	string getProfile(Json::Value &user);

	string setLocation(string &key,Json::Value &user);
};

#endif 
