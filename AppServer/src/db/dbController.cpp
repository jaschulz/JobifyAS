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
        cerr << "Unable to open/create test database './"+dataBase+"'" << endl;
        cerr << status.ToString() << endl;
        return -1;
    }
	return 1;
}


void dbController::CloseDB(){
    // Close the database
    delete db;
}
