#ifndef JOBIFYCONTROLLER_H
#define JOBIFYCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/JsonController.h>

using namespace std;
using namespace Mongoose;

class JobifyController: public JsonController {
public:
	int jsonContainsValue(const Json::Value& root, string key, string value);


	void fillResponse(JsonResponse &response, JsonResponse &jResponse, int code);


	void registerUser(Request &request, JsonResponse &response);

	void login(Request &request, JsonResponse &response);

	void setup();

	void getJobPositions(Request &request, JsonResponse &response);

	void printDB(Request &request, JsonResponse &response);
private:
	string generateToken(const string &email, const string &password) const;
};

#endif 
