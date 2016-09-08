#include <unistd.h>
#include <stdlib.h>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>
#include "JobifyController.h"
#include <curl/curl.h>




	string data; //will hold the url's contents

	int JobifyController::jsonContainsValue(const Json::Value& root, string key, string value) {
		const Json::Value& users = root["users"];
		for (Json::ValueConstIterator it = users.begin(); it != users.end();
				++it) {
			const Json::Value& user = *it;
			if (value.compare(user[key].asString()) == 0)
			{
				return 0;
			}
		}
		return 1;
	}


	void JobifyController::fillResponse(JsonResponse &response, JsonResponse &jResponse, int code) {
		response.setCode(code);
		response["data"] = jResponse;
		response.setHeader("Content-Type", "application/json; charset=utf-8");
	}


	void JobifyController::registerUser(Request &request, JsonResponse &response) {

		Json::Value users;
		Json::Reader reader;

		std::ifstream file("users.json", std::ifstream::binary);
		bool parsingSuccessful2 = reader.parse(file, users, true); //parse process
		if (!parsingSuccessful2) {
			std::cout << "Failed to parse"
					<< reader.getFormattedErrorMessages();

		}
		std::string data = request.getData();

		Json::Value root;

		Json::Reader reader2;
		bool parsingSuccessful = reader2.parse(data.c_str(), root); //parse process
		if (!parsingSuccessful) {
			std::cout << "Failed to parse"
					<< reader2.getFormattedErrorMessages();

		}
		string username = root.get("email", "").asString();
		string password = root.get("password", "").asString();

		Json::Value user1;
		user1["email"] = username;
		user1["password"] = password;

		file.close();

		JsonResponse jResponse;

		if (jsonContainsValue(users,"email" ,username) == 1) { 
			users["users"].append(user1);
			Json::StyledStreamWriter writer;
			std::ofstream out("users.json");
			writer.write(out, users);
			out.close();
			jResponse["token"] = "ok";
			fillResponse(response,jResponse,200);
		} else {
			jResponse["error"] = "The email provided is already registered";
			fillResponse(response,jResponse,401);
		}
	}

	void JobifyController::login(Request &request, JsonResponse &response) {
		
		Json::Value users;
		Json::Reader reader;

		std::ifstream file("users.json", std::ifstream::binary);

		bool parsingSuccessful2 = reader.parse(file, users, true); //parse process
		if (!parsingSuccessful2) {
			std::cout << "Failed to parse"
					<< reader.getFormattedErrorMessages();

		}

		std::string data = request.getData();

		Json::Value root;

		Json::Reader reader2;
		bool parsingSuccessful = reader2.parse(data.c_str(), root); //parse process
		if (!parsingSuccessful) {
			std::cout << "Failed to parse"
					<< reader2.getFormattedErrorMessages();

		}
		string username =
				root.get("email", "A Default Value if not exists").asString();

		string password = root.get("password", "").asString();

		if (jsonContainsValue(users,"email", username) == 0 && jsonContainsValue(users,"password", password) == 0) {
			response["result"].append("login Ok");
		} else {
			response["result"].append("login Failed");
		}
	}

	size_t write_data(char* buf, size_t size, size_t nmemb, void* up)
	{ //callback must have this declaration
	    //buf is a pointer to the data that curl has for us
	    //size*nmemb is the size of the buffer

	    for (int c = 0; c<size*nmemb; c++)
	    {
		data.push_back(buf[c]);
	    }
	    return size*nmemb; //tell curl how many bytes we handled
	}

	void JobifyController::getJobPositions(Request &request, JsonResponse &response) {
		CURL * curl_handle;
		curl_handle = curl_easy_init();
		curl_global_init(CURL_GLOBAL_ALL);

		/* init the curl session */ 
		curl_handle = curl_easy_init();

		/* set URL to get */ 
		curl_easy_setopt(curl_handle, CURLOPT_URL, "http://localhost:8080/job_positions");

		/* send all data to this function  */ 
		
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
		CURLcode res = curl_easy_perform(curl_handle);
		if (res != CURLE_OK) {
			response["Error"] = curl_easy_strerror(res);

		} else {
			response["result"] = data;
		}
		curl_easy_cleanup(curl_handle);

		 
	}

	void JobifyController::setup() {
		// Example of prefix, putting all the urls into "/api"
		setPrefix("/api");

		addRouteResponse("GET", "/login", JobifyController, login, JsonResponse);
		addRouteResponse("POST", "/registerUser", JobifyController, registerUser,
				JsonResponse);
		addRouteResponse("GET", "/job_positions", JobifyController, getJobPositions, JsonResponse);
	}

