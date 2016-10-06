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
#include "../utils/Encrypt.h"


SSController::SSController() {

}


void SSController::getJobPositions(Request &request, JsonResponse &response) {
	SSHandler ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/job_positions",response);
}

/*void AccountController::addJobPositions(Request &request, JsonResponse &response) {
	char oldCategory[50];
	char oldName[50];
	if (2 == sscanf(request.getUrl().c_str(),"/api/job_positions/categories/%99[^/]/%s",oldCategory,oldName)) {
		string category(oldCategory);
		string name(oldName);
		SSHandler ss;
		ss.handlePost("https://still-falls-40635.herokuapp.com/job_positions/categories/" + category + "/" + name,request,response);	
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
}*/

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

void SSController::getCategories(Request &request, JsonResponse &response) {
	SSHandler ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/categories",response);
}

void SSController::getSkills(Request &request, JsonResponse &response) {
	SSHandler ss;
	ss.handleGet("https://still-falls-40635.herokuapp.com/skills",response);
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
	// Example of prefix, putting all the urls into "/api"
	setPrefix("/api");
	addRouteResponse("GET", "/job_positions", SSController, getJobPositions,
			JsonResponse);
	addRouteResponse("POST", "/job_positions/categories/{category}", SSController, addJobPositions,
				JsonResponse);
	addRouteResponse("GET", "/categories", SSController, getCategories,
			JsonResponse);
	addRouteResponse("GET", "/skills", SSController, getSkills,
			JsonResponse);
	addRouteResponse("GET", "/job_positions/categories/{category}", SSController, filterJobPositionsByCategory,
			JsonResponse);
	addRouteResponse("GET", "/skills/categories/{category}", SSController, filterSkillsByCategory,
			JsonResponse);
}

