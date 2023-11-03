#ifndef TEST_USECASE_OSD_CMS_H
#define TEST_USECASE_OSD_CMS_H

#include "test/testsuit.h"

class TestServiceOSDCms: public TestSuit
{
    Q_OBJECT
public:
    using TestSuit::TestSuit;
    ~TestServiceOSDCms();

private slots:
    void createIServiceObject();
};
static TestServiceOSDCms TEST_SERVICE_OSD_CMS;

#endif // TEST_USECASE_OSD_CMS_H
