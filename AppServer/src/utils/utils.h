#ifndef UTILS_H
#define UTILS_H
#include <string>	
#include <jsoncpp/json/json.h>
#include <set>

class utils {
public:
	static std::string sha256(const std::string str);

	static double deg2rad(double deg);

	static double rad2deg(double rad);

	static bool jsonContainsValue(Json::Value &json, std::string &valueToFind);

	static bool setContainsValue(std::set<std::string> &v, std::string value);

	static void jsonArrayToSet(Json::Value array, std::set<std::string> &v);

	static Json::Value setToJsonArray(std::set<std::string> &v);

	static bool moveFromSetToSet(std::set<std::string> &source,
			std::set<std::string> &destination, std::string value);

};

#endif
