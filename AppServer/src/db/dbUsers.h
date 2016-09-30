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
	string editProfile(Json::Value &user);
};

#endif 
