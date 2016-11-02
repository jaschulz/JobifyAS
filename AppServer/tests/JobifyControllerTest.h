#ifndef JOBIFYCONTROLLERTEST_H
#define JOBIFYCONTROLLERTEST_H

#include <mongoose/Request.h>
#include <mongoose/mongoose.h>
#include <mongoose/JsonResponse.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace Mongoose;

class JobifyControllerTest {

public:

    JobifyControllerTest();

    Request makeDummyBodyRequest(string data, string method) const;

    Request generateRequest(string method, string queryString) const;

    Request makeDummyRequest(string method, string queryString) const;

    virtual ~JobifyControllerTest();
};


#endif
