#ifndef FB_CREDENTIALS_H
#define FB_CREDENTIALS_H

#include <unistd.h>
#include <stdlib.h>
#include "Credentials.h"
#include <jsoncpp/json/json.h>
#include <string>

using namespace std;

class fbCredentials: public Credentials {

private:
	string fbId;
public:
	fbCredentials(const string &mail, const string &fbId, const string &token);

	void setFbId(const string &pass);

	const string &getFbId() const;

	~fbCredentials();

};

#endif
