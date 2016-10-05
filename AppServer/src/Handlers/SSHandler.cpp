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


/*JsonResponse	SSHandler::handlePut(string url, Request &request){
  CURL *curl;
  CURLcode res;
 
  /* In windows, this will init the winsock stuff */ 
  //curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
 // curl = curl_easy_init();
 // if(curl) {
    /* we want to use our own read function */ 
//    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
 
    /* enable uploading */ 
  //  curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    /* HTTP PUT please */ 
  //  curl_easy_setopt(curl, CURLOPT_PUT, 1L);
 
    /* specify target URL, and note that this URL should include a file
       name, not only a directory */ 
   /* curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");


    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
 
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_struct); /* data goes here */
    /* now specify which file to upload */ 
    //curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
 
    /* provide the size of the upload, we specicially typecast the value
       to curl_off_t since we must be sure to use the correct data size */ 
    //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)file_info.st_size);
 
    /* Now run off and do what you've been told! */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
   /* curl_easy_cleanup(curl);
  }

 
  curl_global_cleanup();
  return 0;
}*/
