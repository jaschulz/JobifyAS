#ifndef SRC_MODEL_EXPERIENCE_H_
#define SRC_MODEL_EXPERIENCE_H_

#include <string>
#include <jsoncpp/json/json.h>
#include "Entity.h"

class Experience {
public:
	Experience(std::string name, std::string description, std::string category,
			std::string where);
	~Experience();
	Json::Value toJson();
	std::string getWhere();
	std::string getJPName();

private:
	Entity job_position;
	std::string where;
};

#endif /* SRC_MODEL_EXPERIENCE_H_ */
