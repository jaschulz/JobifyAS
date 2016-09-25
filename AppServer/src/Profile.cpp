#include "Profile.h"
#include "Encrypt.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>

Profile::Profile(const Json::Value &value) {
    this->email = value.get("email", "").asString();
    this->password = value.get("password", "").asString();
}

    Profile::Profile(const string &mail, const string &pass) {

    this->email = mail;
    this->password = pass;
}

Json::Value &Profile::profileToJSON() {
    jsonProfile["email"] = email;
    jsonProfile["password"] = encrypt::sha256(password);
    return jsonProfile;
}

void Profile::setEmail(const string &mail) {
    email = mail;
}

const string &Profile::getEmail() const {
    return email;
}

const string &Profile::getPassword() const {
    return password;
}

void Profile::setPassword(const string &pass) {
    this->password = pass;
}

Profile::~Profile() {
}
