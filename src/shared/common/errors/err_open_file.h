#ifndef ERR_OPEN_FILE_H
#define ERR_OPEN_FILE_H

#include "base_error.h"

#include <QException>

class ErrFileNotFound: public BaseError
{
public:
    ErrFileNotFound();
};

class ErrFileCannotOpened: public BaseError
{
public:
    ErrFileCannotOpened();
};

#endif // ERR_OPEN_FILE_H
