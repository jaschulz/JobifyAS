#ifndef PROFILE_H
#define PROFILE_H

#include "Location.h"
#include "../utils/utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <jsoncpp/json/json.h>

using namespace std;

class Profile{

private:
	string email;
	string first_name;
	string last_name;
	string about;
	Location location;
	string job_position;
	string pic;

	vector<string> skills;
	vector<string> contacts;
	vector<string> invitationsSent;
	vector<string> invitationsReceived;
	vector<string> recommendations;

public:
	Profile(const string &mail);

	Profile(const string &mail,const string &firstName,const string &lastName, const string &p_pic, const string &jposition, const double &latitude, const double &longitude);

	Profile(const Json::Value &jsonProfile);

	void setEmail(const string &mail);

	const string &getEmail() const;

	void setFirstName(const string &firstName);

	const string &getFirstName() const;

	void setLastName(const string &lastName);

	const string &getLastName() const;

	void setPassword(const string &pass);

	const string &getPassword() const;

	Json::Value profileToJSON();

	~Profile();

	void fromJSON(Json::Value value);

	Json::Value &profileGetUserJSON();

	int getRecommendationsCount();
	
	void addSentInvitation(const string &receiver);

	void addReceivedInvitation(const string &sender);

	vector<string> &getSkills();

	vector<string> &getContacts();

	vector<string> &getInvitationsSent();

	vector<string> &getInvitationsReceived();

	vector<string> &getRecommendations();
};


#endif //APPSERVER_USER_H
