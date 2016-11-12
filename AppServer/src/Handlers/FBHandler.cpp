#include <unistd.h>
#include <stdlib.h>
#include "FBHandler.h"
#include <curl/curl.h>
#include <stdio.h>
	

size_t FBHandler::writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

void	FBHandler::handleGet(string url, JsonResponse &response, std::string token ){
	CURL * curl_handle;
	curl_handle = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);


	struct curl_slist *slist1;

	std::string auth = "Authorization: OAuth " + token;

	slist1 = NULL;
	//slist1 = curl_slist_append(slist1, "Content-Type: application/json");	
	cout<<"auth"<<auth<<endl;
	slist1 = curl_slist_append(slist1, auth.c_str());

	curl_easy_setopt(curl_handle, CURLOPT_URL,url.c_str());

	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist1);
        std::string response_string;
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_string);
	CURLcode res = curl_easy_perform(curl_handle);
	if (res != CURLE_OK) {
		response["Error"] = curl_easy_strerror(res);

	} else {
	
		Json::Reader reader2;
		bool parsingSuccessful = reader2.parse(response_string.c_str(), response); //parse process
		if (!parsingSuccessful) {
			response["error"] = reader2.getFormattedErrorMessages();

		}
	}
}


