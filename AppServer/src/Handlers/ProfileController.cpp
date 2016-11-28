#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <algorithm> 
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include "ProfileController.h"
#include "SSController.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include "../Model/Profile.h"

ProfileController::ProfileController() {

}
/*
 void ProfileController::printDB(Request &request, JsonResponse &response) {
 dbCredentials dbCont;
 dbCont.connect("./usersdb");
 string error = dbCont.printDB();
 dbCont.CloseDB();

 if (error == "") {
 response.setCode(200);
 response.setHeader("Content-Type", "application/json; charset=utf-8");
 response["print"] = "Ok";
 } else {		
 response.setCode(401);
 response.setHeader("Content-Type", "application/json; charset=utf-8");
 response["error"] = error;
 }
 }
 */
/*
 bool ProfileController::validateEntity(std::map<string, Entity> entitiesMap,
 std::map<string, Category> CategoriesMap, Entity entity,
 std::string &error) {
 std::string cat = entity.getCategory();
 SSController sharedServer;
 if (CategoriesMap.find(cat) == CategoriesMap.end()) {
 if (entitiesMap.find(entity.getName()) != entitiesMap.end()) {
 error = " already exists for another category.";
 return false;
 } else {

 }
 }
 }
 */

bool ProfileController::validateCategory(Entity entity,
		std::map<string, Category> CategoriesMap) {
	std::string cat = entity.getCategory();
	SSController sharedServer;
	if (CategoriesMap.find(cat) == CategoriesMap.end()) {
		return false;
	}
	return true;
}

bool ProfileController::validateSkills(std::map<string, Category> CategoriesMap,
		Json::Value skillsJson, std::string &error) {
	SSController sharedServer;
	std::map<string, Entity> skillsMap = sharedServer.getSkillsMap();

	for (Json::Value::iterator it = skillsJson.begin(); it != skillsJson.end();
			++it) {
		Json::Value value = (*it);
		std::string name = value.get("name", "").asString();
		std::string cat = value.get("category", "").asString();
		std::string desc = value.get("description", "").asString();
		JsonResponse response;
		Entity skill(name, desc, cat);
		if (validateCategory(skill, CategoriesMap)) {
			if (skillsMap.find(skill.getName()) != skillsMap.end()) {
				cout<<"está en los 2"<<endl;
				return true;
			}
		} else {
			if (skillsMap.find(skill.getName()) != skillsMap.end()) {
				error = "Skill already exists for another category.";
				cout<<error<<endl;
				return false;
			}
			Category category(cat, "");
			std::string catJsonStr = category.toJson().asString();
			//request.setData(catJsonStr);
			//Category is created

			cout<<"Category is created"<<endl;
			sharedServer.addCategory(catJsonStr, response);
		}
		std::string skillJsonStr = skill.toJson().asString();
		//request.setData(skillJsonStr);
		cout<<"Skill is created"<<endl;
		sharedServer.addSkills(skillJsonStr,response);
		//TODO ver el código de la response
		return true;

	}
}

bool ProfileController::validateJP(std::map<string, Category> CategoriesMap,
		Json::Value jpJson, std::string &error) {
	SSController sharedServer;
	std::map<string, Entity> jpMap = sharedServer.getJPMap();
	for (Json::Value::iterator it = jpJson.begin(); it != jpJson.end(); ++it) {
		Json::Value value = (*it);
		std::string name = value.get("name", "").asString();
		std::string cat = value.get("category", "").asString();
		std::string desc = value.get("description", "").asString();
		Request request(NULL);
		JsonResponse response;
		Entity jobPosition(name, desc, cat);
		if (validateCategory(jobPosition, CategoriesMap)) {
			if (jpMap.find(jobPosition.getName()) != jpMap.end()) {
				return true;
			}
		} else {
			if (jpMap.find(jobPosition.getName()) != jpMap.end()) {
				error = "Job Position already exists for another category.";
				return false;
			}
			Category category(cat, "");
			std::string catJsonStr = category.toJson().asString();
			//Category is created
			request.setData(catJsonStr);
			sharedServer.addCategory(request, response);
		}
		std::string jpJsonStr = jobPosition.toJson().asString();
		//Skill is created
		request.setData(jpJsonStr);
		sharedServer.addJobPositions(request, response);
		//TODO ver el código de la response
		return true;

	}
}

bool ProfileController::validateInput(Json::Value Body, std::string &error) {
	Json::Value normalizedBody;
	SSController sharedServer;
	cout<<"antes de CategoriesMap"<<endl;
	std::map<string, Category> CategoriesMap = sharedServer.getCategoriesMap();

	cout<<"antes de skills"<<Body["skills"].toStyledString()<<endl;
	if (!Body["skills"].isNull()) {
		Json::Value skills = Body["skills"];
		if (!validateSkills(CategoriesMap, skills, error)) {
			return false;
		}
	}
/*
	cout<<"antes de experiences"<<endl;
	if (!Body["experiences"].isNull()) {
		Json::Value job_positions = Body["job_positions"];
		if (!validateJP(CategoriesMap, job_positions, error)) {
			return false;
		}
	}*/
	return true;
}

