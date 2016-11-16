#ifndef FBHANDLER_H
#define FBHANDLER_H

#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <mongoose/Request.h>
#include <mongoose/JsonResponse.h>

using namespace std;
using namespace Mongoose;

class FBHandler {
public:
	JsonResponse  getBasicData(Request &request);

	JsonResponse  getData(Request &request);

	JsonResponse  getLocationData(Request &request, const string &locationid);
};

#endif 
