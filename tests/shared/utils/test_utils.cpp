#include "test_utils.h"
#include "qtestcase.h"
#include "src/shared/utils/utils.h"

void TestUtils::latitudeConverter()
{
    QString result = Utils::latDecToStringDegree(12.544);
    QCOMPARE(result,"12° 20' 43.224");
}
