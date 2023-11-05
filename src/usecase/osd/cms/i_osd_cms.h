#ifndef IOSDCMS_H
#define IOSDCMS_H

#include <QNetworkReply>

#include "src/model/base_response.h"

template<typename M, typename R> class IOSDCMS
{
public:
    virtual void set(R request) = 0;

protected:
    virtual BaseResponse<M> toResponse(QByteArray raw) = 0;
    virtual BaseResponse<M> errorResponse(QNetworkReply::NetworkError err) = 0;
};

#endif // IOSDCMS_H
