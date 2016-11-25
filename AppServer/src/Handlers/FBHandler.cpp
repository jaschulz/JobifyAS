#include <unistd.h>
#include <stdlib.h>
#include "FBHandler.h"
#include "CurlWrapper.h"
#include "../Model/fbCredentials.h"
#include <stdio.h>

JsonResponse FBHandler::getBasicData(Request &request) {
	JsonResponse response;
	CurlWrapper cw;
	//cw.handleGet("https://graph.facebook.com/v2.8/me?fields=about,birthday,email,first_name,gender,last_name,location", request, response);	
	cw.handleGet("https://graph.facebook.com/v2.8/me?fields=email", request,
			response);
	return response;
}

JsonResponse FBHandler::getData(Request &request) {
	JsonResponse response;
	CurlWrapper cw;
	cw.handleGet(
			"https://graph.facebook.com/v2.8/me?fields=email,first_name,last_name,location",
			request, response);
	return response;
}

JsonResponse FBHandler::getLocationData(Request &request,
		const string &locationid) {
	JsonResponse response;
	CurlWrapper cw;
	cw.handleGet(
			"https://graph.facebook.com/v2.8/" + locationid
					+ "?fields=location", request, response);
	return response;
}

