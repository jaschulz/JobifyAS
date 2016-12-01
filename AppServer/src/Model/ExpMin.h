#ifndef SRC_MODEL_EXPMIN_H_
#define SRC_MODEL_EXPMIN_H_

#include <string>
#include <jsoncpp/json/json.h>

class ExpMin {
public:
	ExpMin(std::string jp, std::string where);
	~ExpMin();
	std::string getWhere();
	std::string getJobPosition();
	Json::Value toJson();

private:
	std::string job_position;
	std::string where;
};

#endif /* SRC_MODEL_EXPMIN_H_ */
