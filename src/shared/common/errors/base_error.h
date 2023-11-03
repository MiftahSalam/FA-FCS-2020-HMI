#ifndef ERRORS_H
#define ERRORS_H

#include <QString>

class BaseError
{
public:
    BaseError(int code, const QString &message);

private:
    int code;
    QString message;
};

#endif // ERRORS_H
