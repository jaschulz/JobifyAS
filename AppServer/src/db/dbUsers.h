#ifndef DBUSERS_H
#define DBUSERS_H

#include <unistd.h>
#include <stdlib.h>
#include "leveldb/db.h"
#include <jsoncpp/json/json.h>
#include "dbController.h"
#include "../Model/Location.h"

using namespace std;

class dbUsers : public dbController {

public:
	string editProfile(string &key,Json::Value &user);

	string addProfile(string &key,Json::Value &user);

	Json::Value getProfile(string &email);

	Json::Value setLocation(string &key, Location &location);

	string addContact(string &key,Json::Value &user);

	Json::Value getContacts(string &key);
private:
	string putContact(string &user1,string &idNewContact);
};

#endif 
