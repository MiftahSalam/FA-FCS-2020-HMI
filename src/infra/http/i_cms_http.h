#ifndef ICMSHTTP_H
#define ICMSHTTP_H

#include <QNetworkReply>

#include "src/infra/core/base_response.h"

template<typename M, typename R> class ICmsHttp
{
public:
    virtual void set(R request) = 0;

protected:
    virtual BaseResponse<M> toResponse(QByteArray raw) = 0;
    virtual BaseResponse<M> errorResponse(QNetworkReply::NetworkError err) = 0;
};

#endif // ICMSHTTP_H
