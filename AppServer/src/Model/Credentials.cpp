#include "Credentials.h"
#include "../utils/utils.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>

Credentials::Credentials(const string &mail, const string &pass, const string &token ) {
	this->email = mail;
	this->password = utils::sha256(pass);
	this->token = token;
}

void Credentials::setEmail(const string &mail) {
    email = mail;
}

const string &Credentials::getEmail() const {
    return email;
}

void Credentials::setPassword(const string &pass) {
    this->password = utils::sha256(pass);
}

const string &Credentials::getPassword() const {
    return password;
}

Json::Value Credentials::toJSON() {
    Json::Value value;
    value["email"] = email;
    value["password"] = password;
    value["token"] = token;
    return value;
}

Credentials::~Credentials() {
}
