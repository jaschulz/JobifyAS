#include "Profile.h"
#include <string>
#include <iostream>
#include "Location.h"

Profile::Profile(const string &mail):location() {
	this->email = mail;
	this->skills.empty();
	this->contacts.empty();
	this->invitationsSent.empty();
	this->invitationsReceived.empty();
	this->recommendations.empty();
}

Profile::Profile(const string &mail,const string &firstName,const string &lastName, const string &p_pic, const string &jposition, const double &latitude, const double &longitude):location(latitude,longitude) {
	this->email = mail;
	this->first_name = firstName;
	this->last_name = lastName;
	this->pic = p_pic;
	this->job_position = jposition;	
	this->skills.empty();
	this->contacts.empty();
	this->invitationsSent.empty();
	this->invitationsReceived.empty();
	this->recommendations.empty();
}



Profile::Profile(const Json::Value &jsonProfile):location(jsonProfile["location"]["latitude"].asDouble(),jsonProfile["location"]["longitude"].asDouble()) {
	this->email = jsonProfile.get("email", "").asString();
	this->first_name = jsonProfile.get("first_name", "").asString();
	this->last_name = jsonProfile.get("last_name", "").asString();
	this->about = jsonProfile.get("last_name", "").asString();
	this->pic = jsonProfile.get("pic", "").asString();
	this->job_position = jsonProfile.get("job_position", "").asString();
	utils::jsonArrayToVector(jsonProfile["skills"],this->skills);
	utils::jsonArrayToVector(jsonProfile["contacts"],this->contacts);
	utils::jsonArrayToVector(jsonProfile["invitationsSent"],this->invitationsSent);
	utils::jsonArrayToVector(jsonProfile["invitationsReceived"],this->invitationsReceived);
	utils::jsonArrayToVector(jsonProfile["recommendations"],this->recommendations);
}

Json::Value Profile::profileToJSON() {
	Json::Value jsonProfile;
	jsonProfile["email"] = email;
	jsonProfile["first_name"] = first_name;
	jsonProfile["last_name"] = last_name;
	jsonProfile["about"] = about;
	jsonProfile["pic"] = pic;
	jsonProfile["job_position"] = job_position;
	jsonProfile["location"]["latitude"] = location.getLatitude();
	jsonProfile["location"]["longitude"] = location.getLongitude();
	jsonProfile["skills"] = Json::arrayValue;
	jsonProfile["contacts"] = Json::arrayValue;
	jsonProfile["invitationsSent"] = Json::arrayValue;
	jsonProfile["invitationsReceived"] = Json::arrayValue;
	jsonProfile["recommendations"]["count"] = getRecommendations();
	jsonProfile["recommendations"]["users"] = Json::arrayValue;
	return jsonProfile;
}

void Profile::setEmail(const string &mail) {
    email = mail;
}

const string &Profile::getEmail() const {
    return email;
}

int Profile::getRecommendations() {
	return recommendations.size();
}

Profile::~Profile() {
}

void Profile::addSentInvitation(string &receiver) {
	invitationsSent.push_back(receiver);
}

void Profile::addReceivedInvitation(string &sender) {
	invitationsReceived.push_back(sender);
}
