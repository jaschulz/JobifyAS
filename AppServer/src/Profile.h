#ifndef PROFILE_H
#define PROFILE_H

#include <unistd.h>
#include <stdlib.h>
#include <json/json.h>

using namespace std;

class Profile{

private:
    string email;
    string password;
Json::Value jsonProfile;

public:
    Profile(const Json::Value &value);

    Profile(const string &mail, const string &pass);

    void setEmail(const string &mail);

    const string &getEmail() const;

    const string &getPassword() const;

    void setPassword(const string &pass);

    Json::Value &profileToJSON();

   ~Profile();

   void fromJSON(Json::Value value);
};


#endif //APPSERVER_USER_H
