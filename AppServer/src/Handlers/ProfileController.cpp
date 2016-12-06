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
			std::map<string, Entity>::iterator it;
			it = skillsMap.find(skill.getName());
			if (it != skillsMap.end()) {
				Entity e = it->second;
				if (e == skill) {
					//cout << "break" << endl;
					break;
				} else {
					error = "Skill " + skill.getName()
							+ " already exists for category " + e.getCategory()
							+ ".";
					return false;
				}
			}
		} else {
			if (skillsMap.find(skill.getName()) != skillsMap.end()) {
				error = "Skill " + skill.getName()
						+ " already exists for another category.";
				cout << error << endl;
				return false;
			}
			Category category(cat, "");
			Json::Value catJsonStr = category.toJson();
			//request.setData(catJsonStr);
			//Category is created

			cout << "Category is created" << endl;
			sharedServer.addCategory(catJsonStr, response);
		}
		Json::Value skillJson = skill.toJson();
		//request.setData(skillJsonStr);
		cout << "Skill is created" << skillJson.toStyledString() << endl;
		sharedServer.addSkills(skillJson, response);
		//TODO ver el código de la response
	}
	return true;
}

bool ProfileController::validateExperience(
		std::map<string, Category> CategoriesMap, Json::Value expJson,
		std::string &error) {
	for (Json::Value::iterator it = expJson.begin(); it != expJson.end();
			++it) {
		Json::Value value = (*it);
		//reducedArray.append(value[id]);
		if (!validateJP(CategoriesMap, value["job_position"], error)) {
			return false;
		}
	}
	return true;
}

bool ProfileController::validateJP(std::map<string, Category> CategoriesMap,
		Json::Value jpJson, std::string &error) {
	SSController sharedServer;
	std::map<string, Entity> jpMap = sharedServer.getJPMap();
	std::string name = jpJson.get("name", "").asString();
	std::string cat = jpJson.get("category", "").asString();
	std::string desc = jpJson.get("description", "").asString();
	cout << "name: " << name << endl;
	cout << "category: " << cat << endl;
	cout << "description: " << desc << endl;
	JsonResponse response;
	Entity jobPosition(name, desc, cat);
	if (validateCategory(jobPosition, CategoriesMap)) {
		std::map<string, Entity>::iterator it;
		it = jpMap.find(jobPosition.getName());
		if (it != jpMap.end()) {
			Entity e = it->second;
			if (e == jobPosition) {
				return true;
			} else {
				error = "Job Position " + jobPosition.getName()
						+ " already exists for category " + e.getCategory()
						+ ".";
				return false;
			}
		}
	} else {
		if (jpMap.find(jobPosition.getName()) != jpMap.end()) {
			error = "Job Position already exists for another category.";
			return false;
		}
		Category category(cat, "");
		Json::Value catJson = category.toJson();
		//Category is created
		sharedServer.addCategory(catJson, response);
	}
	Json::Value jobPosJson = jobPosition.toJson();
	//Skill is created
	sharedServer.addJobPositions(jobPosJson, response);
	//TODO ver el código de la response
	return true;
}

bool ProfileController::validateInput(Json::Value Body, std::string &error) {
	SSController sharedServer;
	std::map<string, Category> CategoriesMap = sharedServer.getCategoriesMap();
	if (!Body["skills"].isNull()) {
		Json::Value skills = Body["skills"];
		if (!validateSkills(CategoriesMap, skills, error)) {
			return false;
		}
	}
	if (!Body["job_position"].isNull()) {
		Json::Value job_position = Body["job_position"];
		if (!validateJP(CategoriesMap, job_position, error)) {
			return false;
		}
	}
	if (!Body["experiences"].isNull()) {
		Json::Value experiences = Body["experiences"];
		if (!validateExperience(CategoriesMap, experiences, error)) {
			return false;
		}
	}
	return true;
}

