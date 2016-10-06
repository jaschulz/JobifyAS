#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "JobifyController.h"

using namespace std;
using namespace Mongoose;

class ProfileController: public JobifyController {

public:
	
	ProfileController();
 
	void setup();

	void printDB(Request &request, JsonResponse &response);

	void editProfile(Request &request, JsonResponse &response);

	void getProfile(Request &request, JsonResponse &response);

	void setLocation(Request &request, JsonResponse &response);

	void addContact(Request &request, JsonResponse &response);

	void getContacts(Request &request, JsonResponse &response);
};

#endif 
