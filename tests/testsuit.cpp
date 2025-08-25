#include "testsuit.h"

TestSuit::TestSuit()
{
    suits().push_back(this);
}

std::vector<QObject *> &TestSuit::suits()
{
    static std::vector<QObject*> objects;

    return objects;
}

TestSuit::~TestSuit() {

}