void ProfileController::normalizeJsonProfile(Json::Value &jsonProfile) {
	Json::Value skills = jsonProfile["skills"];
	jsonProfile["skills"] = utils::reduceJsonArrayToIds(skills, "name");
	jsonProfile["job_position"] =
			jsonProfile["job_position"].get("name", "").asString();
	Json::Value experiences = Json::arrayValue;
	for (Json::Value::iterator it = jsonProfile["experiences"].begin();
			it != jsonProfile["experiences"].end(); ++it) {
		Json::Value value = (*it);
		//reducedArray.append(value[id]);
		std::string jpName = value["job_position"].get("name", "").asString();
		std::string where = value["where"].asString();
		ExpMin objExpMin(jpName, where);
		experiences.append(objExpMin.toJson());
	}
	jsonProfile["experiences"] = experiences;
}

void ProfileController::editProfile(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();

	string error;
	std::string token = request.getHeaderKeyValue("token");
	error = "Wrong number or type of parameters.";
	if (1 == sscanf(request.getUrl().c_str(), "/api/users/%s", email)) {
		string mail(email);
		error = "";
		if (!isValidTokenForUser(token, error, mail)) {
			fillResponse(response,401);
			response["error"] = error;
			return;
		}
		error = "";
		Json::Reader reader;
		Json::Value JsonBody;
		if (reader.parse(data.c_str(), JsonBody)) {
			//SSController sharedServer;
			if (validateInput(JsonBody, error)) {
				dbUsers dbuser;
				dbuser.connect("./usersdb");
				cout << "antes de normalizeJsonProfile" << endl;
				normalizeJsonProfile(JsonBody);

				cout << "despues de normalizeJsonProfile" << endl;
				Profile profile(JsonBody);
				profile.setEmail(mail);
				error = dbuser.editProfile(profile); //key, JsonBody);
				dbuser.CloseDB();
				if (error == "") {
					fillResponse(response, 200);
					Json::Value userJson = profile.publicProfileToJSON();
					expandAttributes(userJson);
					response["user"] = userJson;
					return;
				}
				error = "Error editing profile: " + error;
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
	string error;

	string token = request.getHeaderKeyValue("token");
	cout << "token: " << token << endl;
	if (!isValidToken(token, error)) {
		fillResponse(response,401);
		response["error"] = "ProfileController::getProfile: " + error;
		return;
	}
	error = "Wrong number or type of parameters.";
	if (1 == sscanf(request.getUrl().c_str(), "/api/users/%99[^/]", email)) {
		string mail(email);
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		Json::Value jsonProfile = dbuser.getProfile(mail);
		Profile profile(jsonProfile);
		error = jsonProfile.get("error", "").asString();
		dbuser.CloseDB();
		if (error == "") {
			fillResponse(response, 200);
			Json::Value userJson = profile.publicProfileToJSON();
			expandAttributes(userJson);
			response["user"] = userJson;
			return;
		}
	}
	fillResponse(response, 401);
	response["error"] = error;
}

void ProfileController::getContacts(Request &request, JsonResponse &response) {
	char email[50];

	string error;

	error = "Wrong number or type of parameters.";
	if (1
			== sscanf(request.getUrl().c_str(), "/api/users/%99[^/]/contacts",
					email)) {
		string mail(email);
		string token = request.getHeaderKeyValue("token");
		error = "";
		if (!isValidToken(token, error)) {
			fillResponse(response,401);
			response["error"] = error;
			return;
		}
		Json::Value JsonBody;
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		JsonBody = dbuser.getContacts(mail);
		error = JsonBody.get("error", "").asString();
		dbuser.CloseDB();
		if (error == "") {
			fillResponse(response, 200);
			Json::Value contacts = expandContacts(JsonBody["contacts"]);
			response["contacts"] = contacts;
			return;
		}
	}
	fillResponse(response, 401);
	response["error"] = error;
}

void ProfileController::setLocation(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
	string error;
	if (1
			== sscanf(request.getUrl().c_str(), "/api/users/%99[^/]/location",
					email)) {
		string mail(email);

		string token = request.getHeaderKeyValue("token");
		if (!isValidTokenForUser(token, error, mail)) {
			fillResponse(response,401);
			response["error"] = error;
			return;
		}
		Json::Value JsonBody;
		Json::Reader reader;
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
	string error;
	string data = request.getData();
	if (1
			== sscanf(request.getUrl().c_str(), "/api/users/%99[^/]/contacts",
					email)) {
		string mail(email);
		string token = request.getHeaderKeyValue("token");
		error = "";
		if (!isValidTokenForUser(token, error, mail)) {
			fillResponse(response,401);
			response["error"] = error;
			return;
		}
		Json::Value JsonBody;
		Json::Reader reader;
		error = "";
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
			} else {
				error = senderJson["error"].asString() + " - "
						+ receiverJson["error"].asString();
				code = 404;
			}
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
	string error;

	string token = request.getHeaderKeyValue("token");
	if (!isValidToken(token, error)) {
		fillResponse(response,code);
		response["error"] = error;
		return;
	}

	error = "Wrong number or type of parameters.";
	string data = request.getData();
	if (1
			== sscanf(request.getUrl().c_str(),
					"/api/users/%99[^/]/recommendation", email)) {
		error ="";
		string recommendedUser(email);
		cout << "recommendedUser:" << recommendedUser << endl;
		dbToken dbTkn;
		dbTkn.connect("./tokens");
		string loggedUser = dbTkn.getUser(token, error);
		dbTkn.CloseDB();
		if (!error.empty()) {
			fillResponse(response, code);
			response["error"] = "Problems retrieving user: " + error;
			return;
		}
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		Json::Value senderJson = dbuser.getProfile(loggedUser);
		Json::Value receiverJson = dbuser.getProfile(recommendedUser);
		if (senderJson["error"].isNull() && receiverJson["error"].isNull()) {
			Profile sender(senderJson);
			Profile receiver(receiverJson);
			dbuser.recommendUser(sender, receiver);
			dbuser.CloseDB();
			fillResponse(response, 204);
			//Json::Value userJson = receiver.publicProfileToJSON();
			//expandAttributes(userJson);
			//response["user"] = userJson;
			return;
		}
		error = senderJson["error"].asString() + " - "
				+ receiverJson["error"].asString();
		dbuser.CloseDB();
	}
	fillResponse(response, code);
	response["error"] = error;
}

/*
 void ProfileController::acceptInvitation(Request &request,
 JsonResponse &response) {
 char email[50];
 int code = 401;
 string error;

 string token = request.getHeaderKeyValue("token");
 if (!isValidToken(token, error)) {
 fillResponse(response,401);
 response["error"] = error;
 return;
 }

 error = "Wrong number or type of parameters.";
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
 */

void ProfileController::filterUsers(Request &request, JsonResponse &response) {
	string job_pos = request.get("job_position", "");
	string skills = request.get("skills", "");
	string range = request.get("range", "");
	string user = request.get("user", "");

	Json::Value filter;
	if (!job_pos.empty()) {
		filter["job_position"] = job_pos;
	}
	if (!skills.empty()) {
		filter["skills"] = Json::arrayValue;
		std::string delimiter = ",";
		size_t pos = 0;
		std::string token;
		while ((pos = skills.find(delimiter)) != std::string::npos) {
			token = skills.substr(0, pos);
			filter["skills"].append(token);
			skills.erase(0, pos + delimiter.length());
		}
		filter["skills"].append(skills);

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
		filter["user"] = user;
	}
	cout << "filter: " << filter.toStyledString() << endl;
	dbUsers dbuser;
	dbuser.connect("./usersdb");
	string error;
	Json::Value users = dbuser.searchProfile(filter, error);
	Json::Value usersArray = Json::arrayValue;
	dbuser.CloseDB();
	for (Json::Value::iterator it = users.begin(); it != users.end(); ++it) {
		Json::Value user = (*it);
		expandAttributes(user);
		usersArray.append(user);
	}
	if (error == "") {
		response["users"] = usersArray;
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
	string skill = request.get("skills", "");
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
	//addRouteResponse("POST", "/users/{email}/invitation", ProfileController,
	//	acceptInvitation, JsonResponse);
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

