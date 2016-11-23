#ifndef UTILS_H
#define UTILS_H
#include <string>	
#include <jsoncpp/json/json.h>


class utils
{
public:
static std::string sha256(const std::string str);

static double deg2rad(double deg);

static double rad2deg(double rad);

static bool jsonContainsValue(Json::Value &json, std::string &valueToFind);

static void jsonArrayToVector(Json::Value array, std::vector<std::string> &v);

};

#endif
