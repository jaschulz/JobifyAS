#include "fbCredentials.h"
#include "../utils/utils.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>

fbCredentials::fbCredentials(const string &mail, const string &fbid, const string &token ):Credentials(mail,token) {
	this->fbId = fbid;
}

void fbCredentials::setFbId(const string &fbid) {
	this->fbId = fbid;
}

const string &fbCredentials::getFbId() const {
    return fbId;
}

Json::Value fbCredentials::toJSON() {
    Json::Value value;
    value["email"] = email;
    value["fbid"] = fbId;
    value["token"] = token;
    return value;
}

fbCredentials::~fbCredentials() {
}
