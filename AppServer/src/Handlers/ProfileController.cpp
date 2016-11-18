#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "ProfileController.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include <curl/curl.h>
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

void ProfileController::editProfile(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
	//TODO verify token
	string error = "Wrong number or type of parameters.";	
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%s",email)) {
		error = "";
		Json::Reader reader;
		Json::Value JsonBody;
		if (reader.parse(data.c_str(), JsonBody)) {
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			string key(email);
			error = dbuser.editProfile(key,JsonBody);
			dbuser.CloseDB();
			if (error == "") {
				fillResponse(response,200);
				response["user"] = JsonBody;
				return;
			} 
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response,401);
	response["error"] = error;
}


void ProfileController::getProfile(Request &request, JsonResponse &response) {
	char email[50];	
	string error = "Wrong number or type of parameters.";	
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]",email)) {
		string mail(email);
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		Json::Value JsonBody = dbuser.getProfile(mail);
		error = JsonBody.get("error", "").asString();
		dbuser.CloseDB();
		if (error == "") {
			fillResponse(response,200);
			response["user"] = JsonBody;
			return;
		}
	}
	fillResponse(response,401);
	response["error"] = error;	     
}

void ProfileController::getContacts(Request &request, JsonResponse &response) {
	char email[50];	
	string error = "Wrong number or type of parameters.";	
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]/contacts",email)) {
		string mail(email);
		Json::Value JsonBody;
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		JsonBody = dbuser.getContacts(mail);
		cout<<"JsonBody: "<< JsonBody.toStyledString()<<endl;
		error = JsonBody.get("error", "").asString();
		dbuser.CloseDB();
		if (error == "") {			
			fillResponse(response,200);
			response["contacts"] = JsonBody["contacts"];
			return;
		}
	}
	fillResponse(response,401); 
	response["error"] = error;        
}

void ProfileController::setLocation(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]/location",email)) {
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
			response["user"] = dbuser.setLocation(mail,location);
			dbuser.CloseDB();
			if (error == "") {			
				fillResponse(response,200);
				response["user"] = JsonBody;
				return;
			}
		}  else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response,401); 
	response["error"] = "Wrong number or type of parameters.";        
}

void ProfileController::addContact(Request &request, JsonResponse &response) {
	char email[50];
	string data = request.getData();
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]/contacts",email)) {
		string mail(email);
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			error = dbuser.addContact(mail,JsonBody);
			dbuser.CloseDB();
			if (error == "") {			
				fillResponse(response,200);
				response["user"] = JsonBody;
				return;
			}
		} else {
			error = reader.getFormattedErrorMessages();
		}
	} 
	fillResponse(response,401); 
	response["error"] = "Wrong number or type of parameters.";        
}

void ProfileController::setup() {
	setPrefix("/api");
	addRouteResponse("GET", "/users/{email}", ProfileController, getProfile, JsonResponse);
	addRouteResponse("PUT", "/users/{email}", ProfileController, editProfile, JsonResponse);
	addRouteResponse("POST", "/users/{email}/contacts", ProfileController, addContact, JsonResponse);
	addRouteResponse("GET", "/users/{email}/contacts", ProfileController, getContacts, JsonResponse);
	addRouteResponse("POST", "/users/{email}/location", ProfileController, setLocation, JsonResponse);
	//addRouteResponse("GET", "/printProfiles", ProfileController, printDB,JsonResponse);
}

