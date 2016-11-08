#include <iostream>
#include <sstream>
#include <string>
#include "dbCredentials.h"
#include "../utils/utils.h"

#include "leveldb/db.h"

using namespace std;

bool dbCredentials::addNewUser(Json::Value user, string &error){
	leveldb::WriteOptions writeOptions;
	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();
	string pass;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
	if (st.ok() != 1) {
		if (st.ToString().compare("NotFound: ") == 0) {
		        db->Put(writeOptions, username, user.asString());    
		} else {
			error = "Failed: " + st.ToString();
			return false;
		}
	} else {
		error = "Email already exists.";
		return false;
	}
	return true;
}

//verifyLogin
bool dbCredentials::verifyLogin(Json::Value &user, string &error){
	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();
	string strJson;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&strJson);
	Json::Value root;   
	Json::Reader reader;
	if ( !reader.parse( strJson.c_str(), root ) )
	{
		error = reader.getFormattedErrorMessages();
		return false;
	} else {
		if (password.compare(root.get("password", "").asString()) != 0) {    
			error = "Login failed";
			return false;
		}
	}
	return true;
}

	bool dbCredentials::isValidToken(string email, string token){
		//TODO: IMPLEMENT THIS FUNCTION
		return true;
	}

/*Json::Value dbController::getProfile(string &email){
	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();
	string pass;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
        string error = "";
	if (password.compare(pass) != 0) {    
		error = "Login failed";
	}
	return error;
}*/
