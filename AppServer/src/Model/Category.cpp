/*
 * Category.cpp
 *
 *  Created on: 27/11/2016
 *      Author: javier
 */

#include "Category.h"

Category::Category(std::string name, std::string desc) {
	this->name = name;
	this->description = desc;
}

Category::~Category() {
}

Category::Category() {
}

Json::Value Category::toJson() {
	Json::Value category;
	category["name"] = this->name;
	category["description"] = this->description;
	return category;
}
