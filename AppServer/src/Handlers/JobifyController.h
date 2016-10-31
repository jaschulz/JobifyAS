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

	string replaceRouteParams(string key) const;

	void parseRouteParams(const string &key, const string &currentRequest) const;

public:
	
	JobifyController();
 
	virtual Response *process(Request &request) override;

	virtual bool handles(string method, string url) override;

	//void fillResponse(JsonResponse &response, JsonResponse &jResponse, int code);

	void registerUser(Request &request, JsonResponse &response);

	string  requestToJson(Request &request, Json::Value &root);

/*	void login(Request &request, JsonResponse &response);

	void setup();

	void getJobPositions(Request &request, JsonResponse &response);

	void printDB(Request &request, JsonResponse &response);

	void editProfile(Request &request, JsonResponse &response);

	void getProfile(Request &request, JsonResponse &response);

	void setLocation(Request &request, JsonResponse &response);
*/};

#endif 
