#include "ProfileTest.h"
#include "../src/utils/Encrypt.h"

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

void ProfileTest::checkPassword(){
	const string password = encrypt::sha256("123456");
	Profile profile("prueba@mail.com","123456","Juan","Perez");
	CPPUNIT_ASSERT_EQUAL(password, profile.getPassword());

}

void ProfileTest::tearDown() {
//    profile.~Profile();
}

ProfileTest::~ProfileTest() {

}
