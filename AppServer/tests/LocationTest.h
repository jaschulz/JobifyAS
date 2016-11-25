#ifndef APPSERVER_LOCATIONTEST_H
#define APPSERVER_LOCATIONTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../src/Model/Location.h"

using namespace CPPUNIT_NS;

class LocationTest: public TestFixture {
	CPPUNIT_TEST_SUITE (LocationTest);
	CPPUNIT_TEST (checkDistance);CPPUNIT_TEST_SUITE_END();

public:
	LocationTest();

	void checkDistance();

	virtual void setUp() override;

	virtual void tearDown() override;

	virtual ~LocationTest();

};

#endif
