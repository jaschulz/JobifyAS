#include "LocationTest.h"
#include "../src/utils/utils.h"

CPPUNIT_TEST_SUITE_REGISTRATION (LocationTest);

LocationTest::LocationTest() {
}

void LocationTest::setUp() {
}

/*
 1.76*/
void LocationTest::checkDistance() {
	double lat1 = -34.7035466;
	double long1 = -58.3930756;
	double lat2 = -34.703450;
	double long2 = -58.385126;
	Location source(lat1, long1);
	Location destination(lat2, long2);
	//results taken from http://andrew.hedges.name/experiments/haversine/
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.727, source.distanceTo(destination), 0.01); //
	lat1 = 38.898556;
	long1 = -77.037852;
	lat2 = 38.897147;
	long2 = -77.043934;
	source.setLatitude(lat1);
	source.setLongitude(long1);
	destination.setLatitude(lat2);
	destination.setLongitude(long2);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.549, source.distanceTo(destination), 0.01); //
}

void LocationTest::tearDown() {

}

LocationTest::~LocationTest() {

}
