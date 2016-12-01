/*
 * dbToken.cpp
 *
 *  Created on: 30/11/2016
 *      Author: javier
 */

#include "dbToken.h"

bool dbToken::addNewToken(string &token, string &user, string &error){
	leveldb::WriteOptions writeOptions;
	leveldb::Status st = db->Put(writeOptions, token, user);
	if (st.ok() != 1) {
		error = "Failure: " + st.ToString();
		return false;
	}
	return true;
}

string dbToken::getUser(string &token,string &error){
	string user;
	leveldb::Status st = db->Get(leveldb::ReadOptions(), token, &user);
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
		return error;
	}
	return user;

}
