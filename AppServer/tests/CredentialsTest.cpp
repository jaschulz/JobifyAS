#include "CredentialsTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CredentialsTest);

CredentialsTest::CredentialsTest() {
}

void CredentialsTest::setUp() {
	const string mail = "prueba@mail.com";
	const string token = "fakeToken";
	string error;
	jobifyCredentials credential(mail,"123456",token);
	dbCredentials dbCred;
	dbCred.connect("./testAccounts");
	dbCred.addNewUser(credential.toJSON(),error);
	dbCred.CloseDB();
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

void CredentialsTest::verifyLogin(){
	const string mail = "prueba@mail.com";
	const string token = "fakeToken";
	jobifyCredentials credential(mail,"123456",token);
	dbCredentials dbCont;
	dbCont.connect("./testAccounts");
	string error;
	bool loginOk = dbCont.verifyLogin(credential,error);
	dbCont.CloseDB();
	CPPUNIT_ASSERT(loginOk);
}

void CredentialsTest::validateToken(){
	const string mail = "prueba@mail.com";
	const string token = "fakeToken";
	jobifyCredentials credential(mail,"123456",token);
	dbCredentials dbCont;
	dbCont.connect("./testAccounts");
	CPPUNIT_ASSERT(dbCont.isValidToken(mail, token));
	CPPUNIT_ASSERT(!dbCont.isValidToken(mail, "qwerty"));
	//bool validToken = dbCont.isValidToken(mail, token);
	dbCont.CloseDB();
	//CPPUNIT_ASSERT(validToken);
}

void CredentialsTest::tearDown() {

}

CredentialsTest::~CredentialsTest() {

}
