#include <unistd.h>
#include <stdlib.h>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "JobifyController.h"
#include "SSHandler.h"
#include "../db/dbController.h"
#include <curl/curl.h>
#include <openssl/sha.h>
#include "../Model/Profile.h"
#include "../utils/Encrypt.h"

void JobifyController::registerUser(Request &request, JsonResponse &response) {

	Json::Value users;
	Json::Reader reader;

	std::string data = request.getData();

	Json::Value root;

	Json::Reader reader2;
	bool parsingSuccessful = reader2.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		std::cout << "Failed to parse" << reader2.getFormattedErrorMessages();

	}

	string username = root.get("email", "").asString();
	string pass = root.get("password", "").asString();
	string first_name = root.get("first_name", "").asString();
	string last_name = root.get("last_name", "").asString();
	Profile profile (username,pass,first_name,last_name);

	string error = "";

	dbController dbCont;
	dbCont.connect("./testdb");
	error = dbCont.addNewUser(profile.profileToJSON());
	dbCont.CloseDB();

	JsonResponse jResponse;


	if (error.compare("") == 0) {
		response.setCode(200);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["token"] = generateToken(root.get("email", "").asString(),root.get("password", "").asString());

	} else {

		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = error;

	}
}

void JobifyController::printDB(Request &request, JsonResponse &response) {
	dbController dbCont;
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


void JobifyController::login(Request &request, JsonResponse &response) {

	Json::Reader reader;

	std::string data = request.getData();

	Json::Value root;

	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		std::cout << "Failed to parse 2" << reader.getFormattedErrorMessages();

	}

	Profile profile(root);

	dbController dbCont;
	dbCont.connect("./testdb");
	Json::Value jsonProfile = profile.profileToJSON();
	string error = dbCont.verifyLogin(jsonProfile);
	dbCont.CloseDB();

	if (error == "") {
		response.setCode(200);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["token"] = generateToken(root.get("email", "").asString(),root.get("password", "").asString());
		response["user"] = jsonProfile;
	} else {		
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = error;
	}
}

string JobifyController::generateToken(const string &email, const string &password) const {
    time_t now = time(0);
    char *dt = ctime(&now);
    return encrypt::sha256(email + password + dt);
}

void JobifyController::getJobPositions(Request &request,
		JsonResponse &response) {
	SSHandler ss;
	response = ss.handleGet("http://localhost:8080/job_positions");

}

void JobifyController::setup() {
	// Example of prefix, putting all the urls into "/api"
	setPrefix("/api");

	addRouteResponse("GET", "/session", JobifyController, login, JsonResponse);
	addRouteResponse("POST", "/users", JobifyController, registerUser,
			JsonResponse);
	addRouteResponse("GET", "/job_positions", JobifyController, getJobPositions,
			JsonResponse);

	addRouteResponse("GET", "/printDB", JobifyController, printDB,
			JsonResponse);
}
