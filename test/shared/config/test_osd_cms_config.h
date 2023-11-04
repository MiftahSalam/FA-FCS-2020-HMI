#ifndef TESTOSDCMSCONFIG_H
#define TESTOSDCMSCONFIG_H

#include "test/testsuit.h"

class TestOSDCMSConfig: public TestSuit
{
    Q_OBJECT
public:
    using TestSuit::TestSuit;

private slots:
    void getInstanceFailed();
    void getInstanceOk();

};
static TestOSDCMSConfig TEST_CONFIG_OSD_CMS;

#endif // TESTOSDCMSCONFIG_H
