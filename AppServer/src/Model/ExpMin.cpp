/*
 * ExpMin.cpp
 *
 *  Created on: 29/11/2016
 *      Author: javier
 */

#include "ExpMin.h"

ExpMin::ExpMin(std::string jp, std::string where) {
	this->job_position = jp;
	this->where = where;
}

ExpMin::~ExpMin() {
}

std::string ExpMin::getWhere() {
	return where;
}

std::string ExpMin::getJobPosition() {
	return job_position;
}

Json::Value ExpMin::toJson() {
	Json::Value jsonExp;
	jsonExp["where"] = this->where;
	jsonExp["job_position"] = this->job_position;
	return jsonExp;
}
