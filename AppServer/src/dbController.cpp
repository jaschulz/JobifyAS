#include <iostream>
#include <sstream>
#include <string>
#include "dbController.h"

#include "leveldb/db.h"

using namespace std;

int dbController::connect(string dataBase)
{
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, dataBase.c_str(), &db);

    if (false == status.ok())
    {
        cerr << "Unable to open/create test database './testdb'" << endl;
        cerr << status.ToString() << endl;
        return -1;
    }
	return 1;
}


void dbController::CloseDB(){
    // Close the database
    delete db;
}

string dbController::addNewUser(Json::Value &user){
    leveldb::WriteOptions writeOptions;
	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();

	string pass;
	string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
	if (false == st.ok()) {
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
string dbController::verifyLogin(Json::Value &user){

	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();
	string pass;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
        string error = "";

    if (password.compare(pass) != 0) {
    
	error = "Incorrect user or password";
}
	return error;
    
}


