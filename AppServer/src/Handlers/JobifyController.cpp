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
	//cout<<"currentRequest: "<<currentRequest<<endl;
    Response *response = NULL;

    map<string, RequestHandlerBase *>::iterator it;
    for (it = routes.begin(); it != routes.end(); it++) {
        string key = it->first;
	//	cout<<"key: "<<key<<endl;

        string regexKey = replaceRouteParams(key);
	//	cout<<"regexKey: "<<regexKey<<endl;

        if (regex_match(currentRequest, regex(regexKey))) {

	//	cout<<"adentro: "<<endl;
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

string JobifyController::replaceRouteParams(string key) const {

    string replacedKey = key;
    unsigned long firstPos = replacedKey.find("{");
    unsigned long secondPos = replacedKey.find("}");

    //Replace {param} with .*
    while (firstPos != string::npos && secondPos != string::npos) {
        replacedKey = replacedKey.replace(firstPos, secondPos - firstPos + 1, "\[^/]*");
        firstPos = replacedKey.find("{");
        secondPos = replacedKey.find("}");
    }
//	replacedKey = replacedKey+"\[^/]";
		cout<<"replacedKey: "<<replacedKey<<endl;
    return replacedKey;
}


string  JobifyController::requestToJson(Request &request, Json::Value root) {
	Json::Reader reader;

	std::string data = request.getData();

	string error = "";

	bool parsingSuccessful = reader.parse(data.c_str(), root); //parse process
	if (!parsingSuccessful) {
		error = "Failed to parse" + reader.getFormattedErrorMessages();
	}
	return error;
}
