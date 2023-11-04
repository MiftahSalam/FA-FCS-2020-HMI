#ifndef ERRORS_H
#define ERRORS_H

#include <QException>
#include <QString>

const QPair<int, QString> ERROR_CODE_UNKNOWN(-1, "Unknown error");

const QPair<int, QString> ERROR_CODE_FILE_NOT_FOUND(1, "File not found");
const QPair<int, QString> ERROR_CODE_FILE_CANNOT_BE_OPENED(2, "File cannot not be opened");

const QPair<int, QString> ERROR_CODE_OBJECT_CREATE_REQUIRED_ARGUMENT_NOT_PROVIDE(3, "One or more constructor arguments are not provided");

const QPair<int, QString> ERROR_CODE_JSON_PARSE(4, "error when parse json");

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

#endif // ERRORS_H
