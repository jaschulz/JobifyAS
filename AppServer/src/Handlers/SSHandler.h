#ifndef SSHANDLER_H
#define SSHANDLER_H

#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <mongoose/JsonResponse.h>

using namespace std;
using namespace Mongoose;

class SSHandler {
public:
	void	handleGet(string url,JsonResponse &response);
};

#endif 
