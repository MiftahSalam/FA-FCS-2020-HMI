#ifndef ENGAGEMENT_DATA_BASE_STREAM_H
#define ENGAGEMENT_DATA_BASE_STREAM_H

#include "src/shared/common/errors/err_base.h"
#include <QByteArray>

template <typename StreamModel> class EngagementStreamBase
{
public:
    virtual void onDataReceived(QByteArray data) = 0;
    virtual void signalDataProcessed(StreamModel data) = 0;
    virtual BaseError check() = 0;
//    virtual void periodUpdate() = 0;
};

#endif // ENGAGEMENT_DATA_BASE_STREAM_H
