#include "gun_feedback_barrel_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

GunFeedbackBarrelStream *GunFeedbackBarrelStream::gunBarrelStream = nullptr;

GunFeedbackBarrelStream::GunFeedbackBarrelStream(
    TcpMessagingOpts *config,
    GunFeedbackRepository *repoGunFback
    ): cfg(config), repoGunFback(repoGunFback), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &GunFeedbackBarrelStream::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GunFeedbackBarrelStream::periodUpdate);
    timer->start(1000);
}

void GunFeedbackBarrelStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
    GunBarrelModel model(respObj["azimuth"].toDouble(),
                             respObj["elevation"].toDouble()
                             );

        qDebug()<<Q_FUNC_INFO<<"data gun barrel: azimuth->"<<model.getAzimuth()<<"elevation->"<<model.getElevation();

        repoGunFback->SetBarrel(
            model.getAzimuth(),
            model.getElevation()
            );

        emit signalDataProcessed(model);

    }catch(ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void GunFeedbackBarrelStream::periodUpdate()
{
    check();
    qDebug() << Q_FUNC_INFO;
}

void GunFeedbackBarrelStream::handleError(const QString &err)
{

}

GunFeedbackBarrelStream *GunFeedbackBarrelStream::getInstance(
    TcpMessagingOpts *config = nullptr,
    GunFeedbackRepository *repoGunFback = nullptr
    )
{
    if(gunBarrelStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(repoGunFback == nullptr) {
            throw ErrObjectCreation();
        }

        gunBarrelStream = new GunFeedbackBarrelStream(config, repoGunFback);
    }

    return gunBarrelStream;
}

BaseError GunFeedbackBarrelStream::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

