#include <QtTest/QTest>

#include "src/infra/core/cms/osd/osd_cms.h"
#include "test_usecase_osd_cms.h"

const QString COMMON_CONFIG_PATH_TEST = QDir::homePath()+QDir::separator()+".fa-hmi-test.conf";

void TestServiceOSDCms::setPosition()
{
    OSDCmsConfig *cfg = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH_TEST);
    OSDCMS *cms = new OSDCMS(nullptr, cfg);
    OSDSetPositionRequest req(1.2, -113.5);

    try {
        cms->getServiceOSDCMSPosition()->set(req);
    } catch (...) {
        QFAIL("unexpected thrown exception");
    }

}

void TestServiceOSDCms::createObjectOk()
{
    OSDCmsConfig *cfg = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH_TEST);

    try {
        OSDCMS *cms = nullptr;
        cms = new OSDCMS(nullptr, cfg);
        QVERIFY(cms != nullptr);
    } catch (...) {
        QFAIL("thrown exception ecpected");
    }

}

void TestServiceOSDCms::createObjectFailed()
{
    try {
        OSDCMS *cms = new OSDCMS();
        Q_UNUSED(cms);
        QFAIL("thrown exception ecpected");
    } catch (...) {
    }

}
