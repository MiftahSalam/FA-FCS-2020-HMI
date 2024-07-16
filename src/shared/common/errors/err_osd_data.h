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

class ErrOSDDataPartiallyInvalid: public BaseError
{
public:
    ErrOSDDataPartiallyInvalid();
};

class ErrOSDDataInvalid: public BaseError
{
public:
    ErrOSDDataInvalid();
};

#endif // ERROSDDATAPOSITION_H
