#ifndef TEST_USECASE_OSD_CMS_H
#define TEST_USECASE_OSD_CMS_H

#include "test/testsuit.h"

class TestServiceOSDCms: public TestSuit
{
    Q_OBJECT
public:
    using TestSuit::TestSuit;

private slots:
    void getInstance();
};
static TestServiceOSDCms TEST_SERVICE_OSD_CMS;

#endif // TEST_USECASE_OSD_CMS_H
