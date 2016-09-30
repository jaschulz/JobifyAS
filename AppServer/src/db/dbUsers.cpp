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
	return error;
}
