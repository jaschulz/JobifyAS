#ifndef ACCOUNTCONTROLLER_H
#define ACCOUNTCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "JobifyController.h"
#include "../Model/Profile.h"

using namespace std;
using namespace Mongoose;

class AccountController: public JobifyController {

public:
	AccountController();

	string registerUserDB(Profile & profile);

	void registerUser(Request &request, JsonResponse &response);

	void login(Request &request, JsonResponse &response);

	void printDB(Request &request, JsonResponse &response);

	void setup();
private:
	string generateToken(const string &email, const string &password) const;
};

#endif 
