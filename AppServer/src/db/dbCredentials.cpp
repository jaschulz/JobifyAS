#include <iostream>
#include <sstream>
#include <string>
#include "dbCredentials.h"
#include "../utils/utils.h"

#include "leveldb/db.h"

using namespace std;

string dbCredentials::addNewUser(Json::Value &user){
	leveldb::WriteOptions writeOptions;
	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();

	string pass;
	string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
	if (st.ok() != 1) {
		if (st.ToString().compare("NotFound: ") == 0) {
		        db->Put(writeOptions, username, password);    
		} else {
			error = "Failed: " + st.ToString();
		}
	} else {
		error = "Email already exists.";
	}
	return error;
}

//verifyLogin
string dbCredentials::verifyLogin(Json::Value &user){

	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();
	string pass;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
        string error = "";
	if (password.compare(pass) != 0) {    
		error = "Login failed";
	}
	return error;
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
