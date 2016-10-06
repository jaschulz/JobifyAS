#include <unistd.h>
#include <stdlib.h>
//#include <mongoose/JsonResponse.h>
#include "SSHandler.h"

#include <stdio.h>

string data; //will hold the url's contents

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

void	SSHandler::handleGet(string url, JsonResponse &response){
	CURL * curl_handle;
	curl_handle = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);

	
	curl_handle = curl_easy_init();

	
	curl_easy_setopt(curl_handle, CURLOPT_URL,url.c_str());

	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
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


JsonResponse	SSHandler::handlePost(string url, Request &request, JsonResponse &response){
CURL *curl;
  CURLcode res;
 

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) {

	
	std::string jsonstr = request.getData();

	        std::string response_string;
 
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
 
    /* send all data to this function  */ 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
 
    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
 
    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */ 
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());
 
    /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
       itself */ 
//    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(jsonstr.c_str()));
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    else {std::cout <<"else" << endl;
		Json::Reader reader2;

		bool parsingSuccessful = reader2.parse(response_string.c_str(), response); //parse process
		if (!parsingSuccessful) {
			response["error"] = reader2.getFormattedErrorMessages();
		}
    }
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 

 
    /* we're done with libcurl, so clean it up */ 
    curl_global_cleanup();
  }

/*	CURLcode ret;
	CURL *hnd;
	struct curl_slist *slist1;
	std::string jsonstr = request.getData();

	slist1 = NULL;
	slist1 = curl_slist_append(slist1, "Content-Type: application/json");

				std::cout <<"1" << endl;
	hnd = curl_easy_init();

	        std::string response_string;
	curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response_string);
	curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, jsonstr.c_str());
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
	curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);


	std::cout <<"2" << endl;
	ret = curl_easy_perform(hnd);
	if (ret != CURLE_OK) {
	std::cout <<"if" << endl;
		response["Error"] = curl_easy_strerror(ret);
	std::cout <<"if2" << endl;
	} else {
			std::cout <<"else" << endl;
		Json::Reader reader2;

		bool parsingSuccessful = reader2.parse(response_string.c_str(), response); //parse process
		if (!parsingSuccessful) {
			response["error"] = reader2.getFormattedErrorMessages();
		}
	}

	curl_easy_cleanup(hnd);
	hnd = NULL;
	curl_slist_free_all(slist1);
	slist1 = NULL; */
}
