#include <string>
#include "dbUsers.h"
#include "leveldb/db.h"

using namespace std;

string dbUsers::editProfile(Json::Value &user){
	leveldb::WriteOptions writeOptions;
	string username = user.get("email", "").asString();
	string strJson;
        string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&strJson);
	if (st.ok() != 1) {
		if (st.ToString().compare("NotFound: ") == 0) {
			db->Put(writeOptions, username, user.toStyledString());
		} else {
			error = "Failed: " + st.ToString();
		}
	} else {
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
		db->Put(writeOptions, username, root.toStyledString());
		user = root;
	}
	return error;
}
