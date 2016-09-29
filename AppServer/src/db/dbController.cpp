#include <iostream>
#include <sstream>
#include <string>
#include "dbController.h"
#include "../utils/Encrypt.h"

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
string dbController::verifyLogin(Json::Value &user){

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

string dbController::printDB(){


    // Iterate over each item in the database and print them
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        cout << it->key().ToString() << " : " << it->value().ToString() << endl;
    }
    string error ="";
    if (false == it->status().ok())
    {
        cerr << "An error was found during the scan" << endl;
        cerr << it->status().ToString() << endl; 
	error = "Failed: " + it->status().ToString();
    }
	delete it;
	return error;
}

