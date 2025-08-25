#include "test_base_error.h"

#include "src/shared/common/errors/err_base.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QtDebug>
#include <QtTest>

void TestErrors::throwBaseError()
{
    QSKIP("");
    QVERIFY_EXCEPTION_THROWN(throw BaseError(0, "test error"), BaseError);
}

void TestErrors::throwFileNotFoundError()
{
    QSKIP("");
    QVERIFY_EXCEPTION_THROWN(throw ErrFileNotFound(), ErrFileNotFound);
}
