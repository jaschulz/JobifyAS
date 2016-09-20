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

int dbController::addNewUser(Json::Value &user){
    leveldb::WriteOptions writeOptions;
	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();

        
        db->Put(writeOptions, username, password);
	return 1;
    
}

//verifyLogin
int dbController::verifyLogin(Json::Value &user){

	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();
	string pass;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),username,&pass);
        

    if (password.compare(pass) != 0) {
    
	return -1;
}
	return 1;
    
}