void normalizeJsonProfile(Json::Value &jsonProfile) {
	Json::Value skills = jsonProfile["skills"];
	jsonProfile["skills"] = utils::reduceJsonArrayToIds(skills, "name");;
}

void ProfileController::editProfile(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
//TODO verify token
	string error = "Wrong number or type of parameters.";
	if (1 == sscanf(request.getUrl().c_str(), "/api/users/%s", email)) {
		error = "";
		Json::Reader reader;
		Json::Value JsonBody;
		if (reader.parse(data.c_str(), JsonBody)) {
			//SSController sharedServer;
			if (validateInput(JsonBody, error)) {
				dbUsers dbuser;
				dbuser.connect("./usersdb");
				cout<<"antes de normalizeJsonProfile"<<endl;
				normalizeJsonProfile(JsonBody);
				Profile profile(JsonBody);
				//string key(email);
				error = dbuser.editProfile(profile); //key, JsonBody);
				dbuser.CloseDB();
				if (error == "") {
					fillResponse(response, 200);
					response["user"] = JsonBody;
					return;
				}
			} else {
				error = "Incorrect input: " + error;
			}
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response, 401);
	response["error"] = error;
}

void ProfileController::getProfile(Request &request, JsonResponse &response) {
	char email[50];
	string error = "Wrong number or type of parameters.";
	if (1 == sscanf(request.getUrl().c_str(), "/api/users/%99[^/]", email)) {
		string mail(email);
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		Json::Value JsonBody = dbuser.getProfile(mail);
		error = JsonBody.get("error", "").asString();
		dbuser.CloseDB();
		if (error == "") {
			fillResponse(response, 200);
			response["user"] = JsonBody;
			return;
		}
	}
	fillResponse(response, 401);
	response["error"] = error;
}

void ProfileController::getContacts(Request &request, JsonResponse &response) {
	char email[50];
	string error = "Wrong number or type of parameters.";
	if (1
			== sscanf(request.getUrl().c_str(), "/api/users/%99[^/]/contacts",
					email)) {
		string mail(email);
		Json::Value JsonBody;
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		JsonBody = dbuser.getContacts(mail);
		error = JsonBody.get("error", "").asString();
		dbuser.CloseDB();
		if (error == "") {
			fillResponse(response, 200);
			response["contacts"] = JsonBody["contacts"];
			return;
		}
	}
	fillResponse(response, 401);
	response["error"] = error;
}

void ProfileController::setLocation(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
	if (1
			== sscanf(request.getUrl().c_str(), "/api/users/%99[^/]/location",
					email)) {
		string mail(email);
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			double latitude = JsonBody["location"]["latitude"].asDouble();
			double longitude = JsonBody["location"]["longitude"].asDouble();
			Location location(latitude, longitude);
			response["user"] = dbuser.setLocation(mail, location);
			dbuser.CloseDB();
			if (error == "") {
				fillResponse(response, 200);
				response["user"] = JsonBody;
				return;
			}
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response, 401);
	response["error"] = "Wrong number or type of parameters.";
}

void ProfileController::addContact(Request &request, JsonResponse &response) {
	char email[50];
	int code = 401;
	string error = "Wrong number or type of parameters.";
	string data = request.getData();
	if (1
			== sscanf(request.getUrl().c_str(), "/api/users/%99[^/]/contacts",
					email)) {
		string mail(email);
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {
			int i = 0;
			string newContact = JsonBody.get("email", "").asString();
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			Json::Value senderJson = dbuser.getProfile(mail);
			Json::Value receiverJson = dbuser.getProfile(newContact);
			if (senderJson["error"].isNull()
					&& receiverJson["error"].isNull()) {
				Profile sender(senderJson);
				Profile receiver(receiverJson);
				if (dbuser.sendInvitation(sender, receiver, error, code)) {
					dbuser.CloseDB();
					fillResponse(response, code);
					response["user"] = JsonBody;
					return;
				}
			}
			error = senderJson["error"].asString() + " - "
					+ receiverJson["error"].asString();
			dbuser.CloseDB();
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response, code);
	response["error"] = error;
}

void ProfileController::recommendUser(Request &request,
		JsonResponse &response) {
	char email[50];
	int code = 401;
	string error = "Wrong number or type of parameters.";
	string data = request.getData();
	if (1
			== sscanf(request.getUrl().c_str(),
					"/api/users/%99[^/]/recommendation", email)) {
		string mail(email);
		cout << "mail:" << mail << endl;
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {
			int i = 0;
			string recommendedUser = JsonBody.get("email", "").asString();
			cout << "recommendedUser:" << recommendedUser << endl;
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			Json::Value senderJson = dbuser.getProfile(mail);
			Json::Value receiverJson = dbuser.getProfile(recommendedUser);
			if (senderJson["error"].isNull()
					&& receiverJson["error"].isNull()) {
				cout << "senderJson: " << senderJson.toStyledString();
				Profile sender(senderJson);
				Profile receiver(receiverJson);
				dbuser.recommendUser(sender, receiver);
				dbuser.CloseDB();
				fillResponse(response, 201);
				response["user"] = JsonBody;
				return;
			}
			error = senderJson["error"].asString() + " - "
					+ receiverJson["error"].asString();
			dbuser.CloseDB();
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response, code);
	response["error"] = error;
}

void ProfileController::acceptInvitation(Request &request,
		JsonResponse &response) {
	char email[50];
	int code = 401;
	string error = "Wrong number or type of parameters.";
	string data = request.getData();
	if (1
			== sscanf(request.getUrl().c_str(), "/api/users/%99[^/]/invitation",
					email)) {
		string mail(email);
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {
			int i = 0;
			string newContact = JsonBody.get("email", "").asString();
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			Json::Value receiverJson = dbuser.getProfile(mail);
			Json::Value senderJson = dbuser.getProfile(newContact);
			if (senderJson["error"].isNull()
					&& receiverJson["error"].isNull()) {
				Profile sender(senderJson);
				Profile receiver(receiverJson);
				if (dbuser.acceptInvitation(sender, receiver, error, code)) {
					dbuser.CloseDB();
					fillResponse(response, code);
					response["user"] = JsonBody;
					return;
				}
			}
			error = senderJson["error"].asString() + " - "
					+ receiverJson["error"].asString();
			dbuser.CloseDB();
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response, code);
	response["error"] = error;
}

void ProfileController::filterUsers(Request &request, JsonResponse &response) {
	string job_pos = request.get("job_position", "");
	string skills = request.get("skills", "");
	string range = request.get("range", "");
	string user = request.get("user", "");

	Json::Value filter;
	if (!job_pos.empty()) {
		filter["job_pos"] = job_pos;
	}
	if (!skills.empty()) {
		filter["skills"] = skills;
	}
	if (!range.empty()) {
		filter["range"] = atof(range.c_str());
		//If range is not part of the queryString, locations are not important
		double latitude = atof(request.get("latitude", "0").c_str());
		double longitude = atof(request.get("longitude", "0").c_str());
		filter["Location"]["latitude"] = latitude;
		filter["Location"]["longitude"] = longitude;
	}
	if (!user.empty()) {
		transform(user.begin(), user.end(), user.begin(), ::toupper);
		filter["user"] = user;
	}
	cout << "filter: " << filter.toStyledString() << endl;
	dbUsers dbuser;
	dbuser.connect("./usersdb");
	string error;
	Json::Value users = dbuser.searchProfile(filter, error);
	dbuser.CloseDB();
	if (error == "") {
		response["users"] = users;
		return;
	}
	fillResponse(response, 401);
	response["error"] = error;

}

bool compare_recommendations(Profile p1, Profile p2) {
	return p1.getRecommendationsCount() > p2.getRecommendationsCount();
}

void ProfileController::rankUsers(Request &request, JsonResponse &response) {
	string job_pos = request.get("job_position", "");
	string skill = request.get("skill", "");
	cout << "skill: " << skill << endl;
	cout << "job_pos: " << job_pos << endl;
	Json::Value filter;
	if (!job_pos.empty()) {
		filter["job_position"] = job_pos;
	}
	if (!skill.empty()) {
		filter["skill"] = skill;
	}
	dbUsers dbuser;
	dbuser.connect("./usersdb");
	string error;
	Json::Value ranking;
	std::list<Profile> users = dbuser.getMostPopularUsers(filter, error);
	dbuser.CloseDB();
	if (error == "") {
		users.sort(compare_recommendations);
		std::list<Profile>::iterator it;
		for (it = users.begin(); it != users.end(); ++it) {
			ranking.append(it->profileToJSON());
		}
		response["users"] = ranking;
		return;
	}
	fillResponse(response, 401);
	response["error"] = error;

}

void ProfileController::setup() {
	setPrefix("/api");
	addRouteResponse("GET", "/users/{email}", ProfileController, getProfile,
			JsonResponse);
	addRouteResponse("PUT", "/users/{email}", ProfileController, editProfile,
			JsonResponse);
	addRouteResponse("POST", "/users/{email}/contacts", ProfileController,
			addContact, JsonResponse);
	addRouteResponse("POST", "/users/{email}/invitation", ProfileController,
			acceptInvitation, JsonResponse);
	addRouteResponse("GET", "/users/{email}/contacts", ProfileController,
			getContacts, JsonResponse);
	addRouteResponse("POST", "/users/{email}/location", ProfileController,
			setLocation, JsonResponse);
	addRouteResponse("POST", "/users/{email}/recommendation", ProfileController,
			recommendUser, JsonResponse);
	addRouteResponse("GET", "/users", ProfileController, filterUsers,
			JsonResponse);
	addRouteResponse("GET", "/ranking", ProfileController, rankUsers,
			JsonResponse);
//addRouteResponse("GET", "/printProfiles", ProfileController, printDB,JsonResponse);
}

