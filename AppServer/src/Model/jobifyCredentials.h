#ifndef JOBIFY_CREDENTIALS_H
#define JOBIFY_CREDENTIALS_H

#include <unistd.h>
#include <stdlib.h>
#include "Credentials.h"
#include <jsoncpp/json/json.h>
#include <string>

using namespace std;

class jobifyCredentials: public Credentials {

private:
	string password;
public:
	jobifyCredentials(const string &mail, const string &pass, const string &token);

	void setPassword(const string &pass);

	const string &getPassword() const;

	~jobifyCredentials();

};

#endif
