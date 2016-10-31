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
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


AccountController::AccountController() {

}

void AccountController::registerUser(Request &request, JsonResponse &response) {

	std::string data = request.getData();

	Json::Value root;

	string error = requestToJson(request,root);
/*	Json::Reader reader;
	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = reader.getFormattedErrorMessages();
		return;

	}*/

	string username = root.get("email", "").asString();
	string pass = root.get("password", "").asString();
	string first_name = root.get("first_name", "").asString();
	string last_name = root.get("last_name", "").asString();
	Profile profile (username,pass,first_name,last_name);

	dbCredentials dbCont;
	dbCont.connect("./testdb");
	error = dbCont.addNewUser(profile.profileToJSON());
	dbCont.CloseDB();

	if (error.compare("") == 0) {
		Json::Value JsonBody;
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		error = dbuser.editProfile(username,profile.publicProfileToJSON());
		dbuser.CloseDB();

		if (error.compare("") == 0) {
			response.setCode(200);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["token"] = generateToken(root.get("email", "").asString(),root.get("password", "").asString());
		}
	} else {

		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = error;
	}
}

void AccountController::printDB(Request &request, JsonResponse &response) {
	dbCredentials dbCont;
	dbCont.connect("./testdb");
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

	string error = requestToJson(request,root);
/*
	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		std::cout << "Failed to parse" << reader.getFormattedErrorMessages();

	}
*/
	Profile profile(root);

	dbCredentials dbCont;
	dbCont.connect("./testdb");
	Json::Value jsonProfile = profile.profileToJSON();
	error = dbCont.verifyLogin(jsonProfile);
	dbCont.CloseDB();

	if (error == "") {
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		error = dbuser.getProfile(root);
		dbuser.CloseDB();
		if (error == "") {
			response.setCode(200);
			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response["token"] = generateToken(root.get("email", "").asString(),root.get("password", "").asString());
			response["user"] = root;
			response["message"] = "Successful login";
			return;
		}
	} 
	response.setCode(401);
	response.setHeader("Content-Type", "application/json; charset=utf-8");
	response["error"] = error;

}

string AccountController::generateToken(const string &email, const string &password) const {
    time_t now = time(0);
    char *dt = ctime(&now);
    return encrypt::sha256(email + password + dt);
}

void AccountController::setup() {
	// Example of prefix, putting all the urls into "/api"
	setPrefix("/api");

	addRouteResponse("POST", "/session", AccountController, login, JsonResponse);
	addRouteResponse("POST", "/users", AccountController, registerUser,
			JsonResponse);
	addRouteResponse("GET", "/printAccounts", AccountController, printDB,
			JsonResponse);
}

