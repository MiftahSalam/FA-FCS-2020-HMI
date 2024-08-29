#include "fire_triangle_service.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

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

        qDebug()<<Q_FUNC_INFO<<"data fire triangle: ttlf ->"<<model.getTtlf()<<"ttlf X->"<<model.getTtlfX()
                 <<"ttlf y ->"<<model.getTtlfY();

        repoFireTriangle->SetFireTriangle(FireTriangleEntity(
            model.getTtlf(),
            model.getTtlfX(),
            model.getTtlfY()
            ));

        emit signalDataProcessed(model);

    }catch(ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void FireTriangleService::periodeUpdate()
{
    check();
    qDebug() << Q_FUNC_INFO;
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

