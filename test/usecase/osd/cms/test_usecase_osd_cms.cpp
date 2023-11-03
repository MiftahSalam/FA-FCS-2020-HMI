#include <QtTest/QTest>

#include "src/usecase/osd/cms/osd_cms.h"
#include "test_usecase_osd_cms.h"

void TestServiceOSDCms::createIServiceObject()
{
    OSDCMS *cmsSrv = new OSDCMS();
}

TestServiceOSDCms::~TestServiceOSDCms() {

}

//QTEST_MAIN(TestServiceOSDCms)
//#include "testserviceosdcms.moc"
