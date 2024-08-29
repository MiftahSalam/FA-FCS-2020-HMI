#ifndef FIRE_TRIANGLE_SERVICE_H
#define FIRE_TRIANGLE_SERVICE_H

#include "src/domain/fire_triangle/repository/fire_triangle_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/fire_triangle/fire_triangle_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "qtimer.h"

class FireTriangleService : public QObject
{
    Q_OBJECT
public:
    FireTriangleService(FireTriangleService &other) = delete;
    void operator=(const FireTriangleService&) = delete;
    static FireTriangleService *getInstance(
        TcpMessagingOpts *config,
        FireTriangleBaseRepository *_repoFireTriangle
        );

    BaseError check();

signals:
    void signalDataProcessed(FireTriangleModel data);

protected:
    FireTriangleService(
        TcpMessagingOpts *config = nullptr,
        FireTriangleBaseRepository *repoFireTriangle = nullptr
        );
    QTimer *timer;

private slots:
    void onDataReceived(QByteArray data);
    void periodeUpdate();

private:
    static FireTriangleService *fireTriangleService;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    FireTriangleBaseRepository* repoFireTriangle;

    BaseError currentErr;

    void handleError(const QString &err);
};

#endif // FIRE_TRIANGLE_SERVICE_H
