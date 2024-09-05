#ifndef ERRHELPER_H
#define ERRHELPER_H


#include "qnetworkreply.h"

class ErrHelper
{
public:
    static void throwHttpError(QNetworkReply::NetworkError err);
    static void throwTrackEngageError(int responseCode);
};

#endif // ERRHELPER_H
