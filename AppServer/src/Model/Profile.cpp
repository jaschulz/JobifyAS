#include "Profile.h"
#include <string>
#include <iostream>
#include "Location.h"

Profile::Profile(const string &mail):location() {
	this->email = mail;
}

Profile::Profile(const string &mail,const string &firstName,const string &lastName,  string &p_pic,  string &jposition, double &latitude, double &longitude):location(latitude,longitude) {
	this->email = mail;
	this->first_name = firstName;
	this->last_name = lastName;
	this->pic = p_pic;
	this->job_position = jposition;	
}



Profile::Profile(const Json::Value &jsonProfile):location() {
	this->email = jsonProfile.get("email", "").asString();
	this->first_name = jsonProfile.get("first_name", "").asString();
	this->last_name = jsonProfile.get("last_name", "").asString();
	this->about = jsonProfile.get("last_name", "").asString();
	this->pic = jsonProfile.get("pic", "").asString();
	this->job_position = jsonProfile.get("job_position", "").asString();
	utils::jsonArrayToSet(jsonProfile["skills"],skills);
	utils::jsonArrayToSet(jsonProfile["contacts"],contacts);
	utils::jsonArrayToSet(jsonProfile["invitationsSent"], invitationsSent);
	utils::jsonArrayToSet(jsonProfile["invitationsReceived"], invitationsReceived);
	utils::jsonArrayToSet(jsonProfile["recommendations"]["users"], recommendations);
	double lat = jsonProfile["location"]["latitude"].asDouble();
	double lon = jsonProfile["location"]["longitude"].asDouble();
	location.setLatitude(lat);
	location.setLongitude(lon);
	cout<<"PERFIL CREADO CORRECTAMENTE"<<endl;
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
	jsonProfile["skills"] = utils::setToJsonArray(skills);
	jsonProfile["contacts"] = utils::setToJsonArray(contacts);
	jsonProfile["invitationsSent"] = utils::setToJsonArray(invitationsSent);
	jsonProfile["invitationsReceived"] = utils::setToJsonArray(invitationsReceived);
	jsonProfile["recommendations"]["count"] = getRecommendationsCount();
	jsonProfile["recommendations"]["users"] = utils::setToJsonArray(recommendations);
	return jsonProfile;
}

void Profile::setEmail( string &mail) {
    email = mail;
}

const string &Profile::getEmail() const {
    return email;
}

int Profile::getRecommendationsCount() {
	return recommendations.size();
}

Profile::~Profile() {
}

void Profile::addRecommendation(const string &email){
	cout<<"antes:"<<recommendations.size()<<endl;
	recommendations.insert(email);
	cout<<"despues:"<<recommendations.size()<<endl;
}

void Profile::addSentInvitation(const string &receiver) {
	invitationsSent.insert(receiver);
}

void Profile::addReceivedInvitation(const string &sender) {
	invitationsReceived.insert(sender);
}

	set<string> &Profile::getSkills(){
		return skills;
	}

	set<string> &Profile::getContacts(){
		return contacts;
	}

	set<string> &Profile::getInvitationsSent(){
		return invitationsSent;
	}

	set<string> &Profile::getInvitationsReceived(){
		return invitationsReceived;
	}

	set<string> &Profile::getRecommendations(){
		return recommendations;
	}
