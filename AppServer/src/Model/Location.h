#ifndef LOCATION_H
#define LOCATION_H

#include <unistd.h>
#include <stdlib.h>

using namespace std;

class Location{

private:
	double latitude;

	double longitude;

public:
	Location(const double &lat, const double &lon);

	Location();

	double distanceTo(Location l);

	void setLatitude(const double &lat);

	double getLatitude();

	void setLongitude(const double &l);

	double getLongitude();

	~Location();
};


#endif //APPSERVER_USER_H
