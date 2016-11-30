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
	std::string error = "";
	Json::Value root;

	Json::Reader reader;
	if (!reader.parse(data.c_str(), root)) {
		fillResponse(response, 401);
		response["error"] = "AccountController.registerUser"
				+ reader.getFormattedErrorMessages();
	} else {
		string email = root.get("email", "").asString();
		string pass = root.get("password", "").asString();
		string token = generateToken(email, pass);
		jobifyCredentials credentials(email, pass, token);

		if (addNewUser(credentials, error)) {
			Profile profile(email);
			Json::Value JsonBody = profile.profileToJSON();
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			error = dbuser.addProfile(email, JsonBody);
			dbuser.CloseDB();
			if (error.compare("") == 0) {
				fillResponse(response, 200);
				response["token"] = token;
			}
		} else {
			fillResponse(response, 401);
			response["error"] = error;
		}
	}
}

bool AccountController::addNewUser(Credentials &credentials, string &error) {
	dbCredentials dbCred;
	dbCred.connect("./accounts");
	bool isCreated = dbCred.addNewUser(credentials.toJSON(), error);
	cout << "addNewUser -> error: " << error << endl;
	dbCred.CloseDB();
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
	cout << fbBasicData.toStyledString() << endl;
	if (fbBasicData["error"].isNull()) {
		string email = fbBasicData.get("email", "").asString();
		string fbid = fbBasicData.get("id", "").asString();
		string token = generateToken(email, "");
		fbCredentials credentials(email, fbid, token);
		string error;
		if (addNewUser(credentials, error)) {
			dbUsers dbuser;
			JsonResponse fbData = fbh.getData(request);
			if (fbData["error"].isNull()) {
				string fname = fbData.get("first_name", "").asString();
				string lname = fbData.get("last_name", "").asString();
				string locationId = fbData["location"]["id"].asString();
				string pic = fbData["picture"]["data"]["url"].asString();
				JsonResponse fbLocationData = fbh.getLocationData(request,
						locationId);
				if (fbLocationData["error"].isNull()) {
					double lat =
							fbLocationData["location"]["latitude"].asDouble();
					double longitude =
							fbLocationData["location"]["longitude"].asDouble();
					string jp = "";
					Profile profile(email, fname, lname, pic, jp, lat,
							longitude);
					dbuser.connect("./usersdb");
					Json::Value publicProfile = profile.profileToJSON();
					error = dbuser.addProfile(email, publicProfile);
					dbuser.CloseDB();
					if (error.compare("") == 0) {
						fillResponse(response, 200);
						response["token"] = token;
						Json::Value userJson= profile.profileToJSON();
						expandAttributes(userJson);
						response["user"] = userJson;
					}
				} else {
					response["error"] = fbLocationData["error"]["message"];
				}
			} else {
				response["error"] = fbData["error"]["message"];
			}
		} else {
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			Json::Value jsonProfile = dbuser.getProfile(email);
			dbuser.CloseDB();
			Profile profile(jsonProfile);
			fillResponse(response, 200);
			response["token"] = token;
			Json::Value userJson= profile.publicProfileToJSON();
			expandAttributes(userJson);
			response["user"] = userJson;
		}

	} else {
		response["error"] = fbBasicData["error"]["message"];
	}
}

void AccountController::login(Request &request, JsonResponse &response) {

	Json::Reader reader;

	std::string data = request.getData();

	Json::Value req;
	Json::Value root;

	if (!request.getHeaderKeyValue("Authorization").empty()) {
		fbLogin(request, response);
	} else {

		if (!reader.parse(data.c_str(), root)) {
			fillResponse(response, 401);
			response["error"] = reader.getFormattedErrorMessages();
			return;
		}
		string email = root.get("email", "").asString();
		string pass = root.get("password", "").asString();
		string token = generateToken(email, pass);
		jobifyCredentials credentials(email, pass, token);

		dbCredentials dbCred;
		dbCred.connect("./accounts");
		string error;

		if (dbCred.verifyLogin(credentials, error)) {
			dbUsers dbuser;
			dbuser.connect("./usersdb");
			Json::Value jsonProfile = dbuser.getProfile(email);
			Profile profile(jsonProfile);
			dbuser.CloseDB();
			dbCred.CloseDB();
			if (error == "") {
				fillResponse(response, 200);
				response["token"] = token;
				Json::Value userJson= profile.publicProfileToJSON();
				expandAttributes(userJson);
				response["user"] = userJson;
				return;
			}
		}
		dbCred.CloseDB();
		fillResponse(response, 401);
		response["error"] = error;
	}
}
/*
 void AccountController::getFacebookData(Request &request,
 JsonResponse &response) {
 Json::Reader reader;
 std::string data = request.getData();
 Json::Value root;
 "header: " << request.getHeaderKeyValue("Authorization") << endl;
 string token = root["token"].asString();
 string fbid = root["fbid"].asString();
 CurlWrapper ss;
 ss.handleGet(
 "https://graph.facebook.com/v2.8/" + fbid
 + "?fields=about,birthday,email,first_name,gender,last_name,location",
 request, response);
 string locationId = response["location"]["id"].asString();
 ss.handleGet(
 "https://graph.facebook.com/v2.8/" + locationId
 + "?fields=location", request, response);
 }*/

string AccountController::generateToken(const string &email,
		const string &password) const {
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
	//addRouteResponse("GET", "/fbdata", AccountController, getFacebookData, JsonResponse);
}

