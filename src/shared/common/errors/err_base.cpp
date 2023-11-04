#include "err_base.h"


NoError::NoError(): BaseError(ERROR_NO.first, ERROR_NO.second)
{

}

BaseError::BaseError(int code, const QString &message) : code(code),
    message(message)
{}


void BaseError::raise() const
{
    throw *this;
}

QException *BaseError::clone() const
{
    return new BaseError(*this);
}

int BaseError::getCode() const
{
    return this->code;
}

QString BaseError::getMessage() const
{
    return this->message;
}

