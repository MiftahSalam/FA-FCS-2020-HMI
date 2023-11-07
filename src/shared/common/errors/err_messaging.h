#ifndef ERRMESSAGINGTCPCONFIG_H
#define ERRMESSAGINGTCPCONFIG_H

#include "err_base.h"

class ErrMessagingTcpConfig: public BaseError
{
public:
    ErrMessagingTcpConfig();
};


class ErrMessagingDataInvalidFormat: public BaseError
{
public:
    ErrMessagingDataInvalidFormat();
};

class ErrMessagingDataNoData: public BaseError
{
public:
    ErrMessagingDataNoData();
};

class ErrMessagingNotConnected: public BaseError
{
public:
    ErrMessagingNotConnected();
};

#endif // ERRMESSAGINGTCPCONFIG_H
