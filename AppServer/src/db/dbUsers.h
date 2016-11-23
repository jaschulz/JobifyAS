#ifndef DBUSERS_H
#define DBUSERS_H

#include "leveldb/db.h"
#include "dbController.h"
#include "../Model/Location.h"
#include "../Model/Profile.h"
#include <jsoncpp/json/json.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

class dbUsers : public dbController {

public:
	string editProfile(string &key,Json::Value &user);

	Json::Value searchProfile(Json::Value &filter, string &error);

	string addProfile(string &key,Json::Value &user);

	Json::Value getProfile(string &email);

	Json::Value setLocation(string &key, Location &location);

	//string addContact(string &key,Json::Value &user);

	bool addContact(string &key,Json::Value &user, string &error, int code);

	Json::Value getContacts(string &key);

	bool manageContacts(Profile &sender, Profile &receiver, string &error,string contactLevel);

	bool sendInvitation(Profile &sender, Profile &receiver, string &error, int &code);

	bool addContact(string &key,Json::Value &user, string &error, int &code);

	bool acceptInvitation(Profile &sender, Profile &invitee, string &error, int code);
/*
	bool moveToContacts(string &user, string &newContact, string &error);

	bool moveContact(string &user, string &newContact, string &error, string source);*/
	bool moveContact(std::vector<std::string> &source, std::vector<std::string> &destination, string value);

private:
	string putContact(string &user1,string &idNewContact);

	bool addSentInvitation(Profile &sender, Profile &receiver, string &error);

	bool addReceivedInvitation(Profile &sender, Profile &receiver, string &error);

	//bool validateUser(string &key, string &userJson,string &error, int &code);

};

#endif 
