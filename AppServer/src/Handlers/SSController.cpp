#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "SSController.h"
#include "SSHandler.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include <curl/curl.h>
#include "../Model/Profile.h"

SSController::SSController() {

}


void SSController::getJobPositions(Request &request, JsonResponse &response) {
	SSHandler ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/job_positions",response);
}


std::string replaceSpace(std::string text) {
	
		std::cout <<"replace begin"<< endl;
	std::string space = " ";
	while(text.find(space) != string::npos){
		text.replace(text.find(space),space.length(),"\%20");

		std::cout << text<< endl;
	}
		std::cout <<"replace end"<< endl;
	return text;
}

void SSController::deleteJobPosition(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2 == sscanf(request.getUrl().c_str(),"/api/job_positions/categories/%99[^/]/%99[0-9a-zA-Z ]",charCategory,charName)) {
		string category(charCategory);
		string name(charName);		
		std::cout << category <<" - "<<name<< endl;
		name = replaceSpace(name);
		std::cout << category <<" - "<<name<< endl;
		SSHandler ss;
		ss.handleDelete("https://still-falls-40635.herokuapp.com/job_positions/categories/" + category + "/" + name,request,response);	
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::deleteSkill(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2 == sscanf(request.getUrl().c_str(),"/api/skills/categories/%99[^/]/%99[0-9a-zA-Z ]",charCategory,charName)) {
		string category(charCategory);
		string name(charName);		
		std::cout << category <<" - "<<name<< endl;
		name = replaceSpace(name);
		std::cout << category <<" - "<<name<< endl;
		SSHandler ss;
		ss.handleDelete("https://still-falls-40635.herokuapp.com/skills/categories/" + category + "/" + name,request,response);	
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::deleteCategory(Request &request, JsonResponse &response) {
	char charName[50];
	if (1 == sscanf(request.getUrl().c_str(),"/api/categories/%99[^/]",charName)) {
		string name(charName);		
		std::cout << " Name "<<name<< endl;
		name = replaceSpace(name);
		SSHandler ss;
		ss.handleDelete("https://still-falls-40635.herokuapp.com/categories/" + name,request,response);	
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}


void SSController::modifyJobPosition(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2 == sscanf(request.getUrl().c_str(),"/api/job_positions/categories/%99[^/]/%99[0-9a-zA-Z ]",charCategory,charName)) {
		string category(charCategory);
		string name(charName);		
		std::cout << category <<" - "<<name<< endl;
		name = replaceSpace(name);
		std::cout << category <<" - "<<name<< endl;
		SSHandler ss;
		ss.handlePut("https://still-falls-40635.herokuapp.com/job_positions/categories/" + category + "/" + name,request,response);	
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::modifySkill(Request &request, JsonResponse &response) {
	char charCategory[50];
	char charName[50];
	if (2 == sscanf(request.getUrl().c_str(),"/api/skills/categories/%99[^/]/%99[0-9a-zA-Z ]",charCategory,charName)) {
		string category(charCategory);
		string name(charName);		
		std::cout << category <<" - "<<name<< endl;
		name = replaceSpace(name);
		std::cout << category <<" - "<<name<< endl;
		SSHandler ss;
		ss.handlePut("https://still-falls-40635.herokuapp.com/skills/categories/" + category + "/" + name,request,response);	
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::modifyCategory(Request &request, JsonResponse &response) {
	char charName[50];
	if (1 == sscanf(request.getUrl().c_str(),"/api/categories/%99[^/]",charName)) {
		string name(charName);		
		name = replaceSpace(name);
		std::cout << "name - "<<name<< endl;
		SSHandler ss;
		ss.handlePut("https://still-falls-40635.herokuapp.com/categories/" + name,request,response);	
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::addJobPositions(Request &request, JsonResponse &response) {
	
	char charCategory[50];
	if (1 == sscanf(request.getUrl().c_str(),"/api/job_positions/categories/%s",charCategory)) {
		string category(charCategory);
		/*Json::Reader reader;
		std::cout << category << endl;
		std::string data = request.getData();
				std::cout << data << endl;
		Json::Value root;

		bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
		if (!parsingSuccessful) {
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = reader.getFormattedErrorMessages();
			return;
		}*/
		SSHandler ss;
		ss.handlePost("https://still-falls-40635.herokuapp.com/job_positions/categories/" + category,request,response);
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::addCategory(Request &request, JsonResponse &response) {
	SSHandler ss;
	ss.handlePost("https://still-falls-40635.herokuapp.com/categories",request,response);
}


void SSController::getCategories(Request &request, JsonResponse &response) {
	SSHandler ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/categories",response);
}

void SSController::getSkills(Request &request, JsonResponse &response) {
	SSHandler ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/skills",response);
}

void SSController::addSkills(Request &request, JsonResponse &response) {
	char charCategory[50];
	if (1 == sscanf(request.getUrl().c_str(),"/api/skills/categories/%s",charCategory)) {
		string category(charCategory);

		SSHandler ss;
		ss.handlePost("https://still-falls-40635.herokuapp.com/job_positions/categories/" + category,request,response);
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::filterJobPositionsByCategory(Request &request,
		JsonResponse &response) {
	char cat[50];
	if (1 == sscanf(request.getUrl().c_str(),"/api/job_positions/categories/%s",cat)) {
		string category(cat);
		SSHandler ss;
		ss.handleGet("https://still-falls-40635.herokuapp.com/job_positions/categories/"+category,response);
	} else {		
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::filterSkillsByCategory(Request &request,
		JsonResponse &response) {
	char cat[50];
	if (1 == sscanf(request.getUrl().c_str(),"/api/skills/categories/%s",cat)) {
		string category(cat);
		SSHandler ss;
		ss.handleGet("https://still-falls-40635.herokuapp.com/skills/categories/"+category,response);
	} else {		
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "Wrong number or type of parameters.";
	}
}

void SSController::setup() {
	setPrefix("/api");

	addRouteResponse("GET", "/job_positions", SSController, getJobPositions,JsonResponse);

	addRouteResponse("GET", "/job_positions/categories/{category}", SSController, filterJobPositionsByCategory,
			JsonResponse);
	addRouteResponse("POST", "/job_positions/categories/{category}", SSController, addJobPositions,
				JsonResponse);
	addRouteResponse("PUT", "/job_positions/categories/{category}/{name}", SSController, modifyJobPosition,
				JsonResponse);
	addRouteResponse("POST", "/categories", SSController, addCategory,JsonResponse);

	addRouteResponse("DELETE", "/job_positions/categories/{category}/{name}", SSController, deleteJobPosition,
			JsonResponse);
	addRouteResponse("GET", "/skills", SSController, getSkills,
			JsonResponse);
	addRouteResponse("POST", "/skills/categories/{category}", SSController, addSkills,
				JsonResponse);
	addRouteResponse("PUT", "/skills/categories/{category}/{name}", SSController, modifySkill,
				JsonResponse);
	addRouteResponse("DELETE", "/skills/categories/{category}/{name}", SSController, deleteSkill,
			JsonResponse);
	addRouteResponse("DELETE", "/categories/{name}", SSController, deleteCategory, JsonResponse);

	addRouteResponse("PUT", "/categories/{name}", SSController, modifyCategory, JsonResponse);

	addRouteResponse("GET", "/categories", SSController, getCategories,
			JsonResponse);
	addRouteResponse("GET", "/skills/categories/{category}", SSController, filterSkillsByCategory,
			JsonResponse);

}

