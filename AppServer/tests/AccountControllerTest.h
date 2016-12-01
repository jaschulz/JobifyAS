#ifndef ACCOUNTCONTROLLERTEST_H
#define ACCOUNTCONTROLLERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../src/Handlers/AccountController.h"
#include "JobifyControllerTest.h"
#include <Handlers/CurlWrapper.h>

using namespace CPPUNIT_NS;

class AccountControllerTest: public TestFixture, JobifyControllerTest {
	CPPUNIT_TEST_SUITE (AccountControllerTest);
	CPPUNIT_TEST (successfulLogin);CPPUNIT_TEST_SUITE_END();

private:
	AccountController accountController;

public:
	AccountControllerTest();

	virtual void setUp();

	void successfulLogin();

	virtual void tearDown();

	virtual ~AccountControllerTest();

};

#endif
