#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <unistd.h>
#include <stdlib.h>

#include <jsoncpp/json/json.h>
#include <string>

using namespace std;

class Credentials{

private:
	string email;
	string token;
public:
	Credentials(const string &mail, const string &token);

	void setEmail(const string &mail);

	const string &getEmail() const;

	virtual Json::Value toJSON() = 0;

	~Credentials();

};

#endif
