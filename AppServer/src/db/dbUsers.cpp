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
	leveldb::Status st = db->Get(leveldb::ReadOptions(),key,&strJson);
	if (st.ok() != 1) {
		error = "Failed1: " + st.ToString();
	} else {
		Json::Value root;   
		Json::Reader reader;
		if ( !reader.parse( strJson.c_str(), root ) )
		{
			error = reader.getFormattedErrorMessages();
		} else {
			for(Json::Value::iterator it = user.begin(); it !=user.end(); ++it)
			{
				Json::Value keyValue = it.key();
				Json::Value value = (*it);
				cout<<keyValue<<": "<<value<<endl;
				root[keyValue.asString()] = value;
			}
			db->Put(writeOptions, key, root.toStyledString());
			user = root;
		}
	}
	return error;
}

string dbUsers::addProfile(string &key,Json::Value &user){
	leveldb::Status st = db->Put(leveldb::WriteOptions(), key, user.toStyledString());
        string error = "";
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
	}
	return error;
}

Json::Value dbUsers::setLocation(string &key,Location &location){
	leveldb::WriteOptions writeOptions;
	string strJson;
        string error = "";
	Json::Value root;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),key,&strJson);
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
	} else {   
		Json::Reader reader;
		if (reader.parse( strJson.c_str(), root ))
		{			
			root["location"]["latitude"] = location.getLatitude();
			root["location"]["longitude"] = location.getLongitude();
			db->Put(writeOptions, key, root.toStyledString());
		} else {
			root["error"] = reader.getFormattedErrorMessages();
		}
	}
	return root;
}



string dbUsers::putContact(string &user1,string &idNewContact){
	Json::Value root;   
	leveldb::WriteOptions writeOptions;
	Json::Reader reader;
	string error = "";
	bool parsingSuccessful = reader.parse( user1.c_str(), root );     //parse process
	if ( !parsingSuccessful )
	{
		error = reader.getFormattedErrorMessages();
		return error;
	}
	string key = root.get("email","").asString();
	Json::Value newContact;
	newContact["id"] = idNewContact;
	Json::Value contactsArray = root["contacts"];
	contactsArray.append(newContact);
	root["contacts"] = contactsArray;	
//	cout<<"putContact key: "<<key<<endl;
	leveldb::Status st = db->Put(writeOptions, key, root.toStyledString());
	if (st.ok() != 1) {
		error = "Failed3: " + st.ToString();
	} 
	return error;
}

string dbUsers::addContact(string &key,Json::Value &user){
	leveldb::WriteOptions writeOptions;
	string loggedUserJson;
        string error = "";	
	string newContactJson;
	string	newContact = user.get("email","").asString();
//	cout<<"newContact: "<<newContact<<endl;
//	cout<<"key: "<<key<<endl;
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),key,&loggedUserJson);
	if (st.ok() != 1) {
		error = "Failed4: " + st.ToString();
		return error;
	} 
	st =  db->Get(leveldb::ReadOptions(),newContact,&newContactJson);
	if (st.ok() != 1) {
		error = "Failed5: " + st.ToString();
		return error;
	}
//	cout<<"loggedUserJson: "<<loggedUserJson<<endl;
	error = putContact(loggedUserJson,newContact);
//		cout<<"addContact newContactJson: "<<newContactJson<<endl;
	error = putContact(newContactJson,key);

	return error;
}
/*
string dbUsers::putPendingContact(string &user1,string &idNewContact){
	Json::Value root;   
	Json::Reader reader;
	string userJson;
	string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),user1,&userJson);
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
		return error;
	} 
	cout<<"aca 1"<<endl;
	bool parsingSuccessful = reader.parse( userJson.c_str(), root );     //parse process
	if ( !parsingSuccessful )
	{
		error = reader.getFormattedErrorMessages();
		return error;
	}
		cout<<"aca 12"<<endl;
	Json::Value newContact;	
	Json::Value contactsArray = root["pendingContacts"];
	if(contactsArray.isMember(idNewContact)) {
		error = "invitation has already been sent.";
		return error;
	}
	cout<<"aca 3 1"<<endl;
	newContact[idNewContact] = idNewContact;
	contactsArray.append(newContact);
	root["pendingContacts"] = contactsArray;	
	st = db->Put(leveldb::WriteOptions(), user1, root.toStyledString());
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
	} 
	return error;
}

string dbUsers::addPendingContact(string &key,Json::Value &user){
        string error = "";	
	string	newContact = user.get("email","").asString();	
	cout<<"antes"<<endl;
	error = putPendingContact(key,newContact);
	if (error == "") {
			cout<<"durantes"<<endl;
		error = putPendingContact(newContact,key);
	}
		cout<<"des"<<endl;
	return error;
}*/

Json::Value  dbUsers::getContacts(string &key) {
	Json::Value result;
	Json::Value contacts;
	string strJson;
        string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),key,&strJson);
	if (st.ok() != 1) {
		result["error"] = "Failed: " + st.ToString();
	} else {  
		Json::Reader reader;
		if ( !reader.parse( strJson.c_str(), result ) )
		{
			result["error"] = reader.getFormattedErrorMessages();
			return result;
		}
		contacts["contacts"] = result["contacts"];
		return contacts;
	}
	return result;
}

Json::Value dbUsers::getProfile(string &email) {
	Json::Value user;
	string strJson;
        string error = "";
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),email,&strJson);
	if (st.ok() != 1) {
		user["error"] = "Failed: " + st.ToString();
	} else {
		Json::Reader reader;
		if ( ! reader.parse( strJson.c_str(), user ) )
		{
			user["error"] = reader.getFormattedErrorMessages();
		}
	}
	return user;
}
