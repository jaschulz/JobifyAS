#include "ProfileTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ProfileTest);

ProfileTest::ProfileTest() {
}

void ProfileTest::setUp() {
}

void ProfileTest::getEmail() {
	const string mail = "prueba@mail.com";
	Profile profile(mail,"aPassword","Juan","Perez");
	CPPUNIT_ASSERT_EQUAL(mail, profile.getEmail());
}

void ProfileTest::tearDown() {
//    profile.~Profile();
}

ProfileTest::~ProfileTest() {

}
