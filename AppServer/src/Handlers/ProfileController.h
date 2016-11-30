#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <list>
#include "../Model/Entity.h"
#include "../Model/ExpMin.h"
#include "../Model/Category.h"
#include "JobifyController.h"

using namespace std;
using namespace Mongoose;

class ProfileController: public JobifyController {

public:

	ProfileController();

	void setup();

	//void printDB(Request &request, JsonResponse &response);

	bool validateCategory(Entity entity,
			std::map<string, Category> CategoriesMap);

	bool validateSkills(std::map<string, Category> CategoriesMap,
			Json::Value skillsJson, std::string &error);

	bool validateExperience(std::map<string, Category> CategoriesMap,
			Json::Value expJson, std::string &error);

	bool validateJP(std::map<string, Category> CategoriesMap,
			Json::Value jpJson, std::string &error);

	bool validateInput(Json::Value Body, std::string &error);

	void normalizeJsonProfile(Json::Value &jsonProfile);

	void editProfile(Request &request, JsonResponse &response);

	void getProfile(Request &request, JsonResponse &response);

	void setLocation(Request &request, JsonResponse &response);

	void addContact(Request &request, JsonResponse &response);

	void getContacts(Request &request, JsonResponse &response);

	void filterUsers(Request &request, JsonResponse &response);

	void rankUsers(Request &request, JsonResponse &response);

	void acceptInvitation(Request &request, JsonResponse &response);

	void recommendUser(Request &request, JsonResponse &response);
};

#endif 
