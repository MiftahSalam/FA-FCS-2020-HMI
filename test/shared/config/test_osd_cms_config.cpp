#include "test_osd_cms_config.h"

#include "src/shared/config/osd_cms_config.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QSettings>
#include <QtDebug>
#include <QtTest/QTest>

const QString COMMON_CONFIG_PATH_TEST = QDir::homePath()+QDir::separator()+".fa-hmi-test.conf";

void TestOSDCMSConfig::getInstanceOk()
{
    QFile testFile(COMMON_CONFIG_PATH_TEST);
    if(!testFile.open(QIODevice::ReadWrite)) {
        QFAIL("cannot create file");
    }

    try {
        OSDCmsConfig *cfg = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH_TEST);
        QVERIFY(cfg != nullptr);
    } catch (ErrFileNotFound &e) {
        QFAIL("caught ErrFileNotFound");
    }
}

void TestOSDCMSConfig::getManualUrlOk()
{
    QFile testFile(COMMON_CONFIG_PATH_TEST);
    if(!testFile.open(QIODevice::ReadWrite)) {
        QFAIL("cannot create file");
    }

    OSDCmsConfig *cfg = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH_TEST);
    QCOMPARE(cfg->getManualDataUrl(), "http://localhost:8081/osd/api/v1/manual_data");
}

void TestOSDCMSConfig::getModeUrlOk()
{
    QFile testFile(COMMON_CONFIG_PATH_TEST);
    if(!testFile.open(QIODevice::ReadWrite)) {
        QFAIL("cannot create file");
    }

    OSDCmsConfig *cfg = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH_TEST);
    QCOMPARE(cfg->getModeUrl(), "http://localhost:8081/osd/api/v1/mode");
}

void TestOSDCMSConfig::getInstanceFailed()
{
    try {
        OSDCmsConfig::getInstance("");
    } catch (ErrFileNotFound &e) {
    }
}
