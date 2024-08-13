#ifndef GUN_STREAM_BASE_H
#define GUN_STREAM_BASE_H

#include "src/shared/common/errors/err_base.h"
#include <QByteArray>

template <typename Model> class GunStreamBase
{
public:
    virtual void onDataReceived(QByteArray data) = 0;
    virtual void signalDataProcessed(Model data) = 0;
    virtual BaseError check() = 0;

private:
    virtual void handleError(const QString &err) = 0;
};
#endif // GUN_STREAM_BASE_H
