#ifndef ERRENGAGEMENTTRACKNOTFOUND_H
#define ERRENGAGEMENTTRACKNOTFOUND_H

#include "err_base.h"

class ErrEngagementTrackNotFound: public BaseError
{
public:
    ErrEngagementTrackNotFound();
};

class ErrEngagementInvalidOSD: public BaseError
{
public:
    ErrEngagementInvalidOSD();
};

class ErrEngagementInvalidIdentity: public BaseError
{
public:
    ErrEngagementInvalidIdentity();
};

class ErrEngagementTrackNotEngageable: public BaseError
{
public:
    ErrEngagementTrackNotEngageable();
};

class ErrEngagementTrackAlreadyEngaged: public BaseError
{
public:
    ErrEngagementTrackAlreadyEngaged();
};

#endif // ERRENGAGEMENTTRACKNOTFOUND_H
