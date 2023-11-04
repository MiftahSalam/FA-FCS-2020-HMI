#ifndef HTTPCLIENTWRAPPER_H
#define HTTPCLIENTWRAPPER_H

#include <QNetworkReply>
#include <QObject>

class HttpClientWrapper : public QObject
{
    Q_OBJECT
public:
    explicit HttpClientWrapper(QObject *parent = nullptr);

protected slots:
    virtual void onReplyFinished() {};

protected:
    QNetworkReply *httpResponse;
    QNetworkAccessManager httpClient;

signals:

};

#endif // HTTPCLIENTWRAPPER_H
