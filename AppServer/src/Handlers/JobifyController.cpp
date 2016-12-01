#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include "JobifyController.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include "../Model/Profile.h"

JobifyController::JobifyController() {
	routeParams = new map<string, string>();
}

JobifyController::~JobifyController() {
	delete (routeParams);
}

void JobifyController::expandAttributes(Json::Value &userJson) {
	userJson["job_position"] = expandJP(userJson.get("job_position","").asString());
	userJson["experiences"] = expandExperiences(userJson.get("experiences",Json::arrayValue));
	userJson["skills"] = expandSkills(userJson.get("skills",Json::arrayValue));
	userJson["contacts"] = expandContacts(userJson.get("contacts",Json::arrayValue));
	cout<<"fin expand attr"<<endl;
}

Json::Value JobifyController::expandJP(std::string jpJson){
	Json::Value job_position = "";
	CurlWrapper cw;
	JsonResponse response;
	cw.handleGet("https://still-falls-40635.herokuapp.com/job_positions", "",
				response);
	Json::Value array = response["job_positions"];
	std::map<string, Entity> jpMap = utils::entityJsonArraytoMap(array);
	std::map<string, Entity>::iterator it;
	it = jpMap.find(jpJson);
	if (it != jpMap.end()) {
		Entity e = it->second;
		job_position = e.toJson();
	}
	return job_position;
}

Json::Value JobifyController::expandExperiences(Json::Value expJson){
	Json::Value experiences = Json::arrayValue;
	for (Json::Value::iterator it = expJson.begin(); it != expJson.end(); ++it) {
		Json::Value value = (*it);
		Json::Value exp;
		std::string where = value.get("where", "").asString();
		std::string jp = value["job_position"].asString();
		exp["where"] = where;
		exp["job_position"] = expandJP(jp);
		experiences.append(exp);
	}
	return experiences;
}


Json::Value JobifyController::expandSkills(Json::Value skillsJson){
	Json::Value skills = Json::arrayValue;
	CurlWrapper cw;
	JsonResponse response;
	cw.handleGet("https://still-falls-40635.herokuapp.com/skills", "",
				response);
	Json::Value array = response["skills"];
	std::map<string, Entity> skillsMap = utils::entityJsonArraytoMap(array);
	for (Json::Value::iterator it = skillsJson.begin(); it != skillsJson.end(); ++it) {
		std::string skill = it->asString();
		std::map<string, Entity>::iterator mapIt;
		mapIt = skillsMap.find(skill);
		if (mapIt != skillsMap.end()) {
			Entity e = mapIt->second;
			skills.append(e.toJson());
		}
	}
	cout<<"JobifyController::expandSkills: "<<skills.toStyledString()<<endl;
	return skills;
}

Json::Value JobifyController::expandUsers(Json::Value usersJson) {
	Json::Value users = Json::arrayValue;
	dbUsers db;
	db.connect("./usersdb");
	for (Json::Value::iterator it = usersJson.begin(); it != usersJson.end(); ++it) {
		std::string contact = it->asString();
		Json::Value jsonProfile = db.getProfile(contact);
		std::string error = jsonProfile.get("error", "").asString();
		if (error == "") {
			Profile profile(jsonProfile);
			users.append(profile.publicProfileToJSON());
		}
	}
	db.CloseDB();
	return users;
}


Json::Value JobifyController::expandContacts(Json::Value contactsJson){
	Json::Value contacts = Json::arrayValue;
	dbUsers db;
	db.connect("./usersdb");
	for (Json::Value::iterator it = contactsJson.begin(); it != contactsJson.end(); ++it) {
		std::string contact = it->asString();
		Json::Value jsonProfile = db.getProfile(contact);
		Profile profile(jsonProfile);
		std::string error = jsonProfile.get("error", "").asString();
		if (error == "") {
			contacts.append(profile.getContactInfoAsJson());
		}
	}
	db.CloseDB();
	return contacts;
}


bool JobifyController::handles(string method, string url) {

	bool handle = false;
	string incomingKey = method + ":" + url;

	map<string, RequestHandlerBase *>::iterator it;
	for (it = routes.begin(); it != routes.end(); it++) {
		string replacedKey = replaceRouteParams(it->first);
		if (regex_match(incomingKey, regex(replacedKey))) {
			handle = true;
			break;
		}
	}

	return handle;
}

void JobifyController::fillResponse(JsonResponse &response, int code) {
	response.setCode(code);
	response.setHeader("Content-Type", "application/json; charset=utf-8");
}

Response *JobifyController::process(Request &request) {

	string currentRequest = request.getMethod() + ":" + request.getUrl();
	Response *response = NULL;

	map<string, RequestHandlerBase *>::iterator it;
	for (it = routes.begin(); it != routes.end(); it++) {
		string key = it->first;
		string regexKey = replaceRouteParams(key);
		if (regex_match(currentRequest, regex(regexKey))) {
			if (regexKey.find("\[^/]*") != string::npos) {
				parseRouteParams(key, currentRequest);
			}
			response = it->second->process(request);
			break;
		}
	}
	return response;
}

void JobifyController::parseRouteParams(const string &key,
		const string &currentRequest) const {
	routeParams->clear();
	//Get map key
	unsigned long firstPos = key.find("{");
	unsigned long secondPos = key.find("}");

	string mapKey = key.substr(firstPos + 1, secondPos - firstPos - 1);
	//Get map value
	string requestTail = currentRequest.substr(firstPos);
	unsigned long incomingRequestValueEnd = requestTail.find("/");
	string value = currentRequest.substr(firstPos, incomingRequestValueEnd);
	routeParams->insert(std::pair<string, string>(mapKey, value));
}

string JobifyController::replaceRouteParams(string key) const {

	string replacedKey = key;
	unsigned long firstPos = replacedKey.find("{");
	unsigned long secondPos = replacedKey.find("}");

	while (firstPos != string::npos && secondPos != string::npos) {
		replacedKey = replacedKey.replace(firstPos, secondPos - firstPos + 1,
				"\[^/]*");
		firstPos = replacedKey.find("{");
		secondPos = replacedKey.find("}");
	}
	return replacedKey;
}

string JobifyController::requestToJson(Request &request, Json::Value & root) {
	Json::Reader reader;

	std::string data = request.getData();

	string error = "";

	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		error = "Failed to parse" + reader.getFormattedErrorMessages();
	}
	return error;
}

bool JobifyController::isValidToken(string token, string & error) {
	dbToken dbTkn;
	dbTkn.connect("./tokens");
	string loggedUser = dbTkn.getUser(token, error);
	dbTkn.CloseDB();
	if (!error.empty()) {
		error = "Invalid token: " + error;
		return false;
	}
	dbCredentials credentials;
	credentials.connect("./accounts");
	if (!credentials.isValidToken(loggedUser, token)) {
		credentials.CloseDB();
		error = "Invalid token";
		return false;
	}
	credentials.CloseDB();
	return true;
}

bool JobifyController::isValidTokenForUser(string token, string & error, string &email) {
	dbToken dbTkn;
	dbTkn.connect("./tokens");
	string loggedUser = dbTkn.getUser(token, error);
	dbTkn.CloseDB();
	if (!error.empty()) {
		error = "Invalid token: " + error;
		return false;
	}
	if (email.compare(loggedUser) != 0) {
		error = "Invalid token";
		return false;
	}
	dbCredentials credentials;
	credentials.connect("./accounts");
	if (!credentials.isValidToken(loggedUser, token)) {
		credentials.CloseDB();
		error = "Invalid token";
		return false;
	}
	credentials.CloseDB();
	return true;
}
