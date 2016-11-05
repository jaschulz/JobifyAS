#include <unistd.h>
#include <stdlib.h>
//#include <mongoose/JsonResponse.h>
#include "SSHandler.h"
#include <curl/curl.h>
#include <stdio.h>
	

size_t SSHandler::writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

void	SSHandler::handleGet(string url, JsonResponse &response){
	CURL * curl_handle;
	curl_handle = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);

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

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}

void	SSHandler::handlePost(string url, Request &request, JsonResponse &response){
	CURLcode ret;
	CURL *hnd;
	struct curl_slist *slist1;
	std::string jsonstr = request.getData();

	slist1 = NULL;
	slist1 = curl_slist_append(slist1, "Content-Type: application/json");

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

	ret = curl_easy_perform(hnd);
	if (ret != CURLE_OK) {
		response["Error"] = curl_easy_strerror(ret);
	} else {
		Json::Reader reader2;
		bool parsingSuccessful = reader2.parse(response_string.c_str(), response); //parse process
		if (!parsingSuccessful) {
			response["error"] = reader2.getFormattedErrorMessages();
		}
	}

	curl_easy_cleanup(hnd);
	hnd = NULL;
	curl_slist_free_all(slist1);
	slist1 = NULL; 

}

void	SSHandler::handleDelete(string url, Request &request, JsonResponse &response){
	CURLcode ret;
	CURL *hnd;
	struct curl_slist *slist1;
		std::cout << url << endl;
	slist1 = NULL;
	slist1 = curl_slist_append(slist1, "Content-Type: application/json");

	hnd = curl_easy_init();

	std::string response_string;
	curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response_string);
	curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
	curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "DELETE");
	curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

	ret = curl_easy_perform(hnd);
	long http_code = 0;
	curl_easy_getinfo(hnd,CURLINFO_RESPONSE_CODE,&http_code);
	if (ret != CURLE_OK) {
		response["Error"] = curl_easy_strerror(ret);
	} else {
		Json::Reader reader2;
		if (http_code!=204) {
			bool parsingSuccessful = reader2.parse(response_string.c_str(), response); //parse process
			if (!parsingSuccessful) {
				response["error"] = reader2.getFormattedErrorMessages();
			}
		}
	}
	curl_easy_cleanup(hnd);
	hnd = NULL;
	curl_slist_free_all(slist1);
	slist1 = NULL; 
}


void SSHandler::handlePut(string url, Request &request, JsonResponse &response){
	CURL * curl_handle;
	curl_handle = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);

	
	struct curl_slist *slist1;
	std::string jsonstr = request.getData();

	slist1 = NULL;
	slist1 = curl_slist_append(slist1, "Content-Type: application/json");

	curl_easy_setopt(curl_handle, CURLOPT_URL,url.c_str());

	curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "PUT");

	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, jsonstr.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "curl/7.38.0");
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
	curl_easy_cleanup(curl_handle);
	curl_handle = NULL;
	curl_slist_free_all(slist1);
	slist1 = NULL; 
}
