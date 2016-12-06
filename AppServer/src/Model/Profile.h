#ifndef PROFILE_H
#define PROFILE_H

#include "Location.h"
#include "../utils/utils.h"
#include <unistd.h>
#include <set>
#include <stdlib.h>
#include <jsoncpp/json/json.h>
#include "ExpMin.h"

using namespace std;

class Profile {

private:
	string email;
	string first_name;
	string last_name;
	string about;
	Location location;
	string job_position;
	string pic;

	set<string> skills;
	set<string> contacts;
	set<string> invitationsSent;
	set<string> invitationsReceived;
	set<string> recommendations;
	vector<ExpMin> experiences;

public:
	Profile(const string &mail);

	Profile(const string &mail, const string &firstName, const string &lastName,
			string &p_pic, string &jposition, double &latitude,
			double &longitude);

	Profile(const Json::Value &jsonProfile);

	void fromJson(const Json::Value &jsonProfile);

	void setEmail(string &mail);

	const string &getEmail() const;

	void setFirstName(const string &firstName);

	const string &getFirstName() const;

	void setLastName(const string &lastName);

	const string &getLastName() const;

	void setJobPosition(string &jobPosition);

	const string &getJobPosition() const;

	Json::Value profileToJSON();

	Json::Value publicProfileToJSON();

	~Profile();

	void addRecommendation(const string &email);

	Json::Value getContactInfoAsJson();

	Json::Value experiencesToJson();

	Json::Value editableProfileToJSON();

	Location &getLocation();

	int getRecommendationsCount();

	void addSentInvitation(const string &receiver);

	void addReceivedInvitation(const string &sender);

	void addContact(const string &contact);

	set<string> &getSkills();

	set<string> &getContacts();

	set<string> &getInvitationsSent();

	set<string> &getInvitationsReceived();

	set<string> &getRecommendations();
};

#endif //APPSERVER_USER_H
