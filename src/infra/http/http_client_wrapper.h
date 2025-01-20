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
    QNetworkAccessManager httpClient;
    QMap<QString, QVariant> reqCtxObj;

signals:

};

#endif // HTTPCLIENTWRAPPER_H
