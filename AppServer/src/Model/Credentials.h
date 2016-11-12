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
	string password;
	string token;
public:
	Credentials(const string &mail, const string &pass, const string &token);

	void setEmail(const string &mail);

	const string &getEmail() const;

	void setPassword(const string &pass);

	const string &getPassword() const;

	Json::Value toJSON();

	~Credentials();

};

#endif
