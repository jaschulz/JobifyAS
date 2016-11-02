#include "JobifyControllerTest.h"


JobifyControllerTest::JobifyControllerTest() {
}

Request JobifyControllerTest::generateRequest(string method, string queryString) const {
    mg_connection conn;
    conn.request_method = method.c_str();
    conn.num_headers = 0;
    char content[] = "mock";
    conn.content = content;
    conn.content_len = 4;
    conn.uri = "mock";

    conn.query_string = queryString.c_str();

    Request request(&conn);
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



