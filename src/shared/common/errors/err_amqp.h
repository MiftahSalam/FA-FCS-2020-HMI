#ifndef ERRAMQP_H
#define ERRAMQP_H

#include "err_base.h"

class ErrAmqpInvalidExchangeType: public BaseError
{
public:
    ErrAmqpInvalidExchangeType();
};

#endif // ERRAMQP_H
