#include "Experience.h"

Experience::Experience(std::string name, std::string description,
		std::string category, std::string where) :
		job_position(name, description, category) {
	this->where = where;
}

Experience::~Experience() {
	job_position.~Entity();
}

Json::Value Experience::toJson() {
	Json::Value jsonExp;
	jsonExp["where"] = this->where;
	jsonExp["job_position"] = job_position.toJson();
	return jsonExp;
}

std::string Experience::getWhere() {
	return where;
}

std::string Experience::getJPName() {
	return job_position.getName();
}

