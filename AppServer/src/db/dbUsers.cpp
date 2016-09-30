#include <string>
#include "dbUsers.h"
#include "leveldb/db.h"

using namespace std;

string dbUsers::editProfile(Json::Value &user){
	string username = user.get("email", "").asString();
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&user);
        string error = "";
	if (password.compare(pass) != 0) {    
		error = "Login failed";
	}
	return error;
}
