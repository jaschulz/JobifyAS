#include "jobifyCredentials.h"
#include "../utils/utils.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>

jobifyCredentials::jobifyCredentials(const string &mail, const string &pass,
		const string &token) :
		Credentials(mail, token) {
	this->password = utils::sha256(pass);
}

void jobifyCredentials::setPassword(const string &pass) {
	this->password = utils::sha256(pass);
}

const string &jobifyCredentials::getPassword() const {
	return password;
}

Json::Value jobifyCredentials::toJSON() {
	Json::Value value;
	value["email"] = email;
	value["password"] = password;
	value["token"] = token;
	return value;
}

jobifyCredentials::~jobifyCredentials() {
}
