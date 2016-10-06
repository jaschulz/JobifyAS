#include <string>
#include "dbUsers.h"
#include "leveldb/db.h"


#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

string dbUsers::editProfile(string &key,Json::Value &user){
	leveldb::WriteOptions writeOptions;
	string strJson;
        string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),key,&strJson);
	if (st.ok() != 1) {
		if (st.ToString().compare("NotFound: ") == 0) {
			db->Put(writeOptions, key, user.toStyledString());
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
		} else {
			for(Json::Value::iterator it = user.begin(); it !=user.end(); ++it)
			{
				Json::Value keyValue = it.key();
				Json::Value value = (*it);
				root[keyValue.asString()] = value;
			}
			db->Put(writeOptions, key, root.toStyledString());
			user = root;
		}
	}
	return error;
}

string dbUsers::setLocation(string &key,Json::Value &user){
	leveldb::WriteOptions writeOptions;
	string strJson;
        string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),key,&strJson);
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
	} else {
		Json::Value root;   
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( strJson.c_str(), root );     //parse process
		if ( !parsingSuccessful )
		{
			error = reader.getFormattedErrorMessages();;
		}
		root["location"]["latitude"] = user["location"]["latitude"];
		root["location"]["longitude"] = user["location"]["longitude"];
		db->Put(writeOptions, key, root.toStyledString());
		user = root;
	}
	return error;
}

string dbUsers::getProfile(Json::Value &user){
	string username = user.get("email", "").asString();
	string strJson;
        string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&strJson);
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
	} else {
		Json::Value root;   
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( strJson.c_str(), user );     //parse process
		if ( !parsingSuccessful )
		{
			error = reader.getFormattedErrorMessages();;
		}
	}
	return error;
}
