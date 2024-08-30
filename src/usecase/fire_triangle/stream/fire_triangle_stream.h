#ifndef FIRE_TRIANGLE_STREAM_H
#define FIRE_TRIANGLE_STREAM_H

#include <QObject>

#include "src/infra/store/fire_triangle/fire_triangle_repository.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/fire_triangle/stream/fire_triangle_service.h"

class FireTriangleStream : public QObject
{
public:
    FireTriangleStream(
        QObject *parent = nullptr,
        MessagingTcpConfig *config = nullptr,
        FireTriangleRepository *repoFta = nullptr
        );

    FireTriangleService *getServiceFireTriangle() const;

private:
    MessagingTcpConfig *consumerConfig;
    FireTriangleService *serviceFireTriangle;
};

#endif // FIRE_TRIANGLE_STREAM_H
