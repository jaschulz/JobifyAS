#include <unistd.h>
#include <stdlib.h>
//#include <mongoose/JsonResponse.h>
#include "SSHandler.h"

string data; //will hold the url's contents


size_t write_data(char* buf, size_t size, size_t nmemb, void* up) { //callback must have this declaration

//buf is a pointer to the data that curl has for us
																	//size*nmemb is the size of the buffer

	for (int c = 0; c < size * nmemb; c++) {
		data.push_back(buf[c]);
	}
	return size * nmemb; //tell curl how many bytes we handled
}


JsonResponse	SSHandler::handleGet(string url){
	JsonResponse response;
	CURL * curl_handle;
	curl_handle = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* set URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL,url.c_str());

	/* send all data to this function  */

	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
	CURLcode res = curl_easy_perform(curl_handle);
	if (res != CURLE_OK) {
		response["Error"] = curl_easy_strerror(res);

	} else {
		Json::Value root;

		Json::Reader reader2;
		bool parsingSuccessful = reader2.parse(data.c_str(), root); //parse process
		if (!parsingSuccessful) {
			std::cout << "Failed to parse"
					<< reader2.getFormattedErrorMessages();

		}
		response["result"] = root;
	}
	curl_easy_cleanup(curl_handle);
	return response;

}


