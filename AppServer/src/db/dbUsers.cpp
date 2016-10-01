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
	//string username = user.get("email", "").asString();
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
			/*for( Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++ ) {
			    // Print depth. 
			    for( int tab = 0 ; tab < depth; tab++) {
			       printf("-"); 
			    }
			    printf(" subvalue(");
			    PrintJSONValue(itr.key());
			    printf(") -");
			    PrintJSONTree( *itr, depth); 
			}*/
			string fName = root.get("first_name", "").asString();
			string lName = root.get("last_name", "").asString();
			root["first_name"] = user.get("first_name", fName).asString();
			root["last_name"] = user.get("last_name", lName).asString();
			db->Put(writeOptions, key, root.toStyledString());
			user = root;
		}
	}
	return error;
}

string dbUsers::setLocation(string &key,Json::Value &user){
	leveldb::WriteOptions writeOptions;
	//string username = user.get("email", "").asString();
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
