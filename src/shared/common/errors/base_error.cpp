#include "base_error.h"

BaseError::BaseError(int code, const QString &message) : code(code),
    message(message)
{}
