#ifndef ERRORS_H
#define ERRORS_H

#include <QException>
#include <QString>

const QPair<int, QString> ERROR_CODE_FILE_NOT_FOUND(1, "File not found");
const QPair<int, QString> ERROR_CODE_FILE_CANNOT_BE_OPENED(2, "File cannot not be opened");

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
