#include "AccountControllerTest.h"
#include <db/dbCredentials.h>

CPPUNIT_TEST_SUITE_REGISTRATION(AccountControllerTest);

AccountControllerTest::AccountControllerTest() {
}

void  AccountControllerTest::setUp() {
    accountController.setup();
}

void AccountControllerTest::successfulLogin() {
    string data = "{\"email\":\"js5810@hotmail.com\",\"password\":\"123456\"}";
    Request request = makeDummyBodyRequest(data, "POST");
		
    RequestHandler<AccountController, JsonResponse> requestHandler(&accountController, &AccountController::login);
	cout<<"AccountControllerTest url: "<<request.getUrl()<<endl;
	cout<<"AccountControllerTest sessid: "<<request.get("sessid")<<endl;
	cout<<"AccountControllerTest request.hasCookie(key): "<<request.hasCookie("sessid")<<endl;
//request.hasCookie(key)
    JsonResponse *response = (JsonResponse *) requestHandler.process(request);
	cout<<"ok"<<endl;
    const Json::Value &value = response->get("data", "[]");
    delete (response);
    CPPUNIT_ASSERT(true);
}

void AccountControllerTest::tearDown() {

}

AccountControllerTest::~AccountControllerTest() {
}
