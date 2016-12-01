#ifndef SRC_MODEL_ENTITY_H_
#define SRC_MODEL_ENTITY_H_

#include <string>
#include <jsoncpp/json/json.h>

class Entity {
public:
	Entity(std::string name, std::string description, std::string category);
	~Entity();
	Entity();
	std::string getName() const;
	std::string getCategory() const;
	std::string getDescription();
	Json::Value toJson();
	bool operator ==(const Entity &e) const;

private:
	std::string name;
	std::string description;
	std::string category;
};

#endif /* SRC_MODEL_ENTITY_H_ */
