#include "Profile.h"
#include "../utils/Encrypt.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>

Profile::Profile(const string &mail, const string &pass) {
	this->email = mail;//value.get("email", "").asString();
	this->password = encrypt::sha256(pass);//value.get("password", "").asString());
	this->skills.empty();
	this->contacts.empty();
	this->pendingContacts.empty();
	emptyJsonProfile();
}

void Profile::jsonToProfile(const Json::Value &value) {
	this->email = value.get("email", "").asString();
	this->password = encrypt::sha256(value.get("password", "").asString());
	this->first_name = value.get("first_name", "").asString();
	this->last_name = value.get("last_name", "").asString();
}

void Profile::emptyJsonProfile() {
	jsonProfile["email"] = "";
	jsonProfile["password"] = "";	
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
	jsonProfile["password"] = password;
	return jsonProfile;
}

Json::Value &Profile::publicProfileToJSON() {
	jsonProfile["email"] = email;
	jsonProfile["first_name"] = first_name;
	jsonProfile["last_name"] = last_name;
	return jsonProfile;
}

void Profile::setEmail(const string &mail) {
    email = mail;
}

const string &Profile::getEmail() const {
    return email;
}

void Profile::setPassword(const string &pass) {
    this->password = encrypt::sha256(pass);
}

const string &Profile::getPassword() const {
    return password;
}

Profile::~Profile() {
}
