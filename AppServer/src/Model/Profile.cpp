#include "Profile.h"
#include "../utils/utils.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>
#include "Location.h"

Profile::Profile(const string &mail, const string &pass):location() {
	this->email = mail;
	this->password = utils::sha256(pass);
	this->skills.empty();
	this->contacts.empty();
	this->pendingContacts.empty();
	emptyJsonProfile();
}

Profile::Profile(const string &mail, const double &latitude, const double &longitude):location(latitude,longitude) {
	this->email = mail;
	emptyJsonProfile();
}

void Profile::jsonToProfile(const Json::Value &value) {
	this->email = value.get("email", "").asString();
	this->password = utils::sha256(value.get("password", "").asString());
	this->first_name = value.get("first_name", "").asString();
	this->last_name = value.get("last_name", "").asString();
}

void Profile::emptyJsonProfile() {
	jsonProfile["email"] = "";
	jsonProfile["first_name"] = "";
	jsonProfile["last_name"] = "";
	jsonProfile["about"] = "";
	jsonProfile["location"]["latitude"] = "";
	jsonProfile["location"]["longitude"] = "";
	jsonProfile["skills"] = Json::arrayValue;
	jsonProfile["contacts"] = Json::arrayValue;
	jsonProfile["pendingContacts"] = Json::arrayValue;
}

Json::Value &Profile::profileToJSON() {
	jsonProfile["email"] = email;
	return jsonProfile;
}

Json::Value &Profile::publicProfileToJSON() {
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

void Profile::setPassword(const string &pass) {
    this->password = utils::sha256(pass);
}

const string &Profile::getPassword() const {
    return password;
}

Profile::~Profile() {
}
