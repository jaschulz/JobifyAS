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
	void handleGet(string url,JsonResponse &response, std::string token);

private:
	static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
};

#endif 
