#include <unistd.h>
#include <stdlib.h>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "JobifyController.h"
#include "SSHandler.h"
#include "dbController.h"
#include <curl/curl.h>
#include <openssl/sha.h>

string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    ss << hash;
    return ss.str();
}

void JobifyController::fillResponse(JsonResponse &response,
		JsonResponse &jResponse, int code) {
	response.setCode(code);
	response["data"] = jResponse;
	response.setHeader("Content-Type", "application/json; charset=utf-8");
}

Json::Value hashCredentials(Json::Value root){

	string username = root.get("email", "").asString();
	string pass = root.get("password", "").asString();
	string password = sha256(pass);

	Json::Value credentials;
	credentials["email"] = username;
	credentials["password"] = password;
	return credentials;
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

	Json::Value credentials = hashCredentials(root);

	string error = "";

	dbController dbCont;
	dbCont.connect("./testdb");
	error = dbCont.addNewUser(credentials);
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

	Json::Value credentials = hashCredentials(root);


	dbController dbCont;
	dbCont.connect("./testdb");
	string error = dbCont.verifyLogin(credentials);
	dbCont.CloseDB();

	if (error == "") {
		response.setCode(200);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["token"] = generateToken(root.get("email", "").asString(),root.get("password", "").asString());
		response["user"] = root;
	} else {		
		response.setCode(401);
		response.setHeader("Content-Type", "application/json; charset=utf-8");
		response["error"] = "login Failed";
	}
}

string JobifyController::generateToken(const string &email, const string &password) const {
    time_t now = time(0);
    char *dt = ctime(&now);
    return sha256(email + password + dt);
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

