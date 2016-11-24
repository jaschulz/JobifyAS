#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <algorithm> 
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
	int code = 401;
	string error = "Wrong number or type of parameters.";
	string data = request.getData();
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]/contacts",email)) {
		string mail(email);
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {			
			int i = 0;
			string	newContact = JsonBody.get("email","").asString();	
			dbUsers dbuser;
			dbuser.connect("./usersdb");	
			Json::Value senderJson = dbuser.getProfile(mail);
			Json::Value receiverJson = dbuser.getProfile(newContact);
			if (senderJson["error"].isNull() && receiverJson["error"].isNull()){
				Profile sender(senderJson);
				Profile receiver(receiverJson);	
				if (dbuser.sendInvitation(sender,receiver,error,code)) {		
					dbuser.CloseDB();	
					fillResponse(response,code);
					response["user"] = JsonBody;				
					return;
				}
			}
			error = senderJson["error"].asString() + " - " + receiverJson["error"].asString();
			dbuser.CloseDB();   
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response,code); 
	response["error"] = error;   
}

void ProfileController::recommendUser(Request &request, JsonResponse &response) {
	char email[50];
	int code = 401;
	string error = "Wrong number or type of parameters.";
	string data = request.getData();
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]/recommendation",email)) {
		string mail(email);
			cout<<"mail:"<<mail<<endl;
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {			
			int i = 0;
			string	recommendedUser = JsonBody.get("email","").asString();	
					cout<<"recommendedUser:"<<recommendedUser<<endl;
			dbUsers dbuser;
			dbuser.connect("./usersdb");	
			Json::Value senderJson = dbuser.getProfile(mail);
			Json::Value receiverJson = dbuser.getProfile(recommendedUser);
			if (senderJson["error"].isNull() && receiverJson["error"].isNull()){
				cout<<"senderJson: "<<senderJson.toStyledString();
				Profile sender(senderJson);
				Profile receiver(receiverJson);	
				dbuser.recommendUser(sender,receiver);
				dbuser.CloseDB();	
				fillResponse(response,201);
				response["user"] = JsonBody;		
				return;
			}
			error = senderJson["error"].asString() + " - " + receiverJson["error"].asString();
			dbuser.CloseDB();   
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response,code); 
	response["error"] = error;   
}

void ProfileController::acceptInvitation(Request &request, JsonResponse &response) {
	char email[50];
	int code = 401;
	string error = "Wrong number or type of parameters.";
	string data = request.getData();
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%99[^/]/invitation",email)) {
		string mail(email);
		Json::Value JsonBody;
		Json::Reader reader;
		string error = "";
		if (reader.parse(data.c_str(), JsonBody)) {			
			int i = 0;
			string	newContact = JsonBody.get("email","").asString();	
			dbUsers dbuser;
			dbuser.connect("./usersdb");	
			Json::Value receiverJson = dbuser.getProfile(mail);
			Json::Value senderJson = dbuser.getProfile(newContact);
			if (senderJson["error"].isNull() && receiverJson["error"].isNull()){
				Profile sender(senderJson);
				Profile receiver(receiverJson);	
				if (dbuser.acceptInvitation(sender,receiver,error,code)) {		
					dbuser.CloseDB();	
					fillResponse(response,code);
					response["user"] = JsonBody;				
					return;
				}
			}
			error = senderJson["error"].asString() + " - " + receiverJson["error"].asString();
			dbuser.CloseDB();   
		} else {
			error = reader.getFormattedErrorMessages();
		}
	}
	fillResponse(response,code); 
	response["error"] = error;   
}

void ProfileController::filterUsers(Request &request, JsonResponse &response) {
	string job_pos = request.get("job_position","");
	string skills = request.get("skills","");
	string range = request.get("range","");
	string user = request.get("user","");
	Json::Value filter;
	if (!job_pos.empty()) {
		filter["job_pos"] = job_pos;
	}
	if (!skills.empty()) {
		filter["skills"] = skills;
	}
	if (!range.empty()) {
		filter["range"] = range;
	}
	if (!user.empty()) {
		transform(user.begin(), user.end(), user.begin(), ::toupper);
		filter["user"] = user;
	}
	dbUsers dbuser;
	dbuser.connect("./usersdb");
	string error;
	Json::Value users = dbuser.searchProfile(filter,error);
	dbuser.CloseDB();
	if (error == "") {
		response["users"] = users;
		return;
	}
	fillResponse(response,401); 
	response["error"] = error;

}

void ProfileController::setup() {
	setPrefix("/api");
	addRouteResponse("GET", "/users/{email}", ProfileController, getProfile, JsonResponse);
	addRouteResponse("PUT", "/users/{email}", ProfileController, editProfile, JsonResponse);
	addRouteResponse("POST", "/users/{email}/contacts", ProfileController, addContact, JsonResponse);
	addRouteResponse("POST", "/users/{email}/invitation", ProfileController, acceptInvitation, JsonResponse);
	addRouteResponse("GET", "/users/{email}/contacts", ProfileController, getContacts, JsonResponse);
	addRouteResponse("POST", "/users/{email}/location", ProfileController, setLocation, JsonResponse);
	addRouteResponse("POST", "/users/{email}/recommendation", ProfileController, recommendUser, JsonResponse);
	addRouteResponse("GET", "/users", ProfileController, filterUsers, JsonResponse);
	//addRouteResponse("GET", "/printProfiles", ProfileController, printDB,JsonResponse);
}

