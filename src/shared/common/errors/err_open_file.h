#ifndef ERR_OPEN_FILE_H
#define ERR_OPEN_FILE_H

#include "base_error.h"


class ErrFileNotFound: public BaseError
{
public:
    ErrFileNotFound();
};

#endif // ERR_OPEN_FILE_H
