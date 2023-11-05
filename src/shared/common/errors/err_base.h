#ifndef ERRORS_H
#define ERRORS_H

#include <QException>
#include <QString>

const QPair<int, QString> ERROR_CODE_UNKNOWN(-1, "Unknown error");

const QPair<int, QString> ERROR_NO(0, "No error");

const QPair<int, QString> ERROR_CODE_FILE_NOT_FOUND(11, "File not found");
const QPair<int, QString> ERROR_CODE_FILE_CANNOT_BE_OPENED(12, "File cannot not be opened");

const QPair<int, QString> ERROR_CODE_OBJECT_CREATE_REQUIRED_ARGUMENT_NOT_PROVIDE(21, "One or more constructor arguments are not provided");

const QPair<int, QString> ERROR_CODE_JSON_PARSE(31, "error when parse json");

const QPair<int, QString> ERROR_CODE_HTTP_RESPONSE_UNKNOWN(40, "unknown http response error");
const QPair<int, QString> ERROR_CODE_HTTP_RESPONSE_CONNECTION_REFUSED(41, "cannot connect to http server");

class BaseError: public QException
{
public:
    BaseError(int code, const QString &message);

private:
    int code;
    QString message;

    // QException interface
public:
    void raise() const override;
    QException *clone() const override;

    int getCode() const;
    QString getMessage() const;
};

class NoError: public BaseError
{
public:
    NoError();
};

class ErrUnknown: public BaseError
{
public:
    ErrUnknown();
};

#endif // ERRORS_H
