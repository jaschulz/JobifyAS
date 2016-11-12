#include "LocationTest.h"
#include "../src/utils/utils.h"

CPPUNIT_TEST_SUITE_REGISTRATION(LocationTest);

LocationTest::LocationTest() {
}

void LocationTest::setUp() {
}


/*
1.76*/
void LocationTest::checkDistance(){
	
	Location source(-34.7035466,-58.3930756);
	Location destination(-34.703450,-58.385126);
	//results taken from http://andrew.hedges.name/experiments/haversine/
	CPPUNIT_ASSERT(0.727 - source.distanceTo(destination) < 0.01);//
	source.setLatitude(38.898556);
	source.setLongitude(-77.037852);
	destination.setLatitude(38.897147);
	destination.setLongitude(-77.043934);
	CPPUNIT_ASSERT(0.549 - source.distanceTo(destination) < 0.01);//
}

void LocationTest::tearDown() {

}

LocationTest::~LocationTest() {

}
