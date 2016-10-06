#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "ProfileController.h"
#include "SSHandler.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include <curl/curl.h>
#include "../Model/Profile.h"
#include "../utils/Encrypt.h"


ProfileController::ProfileController() {

}

void ProfileController::printDB(Request &request, JsonResponse &response) {
	dbCredentials dbCont;
	dbCont.connect("./usersdb");
//	dbCont.connect("./testdb");
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


void ProfileController::editProfile(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
	//TODO verify token
	string error = "Wrong number or type of parameters.";	
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%s",email)) {
		error = "";
		Json::Reader reader;
		Json::Value JsonBody;
		bool parsingSuccessful = reader.parse(data.c_str(), JsonBody); //parse process
		if (parsingSuccessful) {
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			string key(email);
			error = dbuser.editProfile(key,JsonBody);
			dbuser.CloseDB();
			if (error == "") {
				response.setCode(200);
				response.setHeader("Content-Type", "application/json; charset=utf-8");
				response["user"] = JsonBody;
				return;
			} 
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	response.setCode(401);
	response.setHeader("Content-Type", "application/json; charset=utf-8");
	response["error"] = error;
        
}


void ProfileController::getProfile(Request &request, JsonResponse &response) {
	char email[50];
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%s",email)) {
		string mail(email);
		Json::Value JsonBody;
		JsonBody["email"] = mail;
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		string error = "";
		error = dbuser.getProfile(JsonBody);
		dbuser.CloseDB();
		if (error == "") {
			response.setCode(200);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["user"] = JsonBody;
		} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = error;
		}
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
        
}

void ProfileController::setLocation(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]/location",email)) {
		string mail(email);
		Json::Value JsonBody;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(data.c_str(), JsonBody); //parse process
		if (!parsingSuccessful) {
			std::cout << "Failed to parse" << reader.getFormattedErrorMessages();
		}
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		string error = "";
		error = dbuser.setLocation(mail,JsonBody);
		dbuser.CloseDB();
		if (error == "") {
			response.setCode(200);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["user"] = JsonBody;
		} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = error;
		}
	} else {		
			response.setCode(401);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = "Wrong number or type of parameters.";
	}
        
}

void ProfileController::setup() {
	// Example of prefix, putting all the urls into "/api"
	setPrefix("/api");

	addRouteResponse("GET", "/users/{email}", ProfileController, getProfile, JsonResponse);
	addRouteResponse("PUT", "/users/{email}", ProfileController, editProfile, JsonResponse);
	addRouteResponse("POST", "/users/{email}/location", ProfileController, setLocation, JsonResponse);

	addRouteResponse("GET", "/printProfiles", ProfileController, printDB,
			JsonResponse);
}

