/*
 * dbToken.h
 *
 *  Created on: 30/11/2016
 *      Author: javier
 */

#ifndef SRC_DB_DBTOKEN_H_
#define SRC_DB_DBTOKEN_H_

#include <unistd.h>
#include <stdlib.h>

#include "leveldb/db.h"
#include <jsoncpp/json/json.h>
#include "dbController.h"

using namespace std;

class dbToken: public dbController  {
public:

	bool addNewToken(string &token, string &user, string &error);

	string getUser(string &token, string &error);
};

#endif /* SRC_DB_DBTOKEN_H_ */
