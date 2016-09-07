#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "MyController.h"

using namespace std;
using namespace Mongoose;

		MyController::MyController(){};
        MyController::~MyController(){};

        void MyController::hello(Request &request, JsonResponse &response)
        {
            int i;

            for (i=0; i<12; i++) {
                response["users"][i]["Name"] = "Bob";
            }

            response["timestamp"] = (int)time(NULL);
        }

	void MyController::goodbye(Request &request, JsonResponse &response)
        {
            response["greetings"] = "Bye";

            response["timestamp"] = (int)time(NULL);
        }

	void MyController::login(Request &request, JsonResponse &response)
	{
		//Json::Value users;
		Json::Value user1;
		Json::Value user2;
		user1["login"] = "locolope";
		user1["pass"] = "123456";
		user2["login"] = "lima";
		user2["pass"] = "qwerty";

		const Json::Value body = request.getData();

	    	/*string username = body.get("login", "").asString();
	    	string password = body.get("pass", "").asString();

		

		response["result"].append(username);*/
		response["result"].append(body);
	}
 
        void MyController::setup()
        {
            // Example of prefix, putting all the urls into "/api"
            setPrefix("/api");

            // Hello demo
            addRouteResponse("GET", "/hello", MyController, hello, JsonResponse);
            addRouteResponse("GET", "/bye", MyController, goodbye, JsonResponse);
            addRouteResponse("GET", "/login", MyController, login, JsonResponse);
        }
};


//fin
