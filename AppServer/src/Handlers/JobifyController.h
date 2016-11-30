#ifndef JOBIFYCONTROLLER_H
#define JOBIFYCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/JsonController.h>
#include <mongoose/Response.h>
#include "CurlWrapper.h"

using namespace std;
using namespace Mongoose;

class JobifyController: public JsonController {

protected:

	map<string, string> *routeParams;

	string replaceRouteParams(string key) const;

	void parseRouteParams(const string &key,
			const string &currentRequest) const;

public:

	JobifyController();

	~JobifyController();

	void expandAttributes(Json::Value &userJson);

	Json::Value expandJP(std::string jpJson);

	Json::Value expandExperiences(Json::Value expJson);

	Json::Value expandSkills(Json::Value skillsJson);


	Json::Value expandUsers(Json::Value usersJson);

	Json::Value expandContacts(Json::Value contactsJson);

	virtual Response *process(Request &request) override;

	virtual bool handles(string method, string url) override;

	void fillResponse(JsonResponse &response, int code);

	void registerUser(Request &request, JsonResponse &response);

	string requestToJson(Request &request, Json::Value &root);

	bool isValidToken(string email, string token);
};

#endif 
