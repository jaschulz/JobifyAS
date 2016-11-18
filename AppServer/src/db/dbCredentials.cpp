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
	string pass;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
	if (st.ok() != 1) {
		if (st.ToString().compare("NotFound: ") == 0) {
		        db->Put(writeOptions, username, user.toStyledString()); 
			return true;   
		} else {
			error = "Failed: " + st.ToString();
		}
	} else {
		error = "Email already exists.";
	}
	return false;
}

bool dbCredentials::verifyLogin(jobifyCredentials &credentials, string &error){
	string username = credentials.getEmail();
	string password = credentials.getPassword();
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
	string strJson;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),email,&strJson);
	Json::Value root;   
	Json::Reader reader;
	if ( !reader.parse( strJson.c_str(), root ) )
	{
		return false;
	} else {
		if (token.compare(root.get("token", "").asString()) != 0) { 
			return false;
		}
	}
	return true;
}
