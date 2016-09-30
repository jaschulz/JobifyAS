#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "JobifyController.h"
#include "SSHandler.h"
#include "../db/dbCredentials.h"
#include "../db/dbController.h"
#include "../db/dbUsers.h"
#include <curl/curl.h>
#include "../Model/Profile.h"
#include "../utils/Encrypt.h"


JobifyController::JobifyController() {
    routeParams = new map<string, string>();
}

bool JobifyController::handles(string method, string url) {

    bool handle = false;
    string incomingKey = method + ":" + url;

    map<string, RequestHandlerBase *>::iterator it;
    for (it = routes.begin(); it != routes.end(); it++) {
        string replacedKey = replaceRouteParams(it->first);
        if (regex_match(incomingKey, regex(replacedKey))) {
            handle = true;
            break;
        }
    }

    return handle;
}


Response *JobifyController::process(Request &request) {

    string currentRequest = request.getMethod() + ":" + request.getUrl();

    Response *response = NULL;

    map<string, RequestHandlerBase *>::iterator it;
    for (it = routes.begin(); it != routes.end(); it++) {
        string key = it->first;


        string regexKey = replaceRouteParams(key);

        if (regex_match(currentRequest, regex(regexKey))) {

            //Only search for route params in regex keys
            if (regexKey.find(".*") != string::npos) {
                parseRouteParams(key, currentRequest);
            }

            response = it->second->process(request);
            break;
        }


    }

    return response;
}

void JobifyController::parseRouteParams(const string &key, const string &currentRequest) const {
    routeParams->clear();
    //Get map key
    unsigned long firstPos = key.find("{");
    unsigned long secondPos = key.find("}");

    string mapKey = key.substr(firstPos + 1, secondPos - firstPos - 1);

    //Get map value
    string requestTail = currentRequest.substr(firstPos);
    unsigned long incomingRequestValueEnd = requestTail.find("/");
    string value = currentRequest.substr(firstPos, incomingRequestValueEnd);

    routeParams->insert(std::pair<string, string>(mapKey, value));
}

//@Fede Due to a mongoose cpp "double url check" issue we have to run this method twice, so we validate that we are in first one
string JobifyController::replaceRouteParams(string key) const {

    string replacedKey = key;
    unsigned long firstPos = replacedKey.find("{");
    unsigned long secondPos = replacedKey.find("}");

    //Replace {param} with .*
    while (firstPos != string::npos && secondPos != string::npos) {
        replacedKey = replacedKey.replace(firstPos, secondPos - firstPos + 1, ".*");
        firstPos = replacedKey.find("{");
        secondPos = replacedKey.find("}");
    }

    return replacedKey;
}




void JobifyController::registerUser(Request &request, JsonResponse &response) {

	std::string data = request.getData();

	Json::Value root;

	Json::Reader reader;
	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		std::cout << "Failed to parse" << reader.getFormattedErrorMessages();

	}

	string username = root.get("email", "").asString();
	string pass = root.get("password", "").asString();
	string first_name = root.get("first_name", "").asString();
	string last_name = root.get("last_name", "").asString();
	Profile profile (username,pass,first_name,last_name);

	string error = "";

	dbCredentials dbCont;
	dbCont.connect("./testdb");
	error = dbCont.addNewUser(profile.profileToJSON());
	dbCont.CloseDB();

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


void JobifyController::login(Request &request, JsonResponse &response) {

	Json::Reader reader;

	std::string data = request.getData();

	Json::Value root;

	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		std::cout << "Failed to parse" << reader.getFormattedErrorMessages();

	}

	Profile profile(root);

	dbCredentials dbCont;
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

void JobifyController::editProfile(Request &request, JsonResponse &response) {
	char email;
	string data = request.getData();
	std::cout <<data<<endl;
	if (1 == sscanf(request.getUrl().c_str(),"/api/users/%s",&email)) {
		std::cout <<"scanf"<<endl;
		Json::Reader reader;
		Json::Value JsonBody;
		bool parsingSuccessful = reader.parse(data.c_str(), JsonBody); //parse process
		if (!parsingSuccessful) {
			std::cout << "Failed to parse" << reader.getFormattedErrorMessages();

		}
		dbUsers dbuser;
		dbuser.connect("./usersdb");
		string error = "";
		error = dbuser.editProfile(JsonBody);
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
	//addRouteResponse("GET", "/users/{email}", JobifyController, getProfile, JsonResponse);
	addRouteResponse("PUT", "/users/{email}", JobifyController, editProfile, JsonResponse);
	addRouteResponse("GET", "/job_positions", JobifyController, getJobPositions,
			JsonResponse);

	addRouteResponse("GET", "/printDB", JobifyController, printDB,
			JsonResponse);
}

