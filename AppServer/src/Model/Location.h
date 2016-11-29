#ifndef LOCATION_H
#define LOCATION_H

#include <unistd.h>
#include <stdlib.h>

using namespace std;

class Location {

private:
	double latitude;

	double longitude;

	bool valid;

public:
	Location(double &lat, double &lon);

	Location();

	double distanceTo(Location l);

	void setLatitude(double &lat);

	double getLatitude();

	void setLongitude(double &l);

	double getLongitude();

	bool isValid();

	void setValid(bool value);

	~Location();
};

#endif //LOCATION_H
