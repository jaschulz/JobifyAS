#ifndef SSCONTROLLER_H
#define SSCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "JobifyController.h"
#include "../Model/Profile.h"
#include "../Model/Entity.h"
#include "../Model/Category.h"

using namespace std;
using namespace Mongoose;

class SSController: public JobifyController {

public:
	SSController();

	void deleteSkill(Request &request, JsonResponse &response);

	void deleteCategory(Request &request, JsonResponse &response);

	void modifyCategory(Request &request, JsonResponse &response);

	void modifyJobPosition(Request &request, JsonResponse &response);

	void modifySkill(Request &request, JsonResponse &response);

	void deleteJobPosition(Request &request, JsonResponse &response);

	void getJobPositions(Request &request, JsonResponse &response);

	void getJobPositions(JsonResponse &response);

	void addJobPositions(Request &request, JsonResponse &response);

	void getCategories(Request &request, JsonResponse &response);

	void getSkills(Request &request, JsonResponse &response);

	void getCategories(JsonResponse &response);

	void getSkills(JsonResponse &response);

	void addSkills(Request &request, JsonResponse &response);

	void addSkills(Json::Value newSkill, JsonResponse &response);

	void addCategory(Request &request, JsonResponse &response);

	void addCategory(Json::Value newCat, JsonResponse &response);

	void filterJobPositionsByCategory(Request &request, JsonResponse &response);

	void filterSkillsByCategory(Request &request, JsonResponse &response);

	std::map <string, Entity> getMap(Json::Value array);

	std::map <string, Entity> getSkillsMap();

	std::map <string, Entity> getJPMap();

	std::map <string, Category> getCategoriesMap();


	bool verifyInput(Json::Value value, std::string error);

	void setup();

};

#endif 
