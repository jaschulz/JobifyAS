#ifndef APPSERVER_CREDENTIALSTEST_H
#define APPSERVER_CREDENTIALSTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../src/Model/Credentials.h"
#include "../src/utils/utils.h"
#include "../src/db/dbCredentials.h"


using namespace CPPUNIT_NS;

class CredentialsTest : public TestFixture {
CPPUNIT_TEST_SUITE(CredentialsTest);
        CPPUNIT_TEST(getEmail);
        CPPUNIT_TEST(checkPassword);
        CPPUNIT_TEST(validateToken);
    CPPUNIT_TEST_SUITE_END();

public:
    CredentialsTest();

    void getEmail();

    void checkPassword();

    void validateToken();

    virtual void setUp() override;

    virtual void tearDown() override;

    virtual ~CredentialsTest();


};


#endif
