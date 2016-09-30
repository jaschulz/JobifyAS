#ifndef PROFILE_H
#define PROFILE_H

#include <unistd.h>
#include <stdlib.h>
#include <json/json.h>

using namespace std;

class Profile{

private:
	string email;
	string password;
	string first_name;
	string last_name;
	Json::Value jsonProfile;

public:
	Profile(const Json::Value &value);

	Profile(const string &mail, const string &pass, const string &firstName, const string &lastName);

	void setEmail(const string &mail);

	const string &getEmail() const;

	void setFirstName(const string &firstName);

	const string &getFirstName() const;

	void setLastName(const string &lastName);

	const string &getLastName() const;

	void setPassword(const string &pass);

	const string &getPassword() const;

	Json::Value &profileToJSON();

	Json::Value &publicProfileToJSON();

	~Profile();

	void fromJSON(Json::Value value);

	Json::Value &profileGetUserJSON();
};


#endif //APPSERVER_USER_H
