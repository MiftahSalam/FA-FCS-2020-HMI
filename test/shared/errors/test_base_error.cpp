#include "test_base_error.h"

#include "src/shared/common/errors/base_error.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QtDebug>
#include <QtTest>

void TestErrors::throwBaseError()
{
    QSKIP("");
    try {
        throw BaseError(1,"test");
    } catch (BaseError &e) {
        qDebug()<<"caught base error";
    }
}

void TestErrors::throwFileNotFoundError()
{
    QSKIP("");
    try {
        throw ErrFileNotFound();
    } catch (ErrFileNotFound &e) {
        qDebug()<<"caught file not found error";
    }

    QVERIFY_EXCEPTION_THROWN(throw ErrFileNotFound(), BaseError);
}
