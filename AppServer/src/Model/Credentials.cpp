#include "Credentials.h"
#include "../utils/utils.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>

Credentials::Credentials(const string &mail, const string &token ) {
	this->email = mail;
	this->token = token;
}

void Credentials::setEmail(const string &mail) {
    email = mail;
}

const string &Credentials::getEmail() const {
    return email;
}


void Credentials::setToken(const string &token) {
    this->token = token;
}

const string &Credentials::getToken() const {
    return token;
}

Credentials::~Credentials() {
}
