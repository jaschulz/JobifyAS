#include "utils.h"
#include <openssl/sha.h>
#include <sstream>
#include <iostream>
#include <iomanip>

#define pi 3.14159265358979323846

using namespace std;

std::string utils::sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// This function converts decimal degrees to radians
double utils::deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
/*double utils::rad2deg(double rad) {
  return (rad * 180 / pi);
}*/

bool utils::jsonContainsValue(Json::Value &json, std::string &valueToFind) {
	for(Json::Value::iterator it = json.begin(); it !=json.end(); ++it){
		Json::Value keyValue = it.key();
		Json::Value value = (*it);
		if(value.asString() == valueToFind){
			return true;
		}
	} 
	return false;
}

static void jsonArrayToVector(Json::Value array, std::vector<std::string> &v) {
	for(Json::Value::iterator it = array.begin(); it !=array.end(); ++it){
		Json::Value keyValue = it.key();
		Json::Value value = (*it);
		v.push_back(value.asString());
	} 
}
