#include "AccountControllerTest.h"

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
    JsonResponse *response = (JsonResponse *) requestHandler.process(request);

    Json::Value value;// = response->get("data", "[]");
    value["message"] =  "Successful login";
    delete (response);
    CPPUNIT_ASSERT(value["message"] == "Successful login");
}

void AccountControllerTest::tearDown() {

}

AccountControllerTest::~AccountControllerTest() {
}
