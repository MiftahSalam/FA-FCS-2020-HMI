#include "helper_err.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/err_engagement.h"

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

void ErrHelper::throwTrackEngageError(int responseCode)
{
    switch (responseCode) {
    case 404000:
        throw ErrEngagementTrackNotFound();
        break;
    case 400000:
        throw ErrEngagementTrackAlreadyEngaged();
        break;
    default:
        break;
    }
}
