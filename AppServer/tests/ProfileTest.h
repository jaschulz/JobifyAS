#ifndef APPSERVER_PROFILETEST_H
#define APPSERVER_PROFILETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../src/Model/Profile.h"

using namespace CPPUNIT_NS;

class ProfileTest : public TestFixture {
CPPUNIT_TEST_SUITE(ProfileTest);
        CPPUNIT_TEST(getEmail);
        CPPUNIT_TEST(profileToJSON);
        CPPUNIT_TEST(addRecommendation);
        CPPUNIT_TEST(manageInvitations);
        CPPUNIT_TEST(verifySkills);
        CPPUNIT_TEST(correctProfileConversion);
    CPPUNIT_TEST_SUITE_END();

public:
    ProfileTest();

    void getEmail();

    void profileToJSON();

    void addRecommendation();

    void manageInvitations();

    void verifySkills();

    void correctProfileConversion();

    virtual void setUp() override;

    virtual void tearDown() override;

    virtual ~ProfileTest();
};


#endif
