#include "CredentialsTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CredentialsTest);

CredentialsTest::CredentialsTest() {
}

void CredentialsTest::setUp() {
}

void CredentialsTest::getEmail() {
	const string mail = "prueba@mail.com";
	jobifyCredentials credential(mail,"aPassword","fakeToken");
	CPPUNIT_ASSERT_EQUAL(mail, credential.getEmail());
}

void CredentialsTest::checkPassword(){
	const string password = utils::sha256("123456");
	jobifyCredentials credential("prueba@mail.com","123456","fakeToken");
	CPPUNIT_ASSERT_EQUAL(password, credential.getPassword());

}

void CredentialsTest::validateToken(){
	const string mail = "prueba@mail.com";
	const string token = "fakeToken";
	jobifyCredentials credential(mail,"123456",token);
	dbCredentials dbCont;
	dbCont.connect("./accounts");
	string error;
	dbCont.addNewUser(credential.toJSON(),error);
	bool validToken = dbCont.isValidToken(mail, token);
	dbCont.CloseDB();
	CPPUNIT_ASSERT(validToken);
}

void CredentialsTest::tearDown() {

}

CredentialsTest::~CredentialsTest() {

}
