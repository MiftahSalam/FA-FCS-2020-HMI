#ifndef HTTPERROR_H
#define HTTPERROR_H

#include "err_base.h"

class ErrHttpUnknown: public BaseError
{
public:
    ErrHttpUnknown();
};

class ErrHttpConnRefused: public BaseError
{
public:
    ErrHttpConnRefused();
};

#endif // HTTPERROR_H
