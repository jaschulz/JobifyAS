#include "Profile.h"
#include <string>
#include <cmath>
#include <iostream>
#include "Location.h"

Profile::Profile(const string &mail) :
		location() {
	this->email = mail;
}

Profile::Profile(const string &mail, const string &firstName,
		const string &lastName, string &p_pic, string &jposition,
		double &latitude, double &longitude) :
		location(latitude, longitude) {
	this->email = mail;
	this->first_name = firstName;
	this->last_name = lastName;
	this->pic = p_pic;
	this->job_position = jposition;
}

Profile::Profile(const Json::Value &jsonProfile) :
		location() {
	this->email = jsonProfile.get("email", "").asString();
	this->first_name = jsonProfile.get("first_name", "").asString();
	this->last_name = jsonProfile.get("last_name", "").asString();
	this->about = jsonProfile.get("about", "").asString();
	this->pic = jsonProfile.get("pic", "").asString();
	this->job_position = jsonProfile.get("job_position", "").asString();
	utils::jsonArrayToSet(jsonProfile["skills"], skills);
	utils::jsonArrayToSet(jsonProfile["contacts"], contacts);
	utils::jsonArrayToSet(jsonProfile["invitationsSent"], invitationsSent);
	utils::jsonArrayToSet(jsonProfile["invitationsReceived"],
			invitationsReceived);
	utils::jsonArrayToSet(jsonProfile["recommendations"]["users"],
			recommendations);
	if (!jsonProfile["location"]["latitude"].isNull()
			&& !jsonProfile["location"]["longitude"].isNull()) {
		double lat = jsonProfile["location"]["latitude"].asDouble();
		double lon = jsonProfile["location"]["longitude"].asDouble();
		location.setLatitude(lat);
		location.setLongitude(lon);
		location.setValid(true);
	}

	for (Json::Value::iterator it = jsonProfile["experiences"].begin();
			it != jsonProfile["experiences"].end(); ++it) {
		Json::Value value = (*it);
		std::string where = value.get("where", "").asString();
		std::string jp = value["job_position"].asString();
		ExpMin exp(jp, where);
		experiences.push_back(exp);
	}
}

Json::Value Profile::profileToJSON() {
	Json::Value jsonProfile;
	jsonProfile["email"] = email;
	jsonProfile["first_name"] = first_name;
	jsonProfile["last_name"] = last_name;
	jsonProfile["about"] = about;
	jsonProfile["pic"] = pic;
	jsonProfile["job_position"] = job_position;
	//double latitude = location.getLatitude();
	//double longitude = location.getLongitude();
	if (location.isValid()) {
		jsonProfile["location"]["latitude"] = location.getLatitude();
		jsonProfile["location"]["longitude"] = location.getLongitude();
	}
	jsonProfile["skills"] = utils::setToJsonArray(skills);
	jsonProfile["contacts"] = utils::setToJsonArray(contacts);
	jsonProfile["invitationsSent"] = utils::setToJsonArray(invitationsSent);
	jsonProfile["invitationsReceived"] = utils::setToJsonArray(
			invitationsReceived);
	jsonProfile["recommendations"]["count"] = getRecommendationsCount();
	jsonProfile["recommendations"]["users"] = utils::setToJsonArray(
			recommendations);
	jsonProfile["experiences"] = experiencesToJson();
	return jsonProfile;
}

Json::Value Profile::publicProfileToJSON() {
	Json::Value jsonProfile;
	jsonProfile["email"] = email;
	jsonProfile["first_name"] = first_name;
	jsonProfile["last_name"] = last_name;
	jsonProfile["about"] = about;
	jsonProfile["pic"] = pic;
	cout<<"antes de skills"<<endl;
	jsonProfile["skills"] = utils::setToJsonArray(skills);
	jsonProfile["job_position"] = job_position;
	//double latitude = location.getLatitude();
	//double longitude = location.getLongitude();
	cout<<"antes de loc"<<endl;
	if (location.isValid()) {
		jsonProfile["location"]["latitude"] = location.getLatitude();
		jsonProfile["location"]["longitude"] = location.getLongitude();
	}
	cout<<"despues de location"<<endl;
	jsonProfile["contacts"] = utils::setToJsonArray(contacts);
	jsonProfile["recommendations"]["count"] = getRecommendationsCount();
	jsonProfile["recommendations"]["users"] = utils::setToJsonArray(
			recommendations);
	jsonProfile["experiences"] = experiencesToJson();
	cout<<"fin ACA"<<endl;
	return jsonProfile;
}

Json::Value Profile::getContactInfoAsJson() {
	Json::Value jsonProfile;
	jsonProfile["email"] = email;
	jsonProfile["first_name"] = first_name;
	jsonProfile["last_name"] = last_name;
	jsonProfile["pic"] = pic;
	jsonProfile["job_position"] = job_position;
	return jsonProfile;
}

Json::Value Profile::experiencesToJson() {
	Json::Value exp = Json::arrayValue;
	for (unsigned int i = 0; i < experiences.size(); i++) {
		ExpMin expMin = experiences[i];
		exp.append(expMin.toJson());
	}
	return exp;
}

Json::Value Profile::editableProfileToJSON() {
	Json::Value jsonProfile;
	jsonProfile["email"] = email;
	jsonProfile["first_name"] = first_name;
	jsonProfile["last_name"] = last_name;
	jsonProfile["about"] = about;
	jsonProfile["pic"] = pic;
	jsonProfile["skills"] = utils::setToJsonArray(skills);
	jsonProfile["job_position"] = job_position;
	if (location.isValid()) {
		jsonProfile["location"]["latitude"] = location.getLatitude();
		jsonProfile["location"]["longitude"] = location.getLongitude();
	}
	jsonProfile["experiences"] = experiencesToJson();
	return jsonProfile;
}

Location &Profile::getLocation() {
	return location;
}

void Profile::setEmail(string &mail) {
	email = mail;
}

const string &Profile::getEmail() const {
	return email;
}

const string &Profile::getLastName() const {
	return last_name;
}
void Profile::setJobPosition(string &jobPosition) {
	job_position = jobPosition;
}

const string &Profile::getJobPosition() const {
	return job_position;
}

int Profile::getRecommendationsCount() {
	return recommendations.size();
}

Profile::~Profile() {
}

void Profile::addRecommendation(const string &email) {
	recommendations.insert(email);
}

void Profile::addSentInvitation(const string &receiver) {
	invitationsSent.insert(receiver);
}

void Profile::addReceivedInvitation(const string &sender) {
	invitationsReceived.insert(sender);
}

set<string> &Profile::getSkills() {
	return skills;
}

set<string> &Profile::getContacts() {
	return contacts;
}

set<string> &Profile::getInvitationsSent() {
	return invitationsSent;
}

set<string> &Profile::getInvitationsReceived() {
	return invitationsReceived;
}

set<string> &Profile::getRecommendations() {
	return recommendations;
}
