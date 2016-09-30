#ifndef JOBIFYCONTROLLER_H
#define JOBIFYCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/JsonController.h>

using namespace std;
using namespace Mongoose;

class JobifyController: public JsonController {

protected:

    map<string,string> *routeParams;

public:
 
	virtual Response *process(Request &request) override;

	virtual bool handles(string method, string url) override;

	JobifyController();

	int jsonContainsValue(const Json::Value& root, string key, string value);


	void fillResponse(JsonResponse &response, JsonResponse &jResponse, int code);


	void registerUser(Request &request, JsonResponse &response);

	void login(Request &request, JsonResponse &response);

	void setup();

	void getJobPositions(Request &request, JsonResponse &response);

	void printDB(Request &request, JsonResponse &response);

	void editProfile(Request &request, JsonResponse &response);
private:
	string generateToken(const string &email, const string &password) const;

	string replaceRouteParams(string key) const;

    void parseRouteParams(const string &key, const string &currentRequest) const;
};

#endif 
