#ifndef SRC_MODEL_CATEGORY_H_
#define SRC_MODEL_CATEGORY_H_

#include <string>
#include <jsoncpp/json/json.h>

class Category {
public:
	Category(std::string name, std::string description);
	~Category();
	Category();
	Json::Value toJson();
private:
	std::string name;
	std::string description;
};

#endif /* SRC_MODEL_CATEGORY_H_ */
