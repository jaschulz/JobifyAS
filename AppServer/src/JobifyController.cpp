#include <unistd.h>
#include <stdlib.h>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "JobifyController.h"
#include "SSHandler.h"
#include "dbController.h"
#include <curl/curl.h>

int JobifyController::jsonContainsValue(const Json::Value& root, string key,
		string value) {
	const Json::Value& users = root["users"];
	for (Json::ValueConstIterator it = users.begin(); it != users.end(); ++it) {
		const Json::Value& user = *it;
		if (value.compare(user[key].asString()) == 0) {
			return 0;
		}
	}
	return 1;
}

void JobifyController::fillResponse(JsonResponse &response,
		JsonResponse &jResponse, int code) {
	response.setCode(code);
	response["data"] = jResponse;
	response.setHeader("Content-Type", "application/json; charset=utf-8");
}

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
	string password = root.get("password", "").asString();

	string error = "";

	dbController dbCont;
	dbCont.connect("./testdb");
	dbCont.addNewUser(root);
	dbCont.CloseDB();

	JsonResponse jResponse;


	if (error == "") {
		response.setCode(200);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["token"] = "ok";

	} else {

		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = error;

	}
}

void JobifyController::login(Request &request, JsonResponse &response) {

	Json::Value users;
	Json::Reader reader;

	std::string data = request.getData();

	Json::Value root;

	Json::Reader reader2;
	bool parsingSuccessful = reader2.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		std::cout << "Failed to parse 2" << reader2.getFormattedErrorMessages();

	}
	string username = root.get("email", "").asString();

	string password = root.get("password", "").asString();

	dbController dbCont;
	dbCont.connect("./testdb");
	string error = dbCont.verifyLogin(root);
	dbCont.CloseDB();

	if (error == "") {
		response.setCode(200);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["token"] = "Ok";
		response["user"] = root;
	} else {		
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "login Failed";
	}
}

void JobifyController::getJobPositions(Request &request,
		JsonResponse &response) {
	SSHandler ss;
	response = ss.handleGet("http://localhost:8080/job_positions");

}

void JobifyController::setup() {
	// Example of prefix, putting all the urls into "/api"
	setPrefix("/api");

	addRouteResponse("GET", "/login", JobifyController, login, JsonResponse);
	addRouteResponse("POST", "/registerUser", JobifyController, registerUser,
			JsonResponse);
	addRouteResponse("GET", "/job_positions", JobifyController, getJobPositions,
			JsonResponse);
}

