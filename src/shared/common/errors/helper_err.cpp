#include "helper_err.h"
#include "src/shared/common/errors/err_http.h"

void ErrHelper::throwHttpError(QNetworkReply::NetworkError err)
{
    switch (err) {
    case QNetworkReply::NoError:
        break;
    case QNetworkReply::ConnectionRefusedError:
        throw ErrHttpConnRefused();
        break;
    default:
        throw ErrHttpUnknown();
        break;
    }
}
