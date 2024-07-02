#ifndef BASERESPONSE_H
#define BASERESPONSE_H

#include <QString>

template <typename T> class BaseResponse
{
public:
    BaseResponse(int code, QString msg, const T respData) {
        httpCode = code;
        message = msg;
        data = respData;
    }

    int getHttpCode() const {
        return httpCode;
    }
    QString getMessage() const {
        return message;
    }
    T getData() const {
        return data;
    }

private:
    int httpCode;
    QString message;
    T data;
};

#endif // BASERESPONSE_H
