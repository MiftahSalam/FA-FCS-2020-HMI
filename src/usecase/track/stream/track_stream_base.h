#ifndef TRACK_STREAM_BASE_H
#define TRACK_STREAM_BASE_H

#include "qtimer.h"
#include "src/shared/common/errors/err_base.h"
#include <QByteArray>

template <typename TrackModel> class TrackStreamBase
{
public:
    virtual void onDataReceived(QByteArray data) = 0;
    virtual void signalDataProcessed(TrackModel data) = 0;
    virtual BaseError check() = 0;
    virtual void periodUpdate() = 0;

protected:
    QTimer *timer;

private:
    virtual void handleError(const QString &err) = 0;
};

#endif // TRACK_STREAM_BASE_H
