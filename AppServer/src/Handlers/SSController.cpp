#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "SSController.h"
#include "CurlWrapper.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include <curl/curl.h>
#include "../Model/Profile.h"

SSController::SSController() {

}

void SSController::getJobPositions(Request &request, JsonResponse &response) {
	getJobPositions(response);
}

void SSController::getJobPositions(JsonResponse &response) {
	CurlWrapper ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/job_positions",
			"", response);
}

std::string replaceSpace(std::string text) {
	std::string space = " ";
	while (text.find(space) != string::npos) {
		text.replace(text.find(space), space.length(), "\%20");

		std::cout << text << endl;
	}
//		std::cout <<"replace end"<< endl;
	return text;
}

void SSController::deleteJobPosition(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2
			== sscanf(request.getUrl().c_str(),
					"/api/job_positions/categories/%99[^/]/%99[0-9a-zA-Z ]",
					charCategory, charName)) {
		string category(charCategory);
		string name(charName);
		std::cout << category << " - " << name << endl;
		name = replaceSpace(name);
		std::cout << category << " - " << name << endl;
		CurlWrapper ss;
		ss.handleDelete(
				"https://still-falls-40635.herokuapp.com/job_positions/categories/"
						+ category + "/" + name, request, response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::deleteSkill(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2
			== sscanf(request.getUrl().c_str(),
					"/api/skills/categories/%99[^/]/%99[0-9a-zA-Z ]",
					charCategory, charName)) {
		string category(charCategory);
		string name(charName);
		std::cout << category << " - " << name << endl;
		name = replaceSpace(name);
		std::cout << category << " - " << name << endl;
		CurlWrapper ss;
		ss.handleDelete(
				"https://still-falls-40635.herokuapp.com/skills/categories/"
						+ category + "/" + name, request, response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::deleteCategory(Request &request, JsonResponse &response) {
	char charName[50];
	if (1
			== sscanf(request.getUrl().c_str(), "/api/categories/%99[^/]",
					charName)) {
		string name(charName);
		name = replaceSpace(name);
		std::cout << "Name:" << name << "." << endl;
		CurlWrapper ss;
		ss.handleDelete(
				"https://still-falls-40635.herokuapp.com/categories/" + name,
				request, response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::modifyJobPosition(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2
			== sscanf(request.getUrl().c_str(),
					"/api/job_positions/categories/%99[^/]/%99[0-9a-zA-Z ]",
					charCategory, charName)) {
		string category(charCategory);
		string name(charName);
		std::cout << category << " - " << name << endl;
		name = replaceSpace(name);
		std::cout << category << " - " << name << endl;
		CurlWrapper ss;
		ss.handlePut(
				"https://still-falls-40635.herokuapp.com/job_positions/categories/"
						+ category + "/" + name, request, response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::modifySkill(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2
			== sscanf(request.getUrl().c_str(),
					"/api/skills/categories/%99[^/]/%99[0-9a-zA-Z ]",
					charCategory, charName)) {
		string category(charCategory);
		string name(charName);
		std::cout << category << " - " << name << endl;
		name = replaceSpace(name);
		std::cout << category << " - " << name << endl;
		CurlWrapper ss;
		ss.handlePut(
				"https://still-falls-40635.herokuapp.com/skills/categories/"
						+ category + "/" + name, request, response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::modifyCategory(Request &request, JsonResponse &response) {
	char charName[50];
	if (1
			== sscanf(request.getUrl().c_str(), "/api/categories/%99[^/]",
					charName)) {
		string name(charName);
		name = replaceSpace(name);
		std::cout << "modifyCategory: name -" << name << "." << endl;
		CurlWrapper ss;
		ss.handlePut(
				"https://still-falls-40635.herokuapp.com/categories/" + name,
				request, response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::addJobPositions(Request &request, JsonResponse &response) {

	char charCategory[50];
	if (1
			== sscanf(request.getUrl().c_str(),
					"/api/job_positions/categories/%s", charCategory)) {
		string category(charCategory);
		std::cout << "addJobPositions -> category:" << category << endl;
		CurlWrapper ss;
		ss.handlePost(
				"https://still-falls-40635.herokuapp.com/job_positions/categories/"
						+ category, request.getData(), response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::addJobPositions(Json::Value newJP, JsonResponse &response) {
	CurlWrapper ss;
	ss.handlePost("https://still-falls-40635.herokuapp.com/job_positions/categories/" + newJP.get("category","").asString(),
			newJP.toStyledString(), response);
}

void SSController::addCategory(Request &request, JsonResponse &response) {
	CurlWrapper ss;
	ss.handlePost("https://still-falls-40635.herokuapp.com/categories", request.getData(),
			response);
}

void SSController::addCategory(Json::Value newCat, JsonResponse &response) {
	CurlWrapper ss;
	ss.handlePost("https://still-falls-40635.herokuapp.com/categories", newCat.toStyledString(),
			response);
}

void SSController::getCategories(Request &request, JsonResponse &response) {
	getCategories(response);
}

void SSController::getSkills(Request &request, JsonResponse &response) {
	getSkills(response);
}

void SSController::getCategories(JsonResponse &response) {
	CurlWrapper ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/categories", "",
			response);
}

void SSController::getSkills(JsonResponse &response) {
	CurlWrapper ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/skills", "",
			response);
}

void SSController::addSkills(Request &request, JsonResponse &response) {
	char charCategory[50];
	if (1
			== sscanf(request.getUrl().c_str(), "/api/skills/categories/%s",
					charCategory)) {
		string category(charCategory);
		std::cout << "addSkills in cat: " << category << endl;
		CurlWrapper ss;
		ss.handlePost(
				"https://still-falls-40635.herokuapp.com/skills/categories/"
						+ category, request.getData(), response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}


void SSController::addSkills(Json::Value newSkill, JsonResponse &response) {

		CurlWrapper ss;
		cout<<"1"<<newSkill.get("category","").asString()<<endl;
		//cout<<"2"<<newSkill.asString()<<endl;
		ss.handlePost(
				"https://still-falls-40635.herokuapp.com/skills/categories/"
						+ newSkill.get("category","").asString(), newSkill.toStyledString(), response);
}

void SSController::filterJobPositionsByCategory(Request &request,
		JsonResponse &response) {
	char cat[50];
	if (1
			== sscanf(request.getUrl().c_str(),
					"/api/job_positions/categories/%s", cat)) {
		string category(cat);
		CurlWrapper ss;
		ss.handleGet(
				"https://still-falls-40635.herokuapp.com/job_positions/categories/"
						+ category, "", response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::filterSkillsByCategory(Request &request,
		JsonResponse &response) {
	char cat[50];
	if (1
			== sscanf(request.getUrl().c_str(), "/api/skills/categories/%s",
					cat)) {
		string category(cat);
		CurlWrapper ss;
		ss.handleGet(
				"https://still-falls-40635.herokuapp.com/skills/categories/"
						+ category, "", response);
	} else {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

std::map <string, Entity> SSController::getMap(Json::Value array){
	std::map <string, Entity> entityMap;
	for(Json::Value::iterator it = array.begin(); it !=array.end(); ++it){
		Json::Value value = (*it);
		std::string name = value.get("name","").asString();
		std::string cat = value.get("category","").asString();
		std::string desc = value.get("description","").asString();
		Entity objEntity(name, desc, cat);
		entityMap[name] = objEntity;
	}
	return entityMap;
}

std::map <string, Entity> SSController::getSkillsMap(){
	JsonResponse response;
	getSkills(response);
	Json::Value array = response["skills"];
	return getMap(array);
}

std::map <string, Entity> SSController::getJPMap(){
	JsonResponse response;
	getJobPositions(response);
	Json::Value array = response["job_positions"];
	return getMap(array);
}

std::map <string, Category> SSController::getCategoriesMap(){
	JsonResponse response;
	getCategories(response);
	Json::Value array = response["categories"];
	std::map <string, Category> categoriesMap;
	for(Json::Value::iterator it = array.begin(); it !=array.end(); ++it){
		Json::Value value = (*it);
		std::string name = value.get("name","").asString();
		std::string desc = value.get("description","").asString();
		Category category(name, desc);
		categoriesMap[name] = category;
	}
	return categoriesMap;
}

void SSController::setup() {
	setPrefix("/api");

	addRouteResponse("GET", "/job_positions", SSController, getJobPositions,
			JsonResponse);

	addRouteResponse("GET", "/job_positions/categories/{category}",
			SSController, filterJobPositionsByCategory, JsonResponse);
	addRouteResponse("POST", "/job_positions/categories/{category}",
			SSController, addJobPositions, JsonResponse);
	addRouteResponse("PUT", "/job_positions/categories/{category}/{name}",
			SSController, modifyJobPosition, JsonResponse);
	addRouteResponse("POST", "/categories", SSController, addCategory,
			JsonResponse);

	addRouteResponse("DELETE", "/job_positions/categories/{category}/{name}",
			SSController, deleteJobPosition, JsonResponse);
	addRouteResponse("GET", "/skills", SSController, getSkills, JsonResponse);
	addRouteResponse("POST", "/skills/categories/{category}", SSController,
			addSkills, JsonResponse);
	addRouteResponse("PUT", "/skills/categories/{category}/{name}",
			SSController, modifySkill, JsonResponse);
	addRouteResponse("DELETE", "/skills/categories/{category}/{name}",
			SSController, deleteSkill, JsonResponse);
	addRouteResponse("DELETE", "/categories/{name}", SSController,
			deleteCategory, JsonResponse);

	addRouteResponse("PUT", "/categories/{name}", SSController, modifyCategory,
			JsonResponse);

	addRouteResponse("GET", "/categories", SSController, getCategories,
			JsonResponse);
	addRouteResponse("GET", "/skills/categories/{category}", SSController,
			filterSkillsByCategory, JsonResponse);
}

