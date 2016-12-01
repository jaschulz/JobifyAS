#include "Entity.h"
#include <unistd.h>

#include <iostream>

#include <unistd.h>
#include <set>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

Entity::Entity(std::string name, std::string description,
		std::string category) {
	this->name = name;
	this->description = description;
	this->category = category;
}

Entity::~Entity() {
}

Entity::Entity() {
}

std::string Entity::getName() const {
	return name;
}

std::string Entity::getCategory() const {
	return category;
}

std::string Entity::getDescription() {
	return description;
}

Json::Value Entity::toJson() {
	Json::Value jsonEntity;
	std::cout << "name: " << this->name << std::endl;
	jsonEntity["name"] = this->name;
	std::cout << "category: " << this->category << std::endl;
	jsonEntity["category"] = this->category;
	std::cout << "description: " << this->description << std::endl;
	jsonEntity["description"] = this->description;
	std::cout << "creado ok" << std::endl;
	return jsonEntity;
}

bool Entity::operator ==(const Entity &e) const {
	return (category.compare(e.getCategory()) == 0
			&& name.compare(e.getName()) == 0);
}
