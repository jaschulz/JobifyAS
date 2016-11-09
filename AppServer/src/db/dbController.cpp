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
/*
    if (false == status.ok())
    {
        cerr << "Unable to open/create test database './"+dataBase+"'" << endl;
        cerr << status.ToString() << endl;
        return -1;
    }*/
	return status.ok();
}


void dbController::CloseDB(){
    // Close the database
    delete db;
}
/*
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
*/
