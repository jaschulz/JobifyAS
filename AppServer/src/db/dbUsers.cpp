#include <string>
#include "dbUsers.h"
#include "leveldb/db.h"

using namespace std;

string dbUsers::editProfile(Json::Value &user){
	string username = user.get("email", "").asString();
	string strJson;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&strJson);
        string error = "";
	Json::Value root;   
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( strJson.c_str(), root );     //parse process
	if ( !parsingSuccessful )
	{
		error = reader.getFormattedErrorMessages();;
	}
	string fName = root.get("first_name", "").asString();
	string lName = root.get("last_name", "").asString();
	root["first_name"] = user.get("first_name", fName).asString();
	root["last_name"] = user.get("last_name", lName).asString();
	return error;
}
