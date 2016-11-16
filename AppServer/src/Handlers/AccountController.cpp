#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "AccountController.h"
#include "CurlWrapper.h"
#include "FBHandler.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include <curl/curl.h>
#include "../Model/Profile.h"
#include "../Model/Credentials.h"
#include "../Model/fbCredentials.h"
#include "../Model/jobifyCredentials.h"
#include "../utils/utils.h"


AccountController::AccountController() {

}

void AccountController::registerUser(Request &request, JsonResponse &response) {

	std::string data = request.getData();

	Json::Value root;

	Json::Reader reader;
	if (!reader.parse(data.c_str(), root)) {	
		fillResponse(response,401);
		response["error"] = reader.getFormattedErrorMessages();
	} else {
		string email = root.get("email", "").asString();
		string pass = root.get("password", "").asString();
		string token = generateToken(email, pass);
		jobifyCredentials credentials (email,pass,token);

		string error;
		//cout<<"1"<<endl;

		if (addNewUser(credentials,error)) {
			Profile profile (email);
			Json::Value JsonBody;
			dbUsers dbuser;
			//cout<<"2"<<endl;
			dbuser.connect("./usersdb");
			error = dbuser.editProfile(email,profile.publicProfileToJSON());
			dbuser.CloseDB();

			if (error.compare("") == 0) {
				fillResponse(response,200);
				response["token"] = token;
			}
		} else {	
			fillResponse(response, 401);
			response["error"] = error;
		}
	}
}

bool AccountController::addNewUser(Credentials &credentials, string error) {
	dbCredentials dbCont;
	dbCont.connect("./accounts");
	bool isCreated = dbCont.addNewUser(credentials.toJSON(),error);
	dbCont.CloseDB();
	return isCreated;
}
/*
void AccountController::printDB(Request &request, JsonResponse &response) {
	dbCredentials dbCont;
	dbCont.connect("./accounts");
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


void AccountController::fbLogin(Request &request, JsonResponse &response) {

	Json::Reader reader;
	Json::Value root;
	std::string data = request.getData();


	if (reader.parse(data.c_str(), root)) {		
		fillResponse(response, 401);
		response["error"] = reader.getFormattedErrorMessages();
	}
	FBHandler fbh;
	
	JsonResponse fbBasicData = fbh.getBasicData(request);

	
	string email = fbBasicData.get("email", "").asString();
	string fbid = fbBasicData.get("id", "").asString();
	string token = generateToken(email,"");
	fbCredentials credentials(email, fbid,token);
	string error;
	if (addNewUser(credentials,error)) {
		dbUsers dbuser;
		JsonResponse fbData = fbh.getData(request);
		string fname = fbData.get("first_name", "").asString();
		string lname = fbData.get("last_name", "").asString();
		string locationId = fbData["location"]["id"].asString();
		JsonResponse fbLocationData = fbh.getLocationData(request, locationId);
		double lat = fbLocationData["location"]["latitude"].asDouble();
		double longitude = fbLocationData["location"]["longitude"].asDouble();
		Profile profile (email,fname,lname,lat,longitude);
		dbuser.connect("./usersdb");
		Json::Value publicProfile = profile.publicProfileToJSON();
		error = dbuser.editProfile(email,publicProfile);
		dbuser.CloseDB();
		if (error.compare("") == 0) {
			fillResponse(response,200);
			response["token"] = token;
			response["user"] = publicProfile;
		}
	}
}

void AccountController::login(Request &request, JsonResponse &response) {

	Json::Reader reader;

	std::string data = request.getData();

	Json::Value req;
	Json::Value root;

	if (reader.parse(data.c_str(), req)) {		
		fillResponse(response, 401);
		response["error"] = reader.getFormattedErrorMessages();
		return;
	}
	if (!request.getHeaderKeyValue("Authorization").empty()){
		fbLogin(request,response);
	} else {
	
		string email = req.get("email", "").asString();
		string pass = req.get("password", "").asString();
		string token = generateToken(email,pass);
		jobifyCredentials credentials(email, pass,token);

		dbCredentials dbCont;
		dbCont.connect("./accounts");
		Json::Value jsonProfile = credentials.toJSON();
		string error;

		if (dbCont.verifyLogin(jsonProfile,error)) {
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			error = dbuser.getProfile(root);
			dbuser.CloseDB();
			dbCont.CloseDB();
			if (error == "") {
				fillResponse(response, 200);
				response["token"] = token;
				response["user"] = root;
				return;
			}
		} 	
		dbCont.CloseDB();
		fillResponse(response, 401);
		response["error"] = error;
	}
}

void AccountController::getFacebookData(Request &request, JsonResponse &response) {
	Json::Reader reader;
	std::string data = request.getData();
	Json::Value root;
	/*if (!reader.parse(data.c_str(), root)) {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "getFacebookData: " + reader.getFormattedErrorMessages();
		return;
	}*/
	cout<<"header: "<<request.getHeaderKeyValue("Authorization")<<endl;
	string token = root["token"].asString();
	string fbid = root["fbid"].asString();
	CurlWrapper ss;
	ss.handleGet("https://graph.facebook.com/v2.8/"+fbid+"?fields=about,birthday,email,first_name,gender,last_name,location",request,response);
	string locationId = response["location"]["id"].asString();
	ss.handleGet("https://graph.facebook.com/v2.8/"+locationId+"?fields=location",request,response);
}

string AccountController::generateToken(const string &email, const string &password) const {
    time_t now = time(0);
    char *dt = ctime(&now);
    return utils::sha256(email + password + dt);
}

void AccountController::setup() {
	setPrefix("/api");
	addRouteResponse("POST", "/session", AccountController, login, JsonResponse);
	addRouteResponse("POST", "/users", AccountController, registerUser,
			JsonResponse);
	//addRouteResponse("GET", "/printAccounts", AccountController, printDB,JsonResponse);
	addRouteResponse("GET", "/fbdata", AccountController, getFacebookData, JsonResponse);
}

