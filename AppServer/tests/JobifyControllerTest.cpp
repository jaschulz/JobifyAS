#include "JobifyControllerTest.h"


JobifyControllerTest::JobifyControllerTest() {
}

Request JobifyControllerTest::generateRequest(string method, string queryString) const {


    mg_connection connection;
    //Only mock data, method it´s not important
    connection.request_method = method.c_str();
    connection.num_headers = 0;
    char content[] = "mock";
    connection.content = content;
    connection.content_len = 4;
    connection.uri = "http://localhost:8081/api/users";

    connection.query_string = queryString.c_str();

    Request request(&connection);
    return request;
}


Request JobifyControllerTest::makeDummyRequest(string method, string queryString) const {
    const Request &request = generateRequest(method, queryString);
    return request;
}

Request JobifyControllerTest::makeDummyBodyRequest(string data, string method) const {
    Request request = generateRequest(method, "");
    Json::Reader reader;
    Json::Value body;
    reader.parse(data, body);
    request.setBody(body);
    request.setData(data);
    return request;
}


JobifyControllerTest::~JobifyControllerTest() {

}



