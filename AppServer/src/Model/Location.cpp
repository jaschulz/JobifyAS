#include "Location.h"
#include "../utils/utils.h"
#include <openssl/sha.h>
#include <string>
#include <iostream>
#include <cmath> 
#define earthRadiusKm 6371.0

Location::Location(double &lat, double &lon) {
	this->latitude = lat;
	this->longitude = lon;
	cout << "valid = true" << endl;
	valid = true;
}

Location::Location() {
	valid = false;
	cout << "valid = false" << endl;
}

Location::~Location() {
}

/*
 * original source:
 * http://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a
 */
double Location::distanceTo(Location l) {
	double lat1r, lon1r, lat2r, lon2r, u, v;
	lat1r = utils::deg2rad(latitude);
	lon1r = utils::deg2rad(longitude);
	lat2r = utils::deg2rad(l.getLatitude());
	lon2r = utils::deg2rad(l.getLongitude());
	u = sin((lat2r - lat1r) / 2);
	v = sin((lon2r - lon1r) / 2);
	return 2.0 * earthRadiusKm
			* asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

void Location::setLatitude(double &lat) {
	latitude = lat;
}

double Location::getLatitude() {
	return latitude;
}

void Location::setLongitude(double &l) {
	longitude = l;
}

double Location::getLongitude() {
	return longitude;
}

bool Location::isValid() {
	return valid;
}

void Location::setValid(bool value) {
	valid = value;
}
