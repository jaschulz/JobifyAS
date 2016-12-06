#include "dbUsers.h"
#include "leveldb/db.h"
#include "../utils/utils.h"
#include <string>
#include <cmath>
#include <algorithm> 
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

#define MAX_DISTANCE 21000

string dbUsers::editProfile(Profile &profile) {
	leveldb::WriteOptions writeOptions;
	string strJson;
	string error = "";
	string email = profile.getEmail();
	leveldb::Status st = db->Get(leveldb::ReadOptions(), email, &strJson);
	if (st.ok() != 1) {
		error = "Failed: " + email + " - " + st.ToString();
	} else {
		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(strJson.c_str(), root)) {
			error = reader.getFormattedErrorMessages();
		} else {
			Json::Value user = profile.editableProfileToJSON();
			for (Json::Value::iterator it = user.begin(); it != user.end();
					++it) {
				Json::Value keyValue = it.key();
				Json::Value value = (*it);
				cout << keyValue << ": " << value << endl;
				root[keyValue.asString()] = value;
			}
			db->Put(writeOptions, email, root.toStyledString());
			user = root;
		}
	}
	return error;
}

Json::Value dbUsers::searchProfile(Json::Value &filter, string &error) {
	const leveldb::Slice startSlice;
	const leveldb::Slice endSlice;
	string strJson;
	Json::Value usersArray;
	Json::Value result;
	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		if (!it->value().empty()) {
			leveldb::Slice keySlice = it->key();
			leveldb::Slice dataSlice = it->value();
			Json::Value userProfile;
			Json::Reader reader;
			if (!reader.parse(dataSlice.ToString().c_str(), userProfile)) {
				error = reader.getFormattedErrorMessages();
				delete it;
				cout << "error ->" << error << endl;
				return result;
			} else {
				//cout << "userProfile.toStyledString(): "
					//	<< userProfile.toStyledString() << endl;
				Profile profile(userProfile);
				if (filter.isNull()) {
					usersArray.append(profile.publicProfileToJSON());
					break;
				}
				//cout << "despues de filster ins null"<< endl;
				string filter_job_pos = filter.get("job_position",
						profile.getJobPosition()).asString();
				cout << "filter_job_pos ->" << filter_job_pos << endl;
				string user = filter.get("user", profile.getEmail()).asString();
				transform(user.begin(), user.end(), user.begin(), ::toupper);
				cout << "user ->" << user << endl;
				double latitude = filter["Location"].get("latitude",
						profile.getLocation().getLatitude()).asDouble();
				cout << "latitude ->" << latitude << endl;
				double longitude = filter["Location"].get("longitude",
						profile.getLocation().getLongitude()).asDouble();
				cout << "longitude ->" << longitude << endl;
				bool withinRange = true;
				if (!filter["range"].isNull()) {
					double range = filter.get("range", MAX_DISTANCE).asDouble();
					cout << "range ->" << range << endl;
					Location location(latitude, longitude);
					withinRange = false;
					if (profile.getLocation().isValid()) {
						cout << "is valid" << endl;
						withinRange = (location.distanceTo(
								profile.getLocation()) < range);
					}

				}
				std::string mail = profile.getEmail();
				transform(mail.begin(), mail.end(), mail.begin(), ::toupper);
				std::string lName = profile.getEmail();
				transform(lName.begin(), lName.end(), lName.begin(), ::toupper);
				if (profile.getJobPosition().compare(filter_job_pos) == 0
						&& (mail.find(user) != std::string::npos || lName.find(user)
										!= std::string::npos) && withinRange) {
					if (filter["skills"].isNull()) {
						usersArray.append(profile.publicProfileToJSON());
						//cout << "skills.isNull ->" << endl;
					} else {
						Json::Value skills = filter["skills"];

						//cout << "Antes de iterar en skills->"
						//	<< skills.toStyledString() << endl;
						for (Json::Value::iterator it =
								filter["skills"].begin();
								it != filter["skills"].end(); ++it) {
							string value = it->asString();
							//cout << "skills: " << value << endl;
							if (utils::setContainsValue(profile.getSkills(),
									value)) {
								usersArray.append(profile.publicProfileToJSON());
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

std::list<Profile> dbUsers::getMostPopularUsers(Json::Value &filter,
		string &error) {
	const leveldb::Slice startSlice;
	const leveldb::Slice endSlice;
	string strJson;
	std::list<Profile> profiles;
	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		if (!it->value().empty()) {
			leveldb::Slice keySlice = it->key();
			leveldb::Slice dataSlice = it->value();
			Json::Value userProfile;
			Json::Reader reader;
			//cout << "dataSlice.ToString()" << dataSlice.ToString() << endl;
			if (!reader.parse(dataSlice.ToString().c_str(), userProfile)) {
				error = reader.getFormattedErrorMessages();
				delete it;
				return profiles;
			} else {
				Profile profile(userProfile);
				string job_position = filter.get("job_position",
						profile.getJobPosition()).asString();
				bool hasSkill;
				if (!filter["skills"].isNull()){
					hasSkill = false;
					for (Json::Value::iterator it =
							filter["skills"].begin();
							it != filter["skills"].end(); ++it) {
						string value = it->asString();
						if (utils::setContainsValue(profile.getSkills(),
								value)) {
							hasSkill = true;
							break;
						}
					}
				} else {
					hasSkill = true;
				}
				if (job_position == profile.getJobPosition() && hasSkill) {
					profiles.push_back(profile);
				}
			}
		}
	}
	assert(it->status().ok());  // Check for any errors found during the scan
	delete it;
	return profiles;
}

string dbUsers::addProfile(string &key, Json::Value &user) {
	leveldb::Status st = db->Put(leveldb::WriteOptions(), key,
			user.toStyledString());
	string error = "";
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
	}
	return error;
}

Json::Value dbUsers::setLocation(string &key, Location &location) {
	leveldb::WriteOptions writeOptions;
	string strJson;
	string error = "";
	Json::Value root;
	leveldb::Status st = db->Get(leveldb::ReadOptions(), key, &strJson);
	if (st.ok() != 1) {
		error = "Failed: " + st.ToString();
	} else {
		Json::Reader reader;
		if (reader.parse(strJson.c_str(), root)) {
			root["location"]["latitude"] = location.getLatitude();
			root["location"]["longitude"] = location.getLongitude();
			db->Put(writeOptions, key, root.toStyledString());
		} else {
			root["error"] = reader.getFormattedErrorMessages();
		}
	}
	return root;
}

string dbUsers::putContact(string &user1, string &idNewContact) {
	Json::Value root;
	leveldb::WriteOptions writeOptions;
	Json::Reader reader;
	string error = "";
	bool parsingSuccessful = reader.parse(user1.c_str(), root);  //parse process
	if (!parsingSuccessful) {
		error = reader.getFormattedErrorMessages();
		return error;
	}
	string key = root.get("email", "").asString();
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

bool dbUsers::addContact(Profile &sender, Profile &receiver, string &error) {
	string key = sender.getEmail();
	string rec_mail = receiver.getEmail();
	if (utils::setContainsValue(sender.getContacts(), rec_mail)) {
		error = rec_mail + " is already a contact.";
		cout<<error<<endl;
		return false;
	}
	sender.addContact(rec_mail);
	receiver.addContact(key);
	cout<<sender.profileToJSON().toStyledString()<<endl;
	leveldb::Status st = db->Put(leveldb::WriteOptions(), key,
			sender.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = "Error finding sender: " + st.ToString();
		return false;
	}
	cout<<receiver.profileToJSON().toStyledString()<<endl;
	st = db->Put(leveldb::WriteOptions(), rec_mail,
			receiver.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = "Error finding receiver: " + st.ToString();
		return false;
	}
	return true;
}

bool dbUsers::addSentInvitation(Profile &sender, Profile &receiver,
		string &error) {
	string key = sender.getEmail();
	string rec_mail = receiver.getEmail();
	if (utils::setContainsValue(sender.getInvitationsSent(), rec_mail)) {
		error = "An invitation to " + rec_mail + " has already been sent.";
		cout<<error<<endl;
		return false;
	}
	if (utils::setContainsValue(sender.getContacts(), rec_mail)) {
		error = rec_mail + " is already a contact.";
		cout<<error<<endl;
		return false;
	}
	if (utils::setContainsValue(sender.getInvitationsReceived(), rec_mail)) {
		if (!moveContact(sender.getInvitationsReceived(), sender.getContacts(),
				rec_mail)) {
			return false;
		}
		if (!moveContact(receiver.getInvitationsSent(), receiver.getContacts(),
				sender.getEmail())) {
			moveContact(sender.getContacts(), sender.getInvitationsReceived(),
					rec_mail);
			return false;
		}
		leveldb::Status st = db->Put(leveldb::WriteOptions(), sender.getEmail(),
				sender.profileToJSON().toStyledString());
		if (st.ok() != 1) {
			error = st.ToString();
			return false;
		}
		st = db->Put(leveldb::WriteOptions(), receiver.getEmail(),
				receiver.profileToJSON().toStyledString());
		if (st.ok() != 1) {
			error = st.ToString();
			return false;
		}
		return true;
	}
	sender.addSentInvitation(rec_mail);
	leveldb::Status st = db->Put(leveldb::WriteOptions(), key,
			sender.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = st.ToString();
		return false;
	}
	return true;
}

bool dbUsers::addReceivedInvitation(Profile &sender, Profile &receiver,
		string &error) {
	string key = receiver.getEmail();
	string s_mail = sender.getEmail();
	receiver.addReceivedInvitation(s_mail);
	leveldb::Status st = db->Put(leveldb::WriteOptions(), key,
			receiver.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = st.ToString();
		return false;
	}
	return true;
}

bool dbUsers::manageContacts(Profile &sender, Profile &receiver, string &error,
		string contactLevel) {
	/*	Json::Value root;
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
	 } */
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

bool dbUsers::sendInvitation(Profile &sender, Profile &receiver, string &error,
		int &code) {
	error = "";
	code = 201;
	/*std::string value;
	leveldb::Status st = db->Get(leveldb::ReadOptions(), receiver.getEmail(), &value);
	if (!addSentInvitation(sender, receiver, error)
			|| !addReceivedInvitation(sender, receiver, error)) {*/
	if (!addContact(sender, receiver, error)) {
		code = 401;
		return false;
	}
	return true;
}
/*
 bool dbUsers::acceptInvitation(string &user, string &newContact, string &error){
 return moveContact(user, newContact, error,"invitationsReceived");
 }

 bool dbUsers::moveToContacts(string &user, string &newContact, string &error){
 return moveContact(user, newContact, error,"invitationsSent");
 }
 */

bool dbUsers::moveContact(std::set<std::string> &source,
		std::set<std::string> &destination, string value) {
	cout << " acceptInvitation 1" << endl;
	return utils::moveFromSetToSet(source, destination, value);
}

bool dbUsers::acceptInvitation(Profile &sender, Profile &invitee, string &error,
		int code) {
	error = "";
	if (!moveContact(sender.getInvitationsSent(), sender.getContacts(),
			invitee.getEmail())) {
		code = 404;
		return false;
	}
	if (!moveContact(invitee.getInvitationsReceived(), invitee.getContacts(),
			sender.getEmail())) {
		code = 404;
		moveContact(sender.getContacts(), sender.getInvitationsSent(),
				invitee.getEmail());
		return false;
	}
	leveldb::Status st = db->Put(leveldb::WriteOptions(), sender.getEmail(),
			sender.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = st.ToString();
		return false;
	}
	st = db->Put(leveldb::WriteOptions(), invitee.getEmail(),
			invitee.profileToJSON().toStyledString());
	if (st.ok() != 1) {
		error = st.ToString();
		return false;
	}
	code = 201;
	return true;
}

void dbUsers::recommendUser(Profile &sender, Profile &receiver) {

	cout << "who recommends:" << sender.getEmail() << endl;
	receiver.addRecommendation(sender.getEmail());
	leveldb::Status st = db->Put(leveldb::WriteOptions(), receiver.getEmail(),
			receiver.profileToJSON().toStyledString());

}

Json::Value dbUsers::getContacts(string &key) {
	Json::Value result;
	Json::Value contacts;
	string strJson;
	string error = "";
	leveldb::Status st = db->Get(leveldb::ReadOptions(), key, &strJson);
	if (st.ok() != 1) {
		result["error"] = "Failed: " + st.ToString();
	} else {
		Json::Reader reader;
		if (!reader.parse(strJson.c_str(), result)) {
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
	leveldb::Status st = db->Get(leveldb::ReadOptions(), email, &strJson);
	if (st.ok() != 1) {
		user["error"] = "Failed: " + st.ToString();
		return user;     //NULL;
	} else {
		Json::Reader reader;
		if (!reader.parse(strJson.c_str(), user)) {
			user["error"] = reader.getFormattedErrorMessages();
			return user;     //NULL;
		}
	}
	return user;
}
