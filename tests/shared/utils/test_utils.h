#ifndef TESTUTILS_H
#define TESTUTILS_H

#include "test/testsuit.h"

class TestUtils: public TestSuit
{
public:
    using TestSuit::TestSuit;

private slots:
    void latitudeConverter();

};
static TestUtils TEST_UTILS;

#endif // TESTUTILS_H
