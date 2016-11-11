#ifndef SSCONTROLLER_H
#define SSCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "JobifyController.h"
#include "../Model/Profile.h"

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

	void addJobPositions(Request &request, JsonResponse &response);

	void getCategories(Request &request, JsonResponse &response);

	void getSkills(Request &request, JsonResponse &response);

	void addSkills(Request &request, JsonResponse &response);

	void addCategory(Request &request, JsonResponse &response);

	void filterJobPositionsByCategory(Request &request, JsonResponse &response);

	void filterSkillsByCategory(Request &request, JsonResponse &response);

	void setup();

};

#endif 
