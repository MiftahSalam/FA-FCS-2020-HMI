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

const QPair<int, QString> ERROR_CODE_AMQP_INVALID_EXCHANGE_TYPE(51, "amqp invalid exchange type config");

const QPair<int, QString> ERROR_CODE_MESSAGING_INVALID_TCP_CONFIG(61, "messaging invalid tcp  config");
const QPair<int, QString> ERROR_CODE_MESSAGING_NOT_CONNECTED(62, "messaging not connected");
const QPair<int, QString> ERROR_CODE_MESSAGING_NO_DATA(63, "messaging no data received");
const QPair<int, QString> ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT(64, "messaging invalid data format");

const QPair<int, QString> ERROR_CODE_OSD_DATA_INVALID_FORMAT(71, "osd data invalid format");
const QPair<int, QString> ERROR_CODE_OSD_DATA_CONVERSION(72, "osd data cannot convert data");
const QPair<int, QString> ERROR_CODE_OSD_DATA_OUT_OF_RANGE(73, "osd data out of range");
const QPair<int, QString> ERROR_CODE_OSD_DATA_PARTIALLY_INVALID(74, "osd data some data are invalid");
const QPair<int, QString> ERROR_CODE_OSD_DATA_INVALID(75, "osd data invalid");

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
