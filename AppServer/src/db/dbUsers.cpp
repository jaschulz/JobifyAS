#include "dbUsers.h"
#include "leveldb/db.h"
#include "../utils/utils.h"
#include <string>
#include <algorithm> 
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



Json::Value dbUsers::searchProfile(Json::Value &filter, string &error){
	leveldb::WriteOptions writeOptions;
	const leveldb::Slice startSlice;
	const leveldb::Slice endSlice;
	string strJson;
	Json::Value usersArray;
	Json::Value result;
    	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {               
		// Read the record
		cout<<"Read the record"<<endl;
		if( !it->value().empty() )
		{
			leveldb::Slice keySlice = it->key();
			leveldb::Slice dataSlice = it->value();
			Json::Value userProfile;   
			Json::Reader reader;
		cout<<"dataSlice.ToString()"<<dataSlice.ToString()<<endl;
			if ( !reader.parse( dataSlice.ToString().c_str(), userProfile ) )
			{
				error = reader.getFormattedErrorMessages();
				delete it;					
				cout<<"error ->"<<error<<endl;
				return result;
			} else {
				string job_position = userProfile.get("job_position","" ).asString();
				string email = userProfile.get("email", "").asString();
				transform(email.begin(), email.end(), email.begin(), ::toupper);
				string last_name = userProfile.get("last_name", "").asString();
				transform(last_name.begin(), last_name.end(), last_name.begin(), ::toupper);
				cout<<"job_position: "<<job_position<<endl;
				cout<<"email: "<<email<<endl;
				cout<<"last_name: "<<last_name<<endl;
				string filter_job_pos = filter.get("job_position",job_position).asString();
				string user = filter.get("user",email).asString();
				if (job_position.compare(filter_job_pos) == 0 && (email.find(user) != std::string::npos 
				|| last_name.find(user) != std::string::npos)) {
					if(filter["skills"].isNull()) {
						usersArray.append(userProfile);
										cout<<"skills.isNull ->"<<endl;
					} else {
						Json::Value skills = filter["skills"];
						for(Json::Value::iterator it = skills.begin(); it !=skills.end(); ++it)
						{
							Json::Value keyValue = it.key();
							Json::Value value = (*it);
							string strValue = value.asString();
							if(utils::jsonContainsValue(userProfile["skills"],strValue)){
								usersArray.append(userProfile);
								break;
							}
						}
					}
				}
			}
		}
	}
	assert(it->status().ok());  // Check for any errors found during the scan
	delete it;
	return usersArray;
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
/*
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
}*/

bool dbUsers::addSentInvitation(Profile &sender, Profile &receiver, string &error){
	string key = sender.getEmail();
	sender.addSentInvitation(receiver.getEmail()); 
	leveldb::Status st = db->Put(writeOptions, key, sender.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = trace + st.ToString();
		return false;
	} 
	return true;
}

bool dbUsers::addReceivedInvitation(Profile &sender, Profile &receiver, string &error){
	string key = receiver.getEmail();
	receiver.addReceivedInvitation(sender.getEmail()); 
	leveldb::Status st = db->Put(writeOptions, key, receiver.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = trace + st.ToString();
		return false;
	} 
	return true;
}

bool dbUsers::manageContacts(Profile &sender, Profile &receiver, string &error,string contactLevel){
	Json::Value root;   
	leveldb::WriteOptions writeOptions;
	Json::Reader reader;
	error = "";
	string trace = "dbUsers::manageContacts - " + contactLevel + ": ";
	if ( !reader.parse( loggedUser.c_str(), root ) )
	{
		error = trace + reader.getFormattedErrorMessages();
		return false;
	}
	string key = sender.getEmail();
	Json::Value newContact;
	newContact["id"] = sender.getEmail();
	Json::Value contactsArray = root[contactLevel];
	contactsArray.append(newContact);
	root[contactLevel] = contactsArray;	
//	cout<<"putContact key: "<<key<<endl;
	leveldb::Status st = db->Put(writeOptions, key, root.toStyledString());
	if (st.ok() != 1) {
		error = trace + st.ToString();
		return false;
	} 
	return true;
}
/*
bool dbUsers::validateUser(string &key, Json::Value &userJson, string &error){	
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),key,&userJson);
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
		return false;
	} 
	string trace = "dbUsers::validateUser: " + key + " - ";
	Json::Reader reader;
	if ( !reader.parse( loggedUser.c_str(), userJson ) )
	{
		error = trace + reader.getFormattedErrorMessages();
		return false;
	}
	return true;
}*/

bool dbUsers::sendInvitation(Profile &sender, Profile &receiver, string &error, int &code){
	leveldb::WriteOptions writeOptions;
        error = "";	
	//string	newContact = user.get("email","").asString();
	code = 201;
//	cout<<"newContact: "<<newContact<<endl;
//	cout<<"key: "<<key<<endl;
	/*leveldb::Status st =  db->Get(leveldb::ReadOptions(),key,&loggedUserJson);
	if (st.ok() != 1) {
		code = 404;
		error = "Failed: " + st.ToString();
		return false;
	} 
	st =  db->Get(leveldb::ReadOptions(),newContact,&newContactJson);
	if (st.ok() != 1) {
		code = 404;
		error = "Failed: " + st.ToString();
		return false;
	}
	string newContactJson;
	string loggedUserJson;
	if (validateUser(key,loggedUserJson, error, code) || validateUser(newContact, newContactJson, error, code)) {
		return false;
	}*/
	if(!addSentInvitation(sender,receiver,error) || !addReceivedInvitation(receiver,sender,error)){
		code = 404;
		return false;
	}
//	cout<<"loggedUserJson: "<<loggedUserJson<<endl;
//	error = putContact(loggedUserJson,newContact);
//		cout<<"addContact newContactJson: "<<newContactJson<<endl;
//	error = putContact(newContactJson,key);
	return true;
}

bool dbUsers::acceptInvitation(string &user, string &newContact, string &error){
	return moveContact(user, newContact, error,"invitationsReceived");
}

bool dbUsers::moveToContacts(string &user, string &newContact, string &error){
	return moveContact(user, newContact, error,"invitationsSent");
}

bool dbUsers::moveContact(string &user, string &newContact, string &error, string source){
	Json::Value root;   
	Json::Reader reader;
	error = "";
	string trace = "dbUsers::moveContact - ";
	if ( !reader.parse( user.c_str(), root ) ) {
		error = trace + reader.getFormattedErrorMessages();
		return false;
	}
	if(utils::jsonContainsValue(root[source],newContact)){
		root["contacts"].append(newContact);
		//TODO DELETE newContact from root[source]
		return true;
	}
	return false;
}

bool dbUsers::addContact(string &key,Json::Value &user, string &error, int code){
	leveldb::WriteOptions writeOptions;
	string loggedUserJson;
        error = "";	
	string newContactJson;
	string	newContact = user.get("email","").asString();
	code = 201;
//	cout<<"newContact: "<<newContact<<endl;
//	cout<<"key: "<<key<<endl;
	/*if (validateUser(key, loggedUserJson,error, code) || validateUser(newContact,newContactJson, error, code)) {
		return false;
	}*/
	if(!acceptInvitation(loggedUserJson,newContact,error) || !moveToContacts(newContactJson,key,error)){
		code = 404;
		return false;
	}
	return true;
}

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
	leveldb::Status st =  db->Get(leveldb::ReadOptions(),email,&strJson);
	if (st.ok() != 1) {
		user["error"] = "Failed: " + st.ToString();
		return user;//NULL;
	} else {
		Json::Reader reader;
		if ( ! reader.parse( strJson.c_str(), user ) )
		{
			user["error"] = reader.getFormattedErrorMessages();
			return user;//NULL;
		}
	}
	return user;
}
