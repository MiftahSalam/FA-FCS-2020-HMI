#include "test_osd_cms_config.h"
//#include "qtestcase.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/shared/common/errors/err_open_file.h"

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
        OSDCmsConfig *cfg = OSDCmsConfig::getInstance("");
        QVERIFY(cfg != nullptr);
    } catch (ErrFileNotFound &e) {
        QFAIL("caught ErrFileNotFound");
    }
}

void TestOSDCMSConfig::getInstanceFailed()
{
    try {
        OSDCmsConfig *cfg = OSDCmsConfig::getInstance("");
    } catch (ErrFileNotFound &e) {
        qWarning()<<"caught ErrFileNotFound";
    }
}
