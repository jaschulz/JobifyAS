#include <iostream>
#include <sstream>
#include <string>

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
/*
    // Add 256 values to the database
    leveldb::WriteOptions writeOptions;
    for (unsigned int i = 0; i < 256; ++i)
    {
        ostringstream keyStream;
        keyStream  << i;
        
        ostringstream valueStream;
        valueStream << "Test data value: " << i;
        
        db->Put(writeOptions, keyStream.str(), valueStream.str());
    }
    
    // Iterate over each item in the database and print them
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    
    for (unsigned int i = 0; i < 256; ++i)
    {
	std::string value;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),static_cast<ostringstream*>( &(ostringstream() <<i) )->str(),&value);
        cout << i << " : " << value << endl;
    }
    
    if (false == it->status().ok())
    {
        cerr << "An error was found during the scan" << endl;
        cerr << it->status().ToString() << endl; 
    }

	
    
    delete it;
    */

int dbController::addNewUser(Json::Value &user){
    leveldb::WriteOptions writeOptions;
	string username = user.get("email", "").asString();
	string password = user.get("password", "").asString();

        
        db->Put(writeOptions, username, password);
    
}

void dbController::CloseDB(){
    // Close the database
    delete db;
}

