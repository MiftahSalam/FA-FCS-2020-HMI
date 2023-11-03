#ifndef ERRORS_H
#define ERRORS_H

#include <QException>
#include <QString>

const int ERROR_CODE_FILE_NOT_FOUND = 1;
const int ERROR_CODE_FILE_CANNOT_BE_OPENED = 2;

class BaseError: public QException
{
public:
    BaseError(int code, const QString &message);

private:
    int code;
    QString message;

    // QException interface
public:
    void raise() const;
    QException *clone() const;

    int getCode() const;
    QString getMessage() const;
};

#endif // ERRORS_H
