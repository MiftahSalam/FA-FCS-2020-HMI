#ifndef TESTBASEERROR_H
#define TESTBASEERROR_H

#include "test/testsuit.h"

class TestErrors: public TestSuit
{
    Q_OBJECT
public:
    using TestSuit::TestSuit;
private slots:
    void throwBaseError();
    void throwFileNotFoundError();
};
static TestErrors TEST_ERRORS;

#endif // TESTBASEERROR_H
