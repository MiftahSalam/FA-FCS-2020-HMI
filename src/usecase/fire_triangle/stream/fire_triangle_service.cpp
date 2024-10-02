#include "fire_triangle_service.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FireTriangleService)
#else
#include <QDebug>
#endif

FireTriangleService *FireTriangleService::fireTriangleService = nullptr;

FireTriangleService::FireTriangleService(
    TcpMessagingOpts *config,
    FireTriangleBaseRepository *_repoFireTriangle):
    cfg(config), repoFireTriangle(_repoFireTriangle), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &FireTriangleService::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FireTriangleService::periodeUpdate);
    timer->start(1000);
}

void FireTriangleService::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        FireTriangleModel model(respObj["ttlf"].toDouble(),respObj["ttlf_x"].toDouble(),respObj["ttlf_y"].toDouble());

        repoFireTriangle->SetFireTriangle(FireTriangleEntity(
            model.getTtlf(),
            model.getTtlfX(),
            model.getTtlfY()
            ));

        emit signalDataProcessed(model);

    }catch(ErrJsonParse &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
    }
}

void FireTriangleService::periodeUpdate()
{
    check();
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO;
#else
    qDebug()<<Q_FUNC_INFO;
#endif
}

void FireTriangleService::handleError(const QString &err)
{

}

FireTriangleService *FireTriangleService::getInstance(
    TcpMessagingOpts *config = nullptr,
    FireTriangleBaseRepository *_repoFireTriangle = nullptr
    )
{
    if(fireTriangleService == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(_repoFireTriangle == nullptr) {
            throw ErrObjectCreation();
        }

        fireTriangleService = new FireTriangleService(config, _repoFireTriangle);
    }

    return fireTriangleService;
}

BaseError FireTriangleService::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

