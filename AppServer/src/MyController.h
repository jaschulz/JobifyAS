#ifndef _MYCONTROLLER_H
#define _MYCONTROLLER_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/JsonController.h>
#include "MyController.h"

using namespace std;
using namespace Mongoose;
class MyController : public JsonController
{
    public: 

	MyController();
        ~MyController();
        void hello(Request &request, JsonResponse &response);

	void goodbye(Request &request, JsonResponse &response);

	void login(Request &request, JsonResponse &response);

        void setup();
};


#endif
