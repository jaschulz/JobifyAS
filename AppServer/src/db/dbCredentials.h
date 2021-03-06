#ifndef DBCREDENTIALS_H
#define DBCREDENTIALS_H

#include <unistd.h>
#include <stdlib.h>

#include "leveldb/db.h"
#include <jsoncpp/json/json.h>
#include "dbController.h"

using namespace std;

class dbCredentials: public dbController {

public:
	bool addNewUser(Json::Value user, string &error);

	bool verifyLogin(jobifyCredentials &credentials, string &error);

	bool isValidToken(string email, string token);

	void updateToken(Credentials &credentials, string &error);
};

#endif 
