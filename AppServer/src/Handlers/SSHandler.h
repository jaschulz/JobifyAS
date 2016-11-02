#ifndef SSHANDLER_H
#define SSHANDLER_H

#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <mongoose/Request.h>
#include <mongoose/JsonResponse.h>

using namespace std;
using namespace Mongoose;

class SSHandler {
public:
	void handleGet(string url,JsonResponse &response);

	void handlePost(string url, Request &request, JsonResponse &response);

	void handleDelete(string url, Request &request, JsonResponse &response);

	void handlePut(string url, Request &request, JsonResponse &response);

private:
	static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
};

#endif 
