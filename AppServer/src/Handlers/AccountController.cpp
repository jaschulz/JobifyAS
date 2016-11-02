#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "AccountController.h"
#include "SSHandler.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include <curl/curl.h>
#include "../Model/Profile.h"
#include "../utils/Encrypt.h"


AccountController::AccountController() {

}

void AccountController::registerUser(Request &request, JsonResponse &response) {

	std::string data = request.getData();

	Json::Value root;

	Json::Reader reader;
//	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!reader.parse(data.c_str(), root)) {	
		fillResponse(response,401);
		//response.setCode(401);
		//response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = reader.getFormattedErrorMessages();
	} else {
		string username = root.get("email", "").asString();
		string pass = root.get("password", "").asString();
		Profile profile (username,pass);

		dbCredentials dbCont;
		dbCont.connect("./accounts");
		string error = dbCont.addNewUser(profile.profileToJSON());
		dbCont.CloseDB();

		if (error.compare("") == 0) {
			Json::Value JsonBody;
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			error = dbuser.editProfile(username,profile.publicProfileToJSON());
			dbuser.CloseDB();

			if (error.compare("") == 0) {
				fillResponse(response,200);
				//response.setCode(200);
				//response.setHeader("Content-Type", "application/json; charset=utf-8");
				response["token"] = generateToken(root.get("email", "").asString(),root.get("password", "").asString());
			}
		} else {
			fillResponse(response, 401);
			//response.setCode(401);
			//response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["error"] = error;
		}
	}
}

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


void AccountController::login(Request &request, JsonResponse &response) {

	Json::Reader reader;

	std::string data = request.getData();

	Json::Value root;

	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		std::cout << "Failed to parse" << reader.getFormattedErrorMessages();

	}

	
	string username = root.get("email", "").asString();
	string pass = root.get("password", "").asString();
	Profile profile(username, pass);

	dbCredentials dbCont;
	dbCont.connect("./accounts");
	Json::Value jsonProfile = profile.profileToJSON();
	string error = dbCont.verifyLogin(jsonProfile);
	dbCont.CloseDB();

	if (error == "") {
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		error = dbuser.getProfile(root);
		dbuser.CloseDB();
		if (error == "") {
			fillResponse(response, 200);
			response["token"] = generateToken(root.get("email", "").asString(),root.get("password", "").asString());
			response["user"] = root;
			return;
		}
	} 
	fillResponse(response, 401);
	response["error"] = error;

}

string AccountController::generateToken(const string &email, const string &password) const {
    time_t now = time(0);
    char *dt = ctime(&now);
    return encrypt::sha256(email + password + dt);
}

void AccountController::setup() {
	setPrefix("/api");
	addRouteResponse("POST", "/session", AccountController, login, JsonResponse);
	addRouteResponse("POST", "/users", AccountController, registerUser,
			JsonResponse);
	addRouteResponse("GET", "/printAccounts", AccountController, printDB,
			JsonResponse);
}

