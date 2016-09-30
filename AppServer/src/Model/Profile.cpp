#include "Profile.h"
#include "../utils/Encrypt.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>

Profile::Profile(const Json::Value &value) {
	this->email = value.get("email", "").asString();
	this->password = encrypt::sha256(value.get("password", "").asString());
	this->first_name = value.get("first_name", "").asString();
	this->last_name = value.get("last_name", "").asString();
}

Profile::Profile(const string &mail, const string &pass, const string &firstName, const string &lastName) {
	this->email = mail;
	this->password = encrypt::sha256(pass);
	this->first_name = firstName;
	this->last_name = lastName;
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
