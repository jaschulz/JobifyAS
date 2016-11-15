#include "Profile.h"
#include <string>
#include <iostream>
#include "Location.h"

Profile::Profile(const string &mail):location() {
	this->email = mail;
	this->skills.empty();
	this->contacts.empty();
	this->pendingContacts.empty();
	//emptyJsonProfile();
}

Profile::Profile(const string &mail, const double &latitude, const double &longitude):location(latitude,longitude) {
	this->email = mail;
	//emptyJsonProfile();
}

void Profile::jsonToProfile(const Json::Value &value) {
	this->email = value.get("email", "").asString();
	this->first_name = value.get("first_name", "").asString();
	this->last_name = value.get("last_name", "").asString();
}

Json::Value Profile::profileToJSON() {	
	Json::Value jsonProfile;
	jsonProfile["email"] = email;
	return jsonProfile;
}

Json::Value Profile::publicProfileToJSON() {
	Json::Value jsonProfile;
	jsonProfile["email"] = email;
	jsonProfile["first_name"] = first_name;
	jsonProfile["last_name"] = last_name;
	jsonProfile["about"] = about;
	jsonProfile["location"]["latitude"] = location.getLatitude();
	jsonProfile["location"]["longitude"] = location.getLongitude();
	jsonProfile["skills"] = Json::arrayValue;
	jsonProfile["contacts"] = Json::arrayValue;
	jsonProfile["pendingContacts"] = Json::arrayValue;
	return jsonProfile;
}

void Profile::setEmail(const string &mail) {
    email = mail;
}

const string &Profile::getEmail() const {
    return email;
}


Profile::~Profile() {
}
