#include "ProfileTest.h"
#include "../src/utils/utils.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ProfileTest);

ProfileTest::ProfileTest() {
}

void ProfileTest::setUp() {
}

void ProfileTest::getEmail() {
	const string mail = "prueba@mail.com";
	Profile profile(mail);
	CPPUNIT_ASSERT_EQUAL(mail, profile.getEmail());
}

void ProfileTest::profileToJSON() {
	const string mail = "prueba@mail.com";
	Profile profile(mail);
	Json::Value jsonProfile = profile.profileToJSON();
	CPPUNIT_ASSERT_EQUAL(jsonProfile.get("email", "").asString(), profile.getEmail());
}

void ProfileTest::tearDown() {

}

ProfileTest::~ProfileTest() {

}
