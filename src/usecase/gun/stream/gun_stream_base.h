#ifndef GUN_STREAM_BASE_H
#define GUN_STREAM_BASE_H

#include <QTimer>
#include "src/shared/common/errors/err_base.h"
#include <QByteArray>

template <typename GunModel> class GunStreamBase
{
public:
    virtual void onDataReceived(QByteArray data) = 0;
    virtual void signalDataProcessed(GunModel data) = 0;
    virtual BaseError check() = 0;
    virtual void periodUpdate() = 0;

protected:
    QTimer *timer;

private:
    virtual void handleError(const QString &err) = 0;
};
#endif // GUN_STREAM_BASE_H
