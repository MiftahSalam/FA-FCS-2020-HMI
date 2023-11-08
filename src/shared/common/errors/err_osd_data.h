#ifndef ERROSDDATAPOSITION_H
#define ERROSDDATAPOSITION_H

#include "err_base.h"

class ErrOSDDataInvalidFormat: public BaseError
{
public:
    ErrOSDDataInvalidFormat();
};

class ErrOSDDataConversion: public BaseError
{
public:
    ErrOSDDataConversion();
};

class ErrOSDDataOutOfRange: public BaseError
{
public:
    ErrOSDDataOutOfRange();
};

#endif // ERROSDDATAPOSITION_H
